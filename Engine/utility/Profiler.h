#pragma once


#include <unordered_map>
#include <algorithm> // std::clamp
#include <memory> // std::make_unique, std::unique_ptr
#include <utility/time.h>   // qk::utility::now() -> uint64_t nanoseconds
#include <vector>
#include <array>
#include <core.h>



namespace qk::utility {


    inline constexpr size_t kHistory = 240;   // frames of history kept per section

    using History = std::array<f32, kHistory>;

    struct Section {
        str name;
        History     ms{};          // ring buffer: total ms spent in this section, per frame
        f32       acc_ms = 0.0f; // accumulator for the frame in progress
    };

    struct Stat { f32 last = 0.0f, avg = 0.0f, max = 0.0f; };


    // ---------------------------------------------------------------------------
    // Data store
    // ---------------------------------------------------------------------------

    class Profiler {
        std::vector<Section> m_Sections;
        History              m_Frame{};
        size_t          m_Head = 0, m_Count = 0;
        u64        m_LastFrame = 0;
        bool                 m_Paused = false;
    public:

        


        // Call from ScopeTimer's destructor. A section hit several times in one frame is summed.
        void record(str_view name, u64 ns) {
            if (m_Paused) return;
            section(name).acc_ms += static_cast<f32>(static_cast<double>(ns) / 1e6);
        }

        // Call once per frame, after everything you want timed.
        void end_frame() {
            const u64 t = now();
            if (m_LastFrame != 0 && !m_Paused) {
                m_Frame[m_Head] = static_cast<f32>(static_cast<double>(t - m_LastFrame) / 1e6);
                for (auto& s : m_Sections) {
                    s.ms[m_Head] = s.acc_ms;   // sections not hit this frame store 0
                    s.acc_ms = 0.0f;
                }
                m_Head = (m_Head + 1) % kHistory;
                m_Count = (std::min)(m_Count + 1, kHistory);
            }
            m_LastFrame = t;   // updated even while paused so unpausing doesn't log a huge frame
        }

        void reset() {
            m_Sections.clear();
            m_Frame.fill(0.0f);
            m_Head = m_Count = 0;
            m_LastFrame = 0;
        }

        void set_paused(bool p) { m_Paused = p; }
        bool paused() const { return m_Paused; }

        const std::vector<Section>& sections() const { return m_Sections; }
        const History& frame_ms() const { return m_Frame; }   // wall-clock frame-to-frame time
        size_t    count() const { return m_Count; }

        // i = 0 is the most recent frame.
        f32 recent(const History& h, size_t i) const {
            return h[(m_Head + kHistory - 1 - i) % kHistory];
        }

        // Stats over the last `window` frames (clamped to what has been recorded).
        Stat stat(const History& h, size_t window) const {
            Stat s;
            if (m_Count == 0) return s;
            window = std::clamp<size_t>(window, 1, m_Count);
            f32 sum = 0.0f;
            for (size_t i = 0; i < window; ++i) {
                const f32 v = recent(h, i);
                sum += v;
                s.max = (std::max)(s.max, v);
            }
            s.last = recent(h, 0);
            s.avg = sum / static_cast<f32>(window);
            return s;
        }

    private:
        Section& section(str_view name) {
            for (auto& s : m_Sections)
                if (s.name == name) return s;
            m_Sections.push_back(Section{ str(name) });
            return m_Sections.back();
        }


    };

    // ---------------------------------------------------------------------------
    // Registry: owns profilers by name and hands out stable, non-owning pointers.
    // unique_ptr keeps each Profiler at a fixed address, so a Profiler* held by a
    // timer or widget survives the map rehashing as more profilers are added.
    // ---------------------------------------------------------------------------

    class ProfilerRegistry {

        struct StringHash {   // lets find()/destroy() take a string_view without building a str
            using is_transparent = void;
            size_t operator()(str_view s) const noexcept { return std::hash<str_view>{}(s); }
        };

        std::unordered_map<str, std::unique_ptr<Profiler>, StringHash, std::equal_to<>> m_Profilers;

    public:
        // Returns the existing profiler if the name is already taken.
        Profiler& create(str name) {
            auto it = m_Profilers.find(name);
            if (it == m_Profilers.end())
                it = m_Profilers.emplace(std::move(name), std::make_unique<Profiler>()).first;
            return *it->second;
        }

        // nullptr if there is no such profiler. Don't cache the result past destroy().
        Profiler* find(str_view name) const {
            const auto it = m_Profilers.find(name);
            return it == m_Profilers.end() ? nullptr : it->second.get();
        }

        // Invalidates every Profiler* previously handed out for this name.
        bool destroy(str_view name) {
            const auto it = m_Profilers.find(name);
            if (it == m_Profilers.end()) return false;
            m_Profilers.erase(it);
            return true;
        }

        // Ends the current frame on every profiler. Call once per frame.
        void end_frame() {
            for (auto& [name, p] : m_Profilers) p->end_frame();
        }

        template <class F>
        void for_each(F&& f) const {
            for (const auto& [name, p] : m_Profilers) f(name, *p);
        }

        size_t size() const noexcept { return m_Profilers.size(); }

    private:
        

    };

    // ---------------------------------------------------------------------------
    // Widget
    // ---------------------------------------------------------------------------
    /*
    class ProfilerWidget : public Widget {
    public:
        // Non-owning: `prof` must outlive the widget, so the registry that owns it
        // should be destroyed after the layer stack that owns the widget.
        explicit ProfilerWidget(Profiler* prof, const char* title = "Profiler")
            : m_Prof(prof), m_Title(title) {}

        void set_profiler(Profiler* prof) { m_Prof = prof; }

        void set_open(bool open) { m_Open = open; }
        bool is_open() const { return m_Open; }

        void render() override {
            if (!m_Open || !m_Prof) return;
            const bool visible = gui::begin(m_Title, &m_Open);
            if (visible) draw();
            gui::end();   // always paired with begin(), even when collapsed
        }

    private:
        struct Row { const Profiler::Section* s; Profiler::Stat st; };

        // Colors are plain f32s so the widget needs nothing beyond gui::Color.
        static gui::Color green() { return { 0.40f, 1.00f, 0.40f }; }
        static gui::Color yellow() { return { 1.00f, 0.85f, 0.30f }; }
        static gui::Color red() { return { 1.00f, 0.35f, 0.35f }; }
        static gui::Color gray() { return { 0.85f, 0.85f, 0.85f }; }

        gui::Color frame_color(f32 ms) const {
            if (ms <= m_BudgetMs)        return green();
            if (ms <= m_BudgetMs * 2.0f) return yellow();
            return red();
        }

        gui::Color row_color(f32 ms) const {
            if (ms > m_BudgetMs * 0.5f) return red();
            if (ms > m_BudgetMs * 0.2f) return yellow();
            return gray();
        }

        static str bar(f32 frac, int width) {
            const int n = std::clamp(static_cast<int>(frac * static_cast<f32>(width) + 0.5f), 0, width);
            return "[" + str(static_cast<size_t>(n), '#')
                + str(static_cast<size_t>(width - n), '.') + "]";
        }

        void draw() {
            Profiler& p = *m_Prof;
            const size_t win = static_cast<size_t>(m_Window);
            char buf[256];

            // --- headline -------------------------------------------------------
            const Profiler::Stat fs = p.stat(p.frame_ms(), win);
            const f32 fps = fs.avg > 0.0f ? 1000.0f / fs.avg : 0.0f;
            std::snprintf(buf, sizeof buf, "%.1f FPS   %.3f ms avg   %.3f ms max", fps, fs.avg, fs.max);
            gui::text_colored(frame_color(fs.avg), buf);

            // --- frame-time sparkline (ASCII: the default font has no block glyphs) ---
            {
                static constexpr char kRamp[] = "_.-:=+*#@";   // 9 levels
                const size_t n = std::min<size_t>(60, p.count());
                f32 peak = 1e-6f;
                for (size_t i = 0; i < n; ++i) peak = std::max(peak, p.recent(p.frame_ms(), i));

                str line = "frames ";
                for (size_t i = n; i-- > 0;) {   // oldest on the left
                    const f32 v = p.recent(p.frame_ms(), i) / peak;
                    line += kRamp[std::clamp(static_cast<int>(v * 8.999f), 0, 8)];
                }
                gui::text(line.c_str());
            }

            gui::separator();

            // --- controls ---------------------------------------------------------
            bool paused = p.paused();
            if (gui::checkbox("Paused", paused)) p.set_paused(paused);
            gui::same_line();
            gui::checkbox("Sort by cost", m_Sort);
            gui::same_line();
            if (gui::button("Reset")) p.reset();

            gui::slider_int("Window (frames)", m_Window, 1, static_cast<int>(kHistory));
            gui::slider_f32("Budget (ms)", m_BudgetMs, 1.0f, 50.0f);

            gui::separator();

            // --- per-section rows -------------------------------------------------
            m_Rows.clear();
            for (const auto& s : p.sections())
                m_Rows.push_back(Row{ &s, p.stat(s.ms, win) });

            if (m_Sort)
                std::sort(m_Rows.begin(), m_Rows.end(),
                    [](const Row& a, const Row& b) { return a.st.avg > b.st.avg; });

            f32 ref = 1e-6f;   // the largest row fills the bar
            for (const Row& r : m_Rows) ref = std::max(ref, r.st.avg);

            std::snprintf(buf, sizeof buf, "%-14s %12s %12s", "section", "avg (ms)", "max (ms)");
            gui::text(buf);

            for (const Row& r : m_Rows) {
                str name = r.s->name;
                for (char& c : name) if (c == '%') c = '_';   // text() may treat its argument as a format string

                std::snprintf(buf, sizeof buf, "%-14.14s %8.3f avg %8.3f max ", name.c_str(), r.st.avg, r.st.max);
                str line = buf;
                line += bar(r.st.avg / ref, 24);
                gui::text_colored(row_color(r.st.avg), line.c_str());
            }
        }

        Profiler* m_Prof;
        const char* m_Title;
        bool             m_Open = true;
        bool             m_Sort = true;
        int              m_Window = 120;
        f32            m_BudgetMs = 16.67f;   // 60 fps
        std::vector<Row> m_Rows;                // reused every frame to avoid reallocating
    };*/

} // namespace qk::prof