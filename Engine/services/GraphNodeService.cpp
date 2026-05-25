#include "GraphNodeService.h"
#include <string>
#include <spdlog/spdlog.h>


namespace qk::service {

	GraphNodeService::GraphNodeService(entt::registry& target) : m_Registry(target) {}


	CGraphNode& GraphNodeService::heirarchy(entt::entity target) {
		return m_Registry.get<CGraphNode>(target);
	}
	bool GraphNodeService::hasHeirarchy(entt::entity target) {
		return m_Registry.all_of<CGraphNode>(target);

	}

	entt::entity GraphNodeService::root(entt::entity target) {
		if (target == entt::null || !hasHeirarchy(target)) return entt::null;

		entt::entity current = target;
		while (parent(current) != entt::null) {
			current = parent(current);
		}

		return current;
	}

	entt::entity GraphNodeService::newRoot() {
		entt::entity result = m_Registry.create();
		m_Registry.emplace<CGraphNode>(result);
		return result;
	}

	bool GraphNodeService::hasChildren(entt::entity e) {
		return firstChild(e) != entt::null;
	}

	bool GraphNodeService::isLeaf(entt::entity e) {
		return !hasChildren(e);
	}

	size_t GraphNodeService::depth(entt::entity e) {
		size_t d = 0;
		while (parent(e) != entt::null) {
			e = parent(e);
			++d;
		}
		return d;
	}

	void GraphNodeService::reparent(entt::entity child, entt::entity new_parent) {
		detachChild(parent(child), child);
		attachChild(new_parent, child);
	}

	size_t GraphNodeService::numDescendants(entt::entity e) {
		size_t count = 1;
		forEachDescendant(e, [&](entt::entity) { ++count; });
		return count;
	}

	entt::entity GraphNodeService::parent(entt::entity target) {
		if (target == entt::null || !hasHeirarchy(target)) return entt::null;

		return heirarchy(target).parent;
	}

	entt::entity GraphNodeService::firstChild(entt::entity target) {
		if (target == entt::null || !hasHeirarchy(target)) return entt::null;

		return heirarchy(target).first;
	}

	entt::entity GraphNodeService::lastChild(entt::entity target) {
		if (target == entt::null || !hasHeirarchy(target)) return entt::null;

		return heirarchy(target).last;
	}

	entt::entity GraphNodeService::firstSibling(entt::entity e) {
		if (e == entt::null || !hasHeirarchy(e)) return entt::null;
		auto parent = heirarchy(e).parent;
		if (parent == entt::null || !hasHeirarchy(parent)) return e;
		return heirarchy(parent).first;
	}

	entt::entity GraphNodeService::lastSibling(entt::entity e) {
		if (e == entt::null || !hasHeirarchy(e)) return entt::null;
		auto parent = heirarchy(e).parent;
		if (parent == entt::null || !hasHeirarchy(parent)) return e;
		return heirarchy(parent).last;
	}

	entt::entity GraphNodeService::prevSibling(entt::entity e) {
		if (e == entt::null || !hasHeirarchy(e)) return entt::null;
		return heirarchy(e).prev;
	}

	entt::entity GraphNodeService::nextSibling(entt::entity e) {
		if (e == entt::null || !hasHeirarchy(e)) return entt::null;
		return heirarchy(e).next;
	}


	entt::entity GraphNodeService::addChild(entt::entity parent) {
		// Let's not insert into empty nodes
		if (parent == entt::null) return entt::null;

		// Let's also not allocate a CGraphNode for the parent
		if (!hasHeirarchy(parent)) return entt::null;

		// we have implicit permission to do this.
		entt::entity child = m_Registry.create();
		m_Registry.emplace<CGraphNode>(child);

		// first insertion check
		if (firstChild(parent) == entt::null) {

			heirarchy(parent).first = child;
			heirarchy(parent).last = child;
			heirarchy(child).parent = parent;
			return child;
		}
		// If we design it all properly, these should never be true
		assert(lastChild(parent) != entt::null);
		assert(hasHeirarchy(lastChild(parent)));


		entt::entity last = lastChild(parent);
		heirarchy(last).next = child;
		heirarchy(child).prev = last;
		heirarchy(parent).last = child;
		return child;

	}

	bool GraphNodeService::removeChild(entt::entity target, entt::entity child) {
		if (target == entt::null || child == entt::null) return true;
		if (parent(child) != target) return false;

		entt::entity first = firstChild(child);
		entt::entity last = lastChild(child);

		// since we have the linked list now, we can delete `child`
		m_Registry.destroy(child);


		entt::entity current = first;
		// now recursively delete children
		while (current != entt::null) {
			removeChild(current, firstChild(current));
			current = nextSibling(current);
		}

		return true;

	}
	void GraphNodeService::detachChild(entt::entity target, entt::entity child) {
		if (target == entt::null || child == entt::null) return;

		if (!hasHeirarchy(target) || !hasHeirarchy(child)) return;

		// parent links
		if (child == firstChild(target)) heirarchy(target).first = nextSibling(child);
		if (child == lastChild(target)) heirarchy(target).last = prevSibling(child);

		// sister links
		entt::entity prev = prevSibling(child);
		entt::entity next = nextSibling(child);
		if (prev != entt::null) heirarchy(prev).next = next;
		if (next != entt::null) heirarchy(next).prev = prev;

		// just place at root by default
		heirarchy(child).parent = entt::null;

		// also mark it dirty
		invalidate<CGraphNode::DirtyFlag>(child);


	}

	void GraphNodeService::attachChild(entt::entity target, entt::entity newChild) {
		if (target == entt::null || newChild == entt::null) return;

		// Prevent cycles: can't attach a parent under its own descendant
		if (root(target) == newChild) return;

		// Detach from old parent if any
		entt::entity old_parent = parent(newChild);
		if (old_parent != entt::null) {
			detachChild(old_parent, newChild);
		}

		auto& parentComp = heirarchy(target);
		auto& childComp = heirarchy(newChild);

		// Insert at end of parent's child list
		childComp.parent = target;
		childComp.prev = parentComp.last;
		childComp.next = entt::null;

		if (parentComp.last != entt::null)
			heirarchy(parentComp.last).next = newChild;

		parentComp.last = newChild;

		if (parentComp.first == entt::null)
			parentComp.first = newChild;

		// Optionally propagate dirty flag
		invalidate<CGraphNode::DirtyFlag>(newChild);
	}

	void GraphNodeService::printSubtree(entt::entity target, const str& prefix, bool is_last) {
		if (target == entt::null || !m_Registry.all_of<CGraphNode>(target)) return;

		// Build the line
		str line = prefix + (is_last ? "\\ " : "T ") + std::to_string(int(target));

		// Log the line via spdlog raw logger (no timestamp/level)
		spdlog::default_logger_raw()->info(line);

		auto& node = m_Registry.get<CGraphNode>(target);

		// Gather children right-to-left
		std::vector<entt::entity> children;
		for (entt::entity child = node.last; child != entt::null; child = prevSibling(child)) {
			children.push_back(child);
		}

		// Recurse into children
		for (size_t i = 0; i < children.size(); ++i) {
			bool child_is_last = (i == children.size() - 1);
			printSubtree(children[i], prefix + (is_last ? "    " : "|   "), child_is_last);
		}
	}

	void GraphNodeService::printTree(entt::entity root) {
		printSubtree(root);
	}


}