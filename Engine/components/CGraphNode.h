/// **************************************** QUAKASTER ENGINE ****************************************
/// components/CGraphNode.h
/// Purpose: Defines a scene graph node type, within the entt ecosystem
/// **************************************************************************************************
#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/fwd.hpp>
#include <core.h>

namespace qk {

	// Marks a node whose place in the tree changed. old_parent is the parent the node had
	// when it was FIRST dirtied (not overwritten by later moves before the flag is cleared).
	struct CDirtyGraphNode {
		entt::entity old_parent = entt::null;
	};

	// Intrusive doubly linked tree node. Children of a node form a list via first/last,
	// siblings are linked via prev/next. All structural changes go through the functions
	// below so the links stay consistent.
	struct CGraphNode {

		entt::entity parent = entt::null;
		entt::entity prev = entt::null;
		entt::entity next = entt::null;
		entt::entity first = entt::null;
		entt::entity last = entt::null;

		// NOTE: 'parent' is a data member, so the accessor is named parentOf.

		// Precondition: target has a CGraphNode.
		static const CGraphNode& heirarchy(const entt::registry&, entt::entity);

		static bool hasHeirarchy(const entt::registry&, entt::entity);

		static entt::entity root(const entt::registry&, entt::entity);

		static entt::entity newRoot(entt::registry&);

		static bool hasChildren(const entt::registry&, entt::entity);

		static bool isLeaf(const entt::registry&, entt::entity);

		// Number of ancestors (a root has depth 0). O(depth).
		static size_t depth(const entt::registry&, entt::entity);

		// Moves child under new_parent (entt::null makes it a root). Ignored if it would create
		// a cycle. Marks child dirty.
		static void reparent(entt::registry&, entt::entity child, entt::entity new_parent);

		static entt::entity parentOf(const entt::registry&, entt::entity);

		static entt::entity firstChild(const entt::registry&, entt::entity);

		static entt::entity lastChild(const entt::registry&, entt::entity);

		static entt::entity firstSibling(const entt::registry&, entt::entity);

		static entt::entity lastSibling(const entt::registry&, entt::entity);

		static entt::entity prevSibling(const entt::registry&, entt::entity);

		static entt::entity nextSibling(const entt::registry&, entt::entity);

		// Creates a new entity as the last child of the given parent.
		static entt::entity addChild(entt::registry&, entt::entity);

		// Detaches child from target and destroys it and all its descendants.
		// Returns target on success, entt::null if child is not a child of target.
		static entt::entity removeChild(entt::registry&, entt::entity target, entt::entity child);

		// Unlinks child from target without destroying it. Child becomes a root. Marks it dirty.
		static void detachChild(entt::registry&, entt::entity target, entt::entity child);

		// Appends child as last child of target. Child must currently have no parent.
		static void attachChild(entt::registry&, entt::entity target, entt::entity child);

		static str subtree(const entt::registry&, entt::entity, const str& prefix = "", bool is_last = true);

		// Pre - order walk of everything below e(e itself is not visited).Iterative, so deep trees
		// can't overflow the stack. The callback must not change the tree's structure.
		template <typename Func>
		static void forEachDescendant(const entt::registry& r, entt::entity e, Func&& func);

		// Visits e and all its siblings in order (a root is its own only sibling).
		template <typename Func>
		static void forEachSibling(const entt::registry& r, entt::entity e, Func&& func);

		// Removes Flag from every descendant of target (not target itself).
		template <class Flag>
		static void clearDescendantFlags(entt::registry& r, entt::entity target);

		// Lazy dirty propagation: keeps the invariant "no flagged node has a flagged ancestor".
		//  - If a strict ancestor is already flagged, it covers target, so target's own flag and all
		//    descendant flags are dropped.
		//  - Otherwise target gets the flag (constructed from args if not already present, so an
		//    existing flag keeps its data) and descendant flags are dropped.
		template <class Flag, class... Args>
		static void invalidate(entt::registry& r, entt::entity target, Args&&... args);

		// Removes Flag from every entity that has it.
		template <class Flag>
		static void clear(entt::registry& r);


	};

	

}

#include "CGraphNode.inl"