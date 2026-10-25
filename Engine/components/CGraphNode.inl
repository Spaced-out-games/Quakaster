#include <entt/entity/registry.hpp>
#include <utility>

namespace qk {


	template <typename Func>
	void CGraphNode::forEachDescendant(const entt::registry& r, entt::entity e, Func&& func) {
		auto cur = CGraphNode::firstChild(r, e);
		while (cur != entt::null) {
			func(cur);
			auto nxt = CGraphNode::firstChild(r, cur);
			while (nxt == entt::null && cur != e) {
				nxt = CGraphNode::nextSibling(r, cur);
				if (nxt == entt::null) cur = CGraphNode::parentOf(r, cur);
			}
			cur = nxt;
		}
	}


	template <typename Func>
	void CGraphNode::forEachSibling(const entt::registry& r, entt::entity e, Func&& func) {
		for (auto s = CGraphNode::firstSibling(r, e); s != entt::null; s = CGraphNode::nextSibling(r, s))
			func(s);
	}


	template <class Flag>
	void CGraphNode::clearDescendantFlags(entt::registry& r, entt::entity target) {
		forEachDescendant(r, target, [&](entt::entity d) { r.remove<Flag>(d); });
	}


	template <class Flag, class... Args>
	void CGraphNode::invalidate(entt::registry& r, entt::entity target, Args&&... args) {
		if (!CGraphNode::hasHeirarchy(r, target)) return;

		bool covered = false;
		for (auto p = CGraphNode::parentOf(r, target); p != entt::null; p = CGraphNode::parentOf(r, p)) {
			if (r.all_of<Flag>(p)) { covered = true; break; }
		}

		if (covered) r.remove<Flag>(target);
		else if (!r.all_of<Flag>(target)) r.emplace<Flag>(target, std::forward<Args>(args)...);

		clearDescendantFlags<Flag>(r, target);
	}


	template <class Flag>
	void CGraphNode::clear(entt::registry& r) {
		r.clear<Flag>();
	}


}