#include "GraphNodeService.h"
#include <string>
#include <spdlog/spdlog.h>


namespace qk::service {

	GraphNodeService::GraphNodeService(entt::registry& target) : registry(target) {}


	CGraphNode& GraphNodeService::heirarchy(entt::entity target) {
		return registry.get<CGraphNode>(target);
	}
	bool GraphNodeService::has_heirarchy(entt::entity target) {
		return registry.all_of<CGraphNode>(target);

	}

	entt::entity GraphNodeService::root(entt::entity target) {
		if (target == entt::null || !has_heirarchy(target)) return entt::null;

		entt::entity current = target;
		while (parent(current) != entt::null) {
			current = parent(current);
		}

		return current;
	}

	entt::entity GraphNodeService::new_root() {
		entt::entity result = registry.create();
		registry.emplace<CGraphNode>(result);
		return result;
	}

	bool GraphNodeService::has_children(entt::entity e) {
		return first_child(e) != entt::null;
	}

	bool GraphNodeService::is_leaf(entt::entity e) {
		return !has_children(e);
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
		detach_child(parent(child), child);
		attach_child(new_parent, child);
	}

	size_t GraphNodeService::num_descendants(entt::entity e) {
		size_t count = 1;
		for_each_descendant(e, [&](entt::entity) { ++count; });
		return count;
	}

	entt::entity GraphNodeService::parent(entt::entity target) {
		if (target == entt::null || !has_heirarchy(target)) return entt::null;

		return heirarchy(target).parent;
	}

	entt::entity GraphNodeService::first_child(entt::entity target) {
		if (target == entt::null || !has_heirarchy(target)) return entt::null;

		return heirarchy(target).first;
	}

	entt::entity GraphNodeService::last_child(entt::entity target) {
		if (target == entt::null || !has_heirarchy(target)) return entt::null;

		return heirarchy(target).last;
	}

	entt::entity GraphNodeService::first_sibling(entt::entity e) {
		if (e == entt::null || !has_heirarchy(e)) return entt::null;
		auto parent = heirarchy(e).parent;
		if (parent == entt::null || !has_heirarchy(parent)) return e;
		return heirarchy(parent).first;
	}

	entt::entity GraphNodeService::last_sibling(entt::entity e) {
		if (e == entt::null || !has_heirarchy(e)) return entt::null;
		auto parent = heirarchy(e).parent;
		if (parent == entt::null || !has_heirarchy(parent)) return e;
		return heirarchy(parent).last;
	}

	entt::entity GraphNodeService::prev_sibling(entt::entity e) {
		if (e == entt::null || !has_heirarchy(e)) return entt::null;
		return heirarchy(e).prev;
	}

	entt::entity GraphNodeService::next_sibling(entt::entity e) {
		if (e == entt::null || !has_heirarchy(e)) return entt::null;
		return heirarchy(e).next;
	}


	entt::entity GraphNodeService::add_child(entt::entity parent) {
		// Let's not insert into empty nodes
		if (parent == entt::null) return entt::null;

		// Let's also not allocate a CGraphNode for the parent
		if (!has_heirarchy(parent)) return entt::null;

		// we have implicit permission to do this.
		entt::entity child = registry.create();
		registry.emplace<CGraphNode>(child);

		// first insertion check
		if (first_child(parent) == entt::null) {

			heirarchy(parent).first = child;
			heirarchy(parent).last = child;
			heirarchy(child).parent = parent;
			return child;
		}
		// If we design it all properly, these should never be true
		assert(last_child(parent) != entt::null);
		assert(has_heirarchy(last_child(parent)));


		entt::entity last = last_child(parent);
		heirarchy(last).next = child;
		heirarchy(child).prev = last;
		heirarchy(parent).last = child;
		return child;

	}

	bool GraphNodeService::remove_child(entt::entity target, entt::entity child) {
		if (target == entt::null || child == entt::null) return true;
		if (parent(child) != target) return false;

		entt::entity first = first_child(child);
		entt::entity last = last_child(child);

		// since we have the linked list now, we can delete `child`
		registry.destroy(child);


		entt::entity current = first;
		// now recursively delete children
		while (current != entt::null) {
			remove_child(current, first_child(current));
			current = next_sibling(current);
		}

		return true;

	}
	void GraphNodeService::detach_child(entt::entity target, entt::entity child) {
		if (target == entt::null || child == entt::null) return;

		if (!has_heirarchy(target) || !has_heirarchy(child)) return;

		// parent links
		if (child == first_child(target)) heirarchy(target).first = next_sibling(child);
		if (child == last_child(target)) heirarchy(target).last = prev_sibling(child);

		// sister links
		entt::entity prev = prev_sibling(child);
		entt::entity next = next_sibling(child);
		if (prev != entt::null) heirarchy(prev).next = next;
		if (next != entt::null) heirarchy(next).prev = prev;

		// just place at root by default
		heirarchy(child).parent = entt::null;

		// also mark it dirty
		invalidate<CGraphNode::DirtyFlag>(child);


	}

	void GraphNodeService::attach_child(entt::entity target, entt::entity new_child) {
		if (target == entt::null || new_child == entt::null) return;

		// Prevent cycles: can't attach a parent under its own descendant
		if (root(target) == new_child) return;

		// Detach from old parent if any
		entt::entity old_parent = parent(new_child);
		if (old_parent != entt::null) {
			detach_child(old_parent, new_child);
		}

		auto& parentComp = heirarchy(target);
		auto& childComp = heirarchy(new_child);

		// Insert at end of parent's child list
		childComp.parent = target;
		childComp.prev = parentComp.last;
		childComp.next = entt::null;

		if (parentComp.last != entt::null)
			heirarchy(parentComp.last).next = new_child;

		parentComp.last = new_child;

		if (parentComp.first == entt::null)
			parentComp.first = new_child;

		// Optionally propagate dirty flag
		invalidate<CGraphNode::DirtyFlag>(new_child);
	}

	void GraphNodeService::print_subtree(entt::entity target, const std::string& prefix, bool is_last) {
		if (target == entt::null || !registry.all_of<CGraphNode>(target)) return;

		// Build the line
		std::string line = prefix + (is_last ? "\\ " : "T ") + std::to_string(int(target));

		// Log the line via spdlog raw logger (no timestamp/level)
		spdlog::default_logger_raw()->info(line);

		auto& node = registry.get<CGraphNode>(target);

		// Gather children right-to-left
		std::vector<entt::entity> children;
		for (entt::entity child = node.last; child != entt::null; child = prev_sibling(child)) {
			children.push_back(child);
		}

		// Recurse into children
		for (size_t i = 0; i < children.size(); ++i) {
			bool child_is_last = (i == children.size() - 1);
			print_subtree(children[i], prefix + (is_last ? "    " : "|   "), child_is_last);
		}
	}

	void GraphNodeService::print_tree(entt::entity root) {
		print_subtree(root);
	}


}