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
		entt::registry& m_Registry;

		GraphNodeService(entt::registry& target);

		CGraphNode& heirarchy(entt::entity target);

		bool hasHeirarchy(entt::entity target);

		// helper functions
		entt::entity parent(entt::entity target);
		entt::entity firstChild(entt::entity target);
		entt::entity lastChild(entt::entity target);
		entt::entity firstSibling(entt::entity e);
		entt::entity lastSibling(entt::entity e);
		entt::entity prevSibling(entt::entity target);
		entt::entity nextSibling(entt::entity target);
		entt::entity addChild(entt::entity parent);
		entt::entity root(entt::entity target);

		entt::entity newRoot();
		size_t depth(entt::entity e);
		bool hasChildren(entt::entity e);
		size_t numDescendants(entt::entity e);
		bool isLeaf(entt::entity e);

		void reparent(entt::entity child, entt::entity new_parent);

		template <class flag_t>
		void clearDescendantFlags(entt::entity target);

		template <class flag_t>
		void invalidate(entt::entity target);

		template <class flag_t>
		void clear();



		template <typename Func>
		void forEachDescendant(entt::entity e, Func&& func);

		template <typename Func>
		void forEachSibling(entt::entity e, Func&& func);

		bool removeChild(entt::entity target, entt::entity child);

		void detachChild(entt::entity target, entt::entity child);

		void attachChild(entt::entity target, entt::entity new_child);

		void printSubtree(entt::entity target,
			const str& prefix = "", bool is_last = true);

		void printTree(entt::entity root);
	};


	/* -------------------------------- template implementations -------------------------------- */


	template <class flag_t>
	void GraphNodeService::clearDescendantFlags(entt::entity target) {
		if (target == entt::null || firstChild(target) == entt::null) return;

		entt::entity current = firstChild(target);
		while (current != entt::null) {
			// Erase the flag if it exists
			if (m_Registry.all_of<flag_t>(current))
				m_Registry.erase<flag_t>(current);

			// Recurse into this child's subtree
			clearDescendantFlags<flag_t>(current);

			// Move to next sibling
			current = nextSibling(current);
		}
	}

	template <class flag_t>
	void GraphNodeService::invalidate(entt::entity target) {

		if (target == entt::null) return;

		// no ancestors: 
		if (parent(target) == entt::null) {
			m_Registry.emplace<flag_t>(target, entt::null);
			// clear descendants
			clearDescendantFlags<flag_t>(target);
			return;
		}

		entt::entity marked_ancestor = target;
		entt::entity current = target;
		while (current != entt::null) {
			if (m_Registry.all_of<flag_t>(current)) marked_ancestor = current;
			current = parent(current);
		}

		if (marked_ancestor == target) m_Registry.emplace<flag_t>(target);

		clearDescendantFlags<flag_t>(target);

	}

	template <class flag_t>
	void GraphNodeService::clear() {
		auto view = m_Registry.view<flag_t>();
		m_Registry.erase<flag_t>(view.begin(), view.end()); // <-- semicolon
	}

	template <typename Func>
	void GraphNodeService::forEachDescendant(entt::entity e, Func&& func) {
		for (entt::entity child = firstChild(e); child != entt::null; child = nextSibling(child)) {
			func(child);
			forEachDescendant(child, func);
		}
	}

	template <typename Func>
	void GraphNodeService::forEachSibling(entt::entity e, Func&& func) {
		entt::entity first = firstSibling(e);
		for (entt::entity sibling = first; sibling != entt::null; sibling = nextSibling(sibling)) {
			func(sibling);
		}
	}


}