#pragma once

#include "GUIWidget.h"
#include <gui_widget_rhi.h>
#include <utility/Profiler.h>
#include <gfx_rhi.h>

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

namespace gui_rhi = qk::gfx::rhi::gui;

namespace qk::gfx {

    using qk::gfx::gui::Widget;
    using qk::utility::History;
    using qk::utility::kHistory;
    using qk::utility::Profiler;
    using qk::utility::ProfilerRegistry;
    using qk::utility::Section;
    using qk::utility::Stat;

    // One window for every profiler in a registry: a single FPS headline and sparkline,
    // one set of controls (applied to all profilers), and one table with a row per
    // timed section, labelled "profiler/section" when there is more than one profiler.
    class ProfilerWidget : public Widget {
    public:
        // Non-owning: `registry` must outlive this widget, so declare the registry
        // before the layer stack that owns the widget.
        explicit ProfilerWidget(ProfilerRegistry* registry, const char* title = "Profiler")
            : m_Registry(registry), m_Title(title) {}

        void set_registry(ProfilerRegistry* registry) { m_Registry = registry; }

        void set_open(bool open) { m_Open = open; }
        bool is_open() const { return m_Open; }

        void render() override {
            if (!m_Open || !m_Registry)
                return;

            const bool visible = gui_rhi::begin(m_Title, &m_Open);
            if (visible)
                draw();

            gui_rhi::end();
        }

    private:
        struct Entry {
            std::string name;
            Profiler* prof;
        };

        struct Row {
            const std::string* profiler;   // points into m_Entries; valid for the current frame only
            const Section* s;
            Stat               st;
        };

        static gui_rhi::Color green() { return { 0.40f, 1.00f, 0.40f }; }
        static gui_rhi::Color yellow() { return { 1.00f, 0.85f, 0.30f }; }
        static gui_rhi::Color red() { return { 1.00f, 0.35f, 0.35f }; }
        static gui_rhi::Color gray() { return { 0.85f, 0.85f, 0.85f }; }

        gui_rhi::Color frame_color(float ms) const {
            if (ms <= m_BudgetMs)        return green();
            if (ms <= m_BudgetMs * 2.0f) return yellow();
            return red();
        }

        gui_rhi::Color row_color(float ms) const {
            if (ms > m_BudgetMs * 0.5f) return red();
            if (ms > m_BudgetMs * 0.2f) return yellow();
            return gray();
        }

        static std::string bar(float frac, int width) {
            const int n = std::clamp(
                static_cast<int>(frac * static_cast<float>(width) + 0.5f),
                0, width
            );

            return "[" + std::string(static_cast<std::size_t>(n), '#')
                + std::string(static_cast<std::size_t>(width - n), '.') + "]";
        }

        // text() may treat its argument as a format string, so keep '%' out of anything displayed.
        static std::string sanitize(std::string s) {
            for (char& c : s)
                if (c == '%')
                    c = '_';
            return s;
        }

        void draw() {
            // Collect and sort by name: the registry's hash map has no stable iteration order.
            m_Entries.clear();
            m_Registry->for_each([&](const auto& name, Profiler& p) {
                m_Entries.push_back(Entry{ std::string(name), &p });
            });

            if (m_Entries.empty()) {
                gui_rhi::text("No profilers registered.");
                return;
            }

            std::sort(m_Entries.begin(), m_Entries.end(),
                [](const Entry& a, const Entry& b) { return a.name < b.name; });

            const std::size_t win = static_cast<std::size_t>(m_Window);
            char buf[256];

            // Headline + sparkline, drawn once. Every profiler ends its frame together,
            // so any one of them has the same frame-to-frame timing; use the first.
            const Profiler& clock = *m_Entries.front().prof;
            const Stat fs = clock.stat(clock.frame_ms(), win);
            const float fps = fs.avg > 0.0f ? 1000.0f / fs.avg : 0.0f;

            std::snprintf(
                buf, sizeof buf,
                "%.1f FPS   %.3f ms avg   %.3f ms max",
                fps, fs.avg, fs.max
            );
            gui_rhi::text_colored(frame_color(fs.avg), buf);

            // ASCII sparkline: the default font has no block glyphs
            {
                static constexpr char kRamp[] = "_.-:=+*#@";
                const std::size_t n = std::min<std::size_t>(60, clock.count());

                float peak = 1e-6f;
                for (std::size_t i = 0; i < n; ++i)
                    peak = std::max(peak, clock.recent(clock.frame_ms(), i));

                std::string line = "frames ";
                for (std::size_t i = n; i-- > 0;) {
                    const float f = clock.recent(clock.frame_ms(), i) / peak;
                    line += kRamp[std::clamp(static_cast<int>(f * 8.999f), 0, 8)];
                }

                gui_rhi::text(line.c_str());
            }

            gui_rhi::separator();

            // Controls: one set, applied to every profiler.
            bool paused = std::all_of(m_Entries.begin(), m_Entries.end(),
                [](const Entry& e) { return e.prof->paused(); });
            if (gui_rhi::checkbox("Paused", paused))
                for (const Entry& e : m_Entries)
                    e.prof->set_paused(paused);

            gui_rhi::same_line();
            gui_rhi::checkbox("Sort by cost", m_Sort);

            gui_rhi::same_line();
            if (gui_rhi::button("Reset"))
                for (const Entry& e : m_Entries)
                    e.prof->reset();   // clears sections, so rows are collected after this

            gui_rhi::slider_int("Window (frames)", m_Window, 1, static_cast<int>(kHistory));
            gui_rhi::slider_float("Budget (ms)", m_BudgetMs, 1.0f, 50.0f);

            gui_rhi::separator();

            // One table: every section of every profiler.
            m_Rows.clear();
            for (const Entry& e : m_Entries)
                for (const Section& s : e.prof->sections())
                    m_Rows.push_back(Row{ &e.name, &s, e.prof->stat(s.ms, win) });

            if (m_Sort) {
                std::sort(m_Rows.begin(), m_Rows.end(),
                    [](const Row& a, const Row& b) { return a.st.avg > b.st.avg; });
            }

            float ref = 1e-6f;   // the largest row fills the bar
            for (const Row& r : m_Rows)
                ref = std::max(ref, r.st.avg);

            const bool prefixed = m_Entries.size() > 1;

            std::snprintf(buf, sizeof buf, "%-22s %8s     %8s", "section", "avg ms", "max ms");
            gui_rhi::text(buf);

            for (const Row& r : m_Rows) {
                const std::string label = prefixed
                    ? sanitize(*r.profiler + "/" + r.s->name)
                    : sanitize(r.s->name);

                std::snprintf(
                    buf, sizeof buf,
                    "%-22.22s %8.3f avg %8.3f max ",
                    label.c_str(), r.st.avg, r.st.max
                );

                std::string line = buf;
                line += bar(r.st.avg / ref, 24);

                gui_rhi::text_colored(row_color(r.st.avg), line.c_str());
            }
        }

        ProfilerRegistry* m_Registry;
        const char* m_Title;

        bool  m_Open = true;
        bool  m_Sort = true;
        int   m_Window = 120;
        float m_BudgetMs = 16.67f;   // 60 fps

        std::vector<Entry> m_Entries;   // reused every frame
        std::vector<Row>   m_Rows;      // reused every frame
    };

} // namespace qk::gfx