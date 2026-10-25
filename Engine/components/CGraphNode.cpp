#include "CGraphNode.h"
#include <entt/entity/registry.hpp>
#include <vector>
#include <string>

namespace qk {

	namespace {
		// Record the original parent once; later moves before the flag is cleared keep it.
		void markDirty(entt::registry& r, entt::entity e, entt::entity old_parent) {
			if (!r.all_of<CDirtyGraphNode>(e))
				r.emplace<CDirtyGraphNode>(e, old_parent);
		}

		bool isAncestorOrSelf(const entt::registry& r, entt::entity candidate, entt::entity of) {
			for (auto e = of; e != entt::null; e = CGraphNode::parentOf(r, e))
				if (e == candidate) return true;
			return false;
		}
	}

	const CGraphNode& CGraphNode::heirarchy(const entt::registry& r, entt::entity e) {
		return r.get<CGraphNode>(e);
	}

	bool CGraphNode::hasHeirarchy(const entt::registry& r, entt::entity e) {
		return e != entt::null && r.valid(e) && r.all_of<CGraphNode>(e);
	}

	entt::entity CGraphNode::parentOf(const entt::registry& r, entt::entity e) {
		return hasHeirarchy(r, e) ? r.get<CGraphNode>(e).parent : entt::entity{ entt::null };
	}

	entt::entity CGraphNode::firstChild(const entt::registry& r, entt::entity e) {
		return hasHeirarchy(r, e) ? r.get<CGraphNode>(e).first : entt::entity{ entt::null };
	}

	entt::entity CGraphNode::lastChild(const entt::registry& r, entt::entity e) {
		return hasHeirarchy(r, e) ? r.get<CGraphNode>(e).last : entt::entity{ entt::null };
	}

	entt::entity CGraphNode::prevSibling(const entt::registry& r, entt::entity e) {
		return hasHeirarchy(r, e) ? r.get<CGraphNode>(e).prev : entt::entity{ entt::null };
	}

	entt::entity CGraphNode::nextSibling(const entt::registry& r, entt::entity e) {
		return hasHeirarchy(r, e) ? r.get<CGraphNode>(e).next : entt::entity{ entt::null };
	}

	entt::entity CGraphNode::firstSibling(const entt::registry& r, entt::entity e) {
		if (!hasHeirarchy(r, e)) return entt::null;
		auto p = parentOf(r, e);
		if (p == entt::null) return e;  // a root is its own only sibling
		return firstChild(r, p);
	}

	entt::entity CGraphNode::lastSibling(const entt::registry& r, entt::entity e) {
		if (!hasHeirarchy(r, e)) return entt::null;
		auto p = parentOf(r, e);
		if (p == entt::null) return e;
		return lastChild(r, p);
	}

	entt::entity CGraphNode::root(const entt::registry& r, entt::entity e) {
		if (!hasHeirarchy(r, e)) return entt::null;
		while (true) {
			auto p = parentOf(r, e);
			if (p == entt::null) return e;
			e = p;
		}
	}

	entt::entity CGraphNode::newRoot(entt::registry& r) {
		auto e = r.create();
		r.emplace<CGraphNode>(e);
		return e;
	}

	bool CGraphNode::hasChildren(const entt::registry& r, entt::entity e) {
		return firstChild(r, e) != entt::null;
	}

	bool CGraphNode::isLeaf(const entt::registry& r, entt::entity e) {
		return hasHeirarchy(r, e) && !hasChildren(r, e);
	}

	size_t CGraphNode::depth(const entt::registry& r, entt::entity e) {
		size_t d = 0;
		for (auto p = parentOf(r, e); p != entt::null; p = parentOf(r, p)) ++d;
		return d;
	}

	void CGraphNode::attachChild(entt::registry& r, entt::entity target, entt::entity child) {
		if (target == entt::null || child == entt::null || target == child) return;
		if (!r.all_of<CGraphNode>(target)) r.emplace<CGraphNode>(target);
		if (!r.all_of<CGraphNode>(child)) r.emplace<CGraphNode>(child);

		auto& c = r.get<CGraphNode>(child);
		if (c.parent != entt::null) return;                 // must be detached first
		if (isAncestorOrSelf(r, child, target)) return;     // would form a cycle

		auto& p = r.get<CGraphNode>(target);
		c.parent = target;
		c.prev = p.last;
		c.next = entt::null;
		if (p.last != entt::null) r.get<CGraphNode>(p.last).next = child;
		else p.first = child;
		p.last = child;

		markDirty(r, child, entt::null);
	}

	void CGraphNode::detachChild(entt::registry& r, entt::entity target, entt::entity child) {
		if (!hasHeirarchy(r, target) || !hasHeirarchy(r, child)) return;

		auto& c = r.get<CGraphNode>(child);
		if (c.parent != target) return;
		auto& p = r.get<CGraphNode>(target);

		if (c.prev != entt::null) r.get<CGraphNode>(c.prev).next = c.next;
		else p.first = c.next;
		if (c.next != entt::null) r.get<CGraphNode>(c.next).prev = c.prev;
		else p.last = c.prev;

		c.parent = c.prev = c.next = entt::null;
		markDirty(r, child, target);
	}

	void CGraphNode::reparent(entt::registry& r, entt::entity child, entt::entity new_parent) {
		if (!hasHeirarchy(r, child)) return;
		auto old = parentOf(r, child);
		if (old == new_parent) return;
		if (new_parent != entt::null && isAncestorOrSelf(r, child, new_parent)) return;  // cycle

		if (old != entt::null) detachChild(r, old, child);
		if (new_parent != entt::null) attachChild(r, new_parent, child);
		markDirty(r, child, old);  // no-op if already dirtied by the calls above
	}

	entt::entity CGraphNode::addChild(entt::registry& r, entt::entity target) {
		auto child = r.create();
		r.emplace<CGraphNode>(child);
		attachChild(r, target, child);
		return child;
	}

	entt::entity CGraphNode::removeChild(entt::registry& r, entt::entity target, entt::entity child) {
		if (!hasHeirarchy(r, target) || !hasHeirarchy(r, child)) return entt::null;
		if (r.get<CGraphNode>(child).parent != target) return entt::null;

		detachChild(r, target, child);

		// Collect the subtree iteratively (no recursion), then destroy it.
		std::vector<entt::entity> doomed{ child };
		for (size_t i = 0; i < doomed.size(); ++i)
			for (auto c = firstChild(r, doomed[i]); c != entt::null; c = nextSibling(r, c))
				doomed.push_back(c);

		for (auto e : doomed) r.destroy(e);
		return target;
	}

	str CGraphNode::subtree(const entt::registry& r, entt::entity e, const str& prefix, bool is_last) {
		str out = prefix + (is_last ? "`-- " : "|-- ")
			+ std::to_string(static_cast<unsigned long long>(entt::to_integral(e))) + "\n";

		const str child_prefix = prefix + (is_last ? "    " : "|   ");
		for (auto c = firstChild(r, e); c != entt::null; c = nextSibling(r, c))
			out += subtree(r, c, child_prefix, nextSibling(r, c) == entt::null);
		return out;
	}

}