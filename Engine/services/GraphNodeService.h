/// **************************************** QUAKASTER ENGINE **************************************** 
/// services/GraphNodeService.h
/// Purpose: Provides a service to add, remove, modify, and manage scene
/// graphs with the entt ecosystem
/// **************************************************************************************************

#pragma once
#include "../services/IService.h"
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include "../components/CGraphNode.h"
#include "core.h"
using namespace qk::entity;


namespace qk::service {
	/*
	 Provides services for modifying
	 entity heirarchies
	*/
	struct QK_API GraphNodeService : IService {
		entt::registry& registry;

		GraphNodeService(entt::registry& target);

		CGraphNode& heirarchy(entt::entity target);

		bool has_heirarchy(entt::entity target);

		// helper functions
		entt::entity parent(entt::entity target);
		entt::entity first_child(entt::entity target);
		entt::entity last_child(entt::entity target);
		entt::entity first_sibling(entt::entity e);
		entt::entity last_sibling(entt::entity e);
		entt::entity prev_sibling(entt::entity target);
		entt::entity next_sibling(entt::entity target);
		entt::entity add_child(entt::entity parent);
		entt::entity root(entt::entity target);

		entt::entity new_root();
		size_t depth(entt::entity e);
		bool has_children(entt::entity e);
		size_t num_descendants(entt::entity e);
		bool is_leaf(entt::entity e);

		void reparent(entt::entity child, entt::entity new_parent);

		template <class flag_t>
		void clear_descendant_flags(entt::entity target);

		template <class flag_t>
		void invalidate(entt::entity target);

		template <class flag_t>
		void clear();



		template <typename Func>
		void for_each_descendant(entt::entity e, Func&& func);

		template <typename Func>
		void for_each_sibling(entt::entity e, Func&& func);

		bool remove_child(entt::entity target, entt::entity child);

		void detach_child(entt::entity target, entt::entity child);

		void attach_child(entt::entity target, entt::entity new_child);

		void print_subtree(entt::entity target,
			const std::string& prefix = "", bool is_last = true);

		void print_tree(entt::entity root);
	};


	/* -------------------------------- template implementations -------------------------------- */


	template <class flag_t>
	void GraphNodeService::clear_descendant_flags(entt::entity target) {
		if (target == entt::null || first_child(target) == entt::null) return;

		entt::entity current = first_child(target);
		while (current != entt::null) {
			// Erase the flag if it exists
			if (registry.all_of<flag_t>(current))
				registry.erase<flag_t>(current);

			// Recurse into this child's subtree
			clear_descendant_flags<flag_t>(current);

			// Move to next sibling
			current = next_sibling(current);
		}
	}

	template <class flag_t>
	void GraphNodeService::invalidate(entt::entity target) {

		if (target == entt::null) return;

		// no ancestors: 
		if (parent(target) == entt::null) {
			registry.emplace<flag_t>(target, entt::null);
			// clear descendants
			clear_descendant_flags<flag_t>(target);
			return;
		}

		entt::entity marked_ancestor = target;
		entt::entity current = target;
		while (current != entt::null) {
			if (registry.all_of<flag_t>(current)) marked_ancestor = current;
			current = parent(current);
		}

		if (marked_ancestor == target) registry.emplace<flag_t>(target);

		clear_descendant_flags<flag_t>(target);

	}

	template <class flag_t>
	void GraphNodeService::clear() {
		auto view = registry.view<flag_t>();
		registry.erase<flag_t>(view.begin(), view.end()); // <-- semicolon
	}

	template <typename Func>
	void GraphNodeService::for_each_descendant(entt::entity e, Func&& func) {
		for (entt::entity child = first_child(e); child != entt::null; child = next_sibling(child)) {
			func(child);
			for_each_descendant(child, func);
		}
	}

	template <typename Func>
	void GraphNodeService::for_each_sibling(entt::entity e, Func&& func) {
		entt::entity first = first_sibling(e);
		for (entt::entity sibling = first; sibling != entt::null; sibling = next_sibling(sibling)) {
			func(sibling);
		}
	}


}