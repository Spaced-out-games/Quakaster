#pragma once
#include <unordered_map>
#include <stdint.h>
#include <vector>
#include <memory>
#include <string>
#include <limits>
#include <cassert>
#include "spdlog/spdlog.h"

// NOTICE: Fixes the fact that the compiler is expanding numeric_limits::max() to minwindef's max() macro instead of
// the constexpr. DO NOT REMOVE!
#undef max



namespace qk::util {

    // Describes a raw index in the vector
    using index_t = uint32_t;
    // Describes a pointer to another node, at least conceptually. 
    using node_ptr_t = index_t;


    constexpr index_t NULL_LINK = std::numeric_limits<index_t>::max();
    using splice_t = std::pair<index_t, index_t>;
    template <class T>
    using order_pfn_t = bool(*)(const T&, const T&);







    /// <summary>
    /// Accelerated linked list data structure. Conceptually, it's a linked list with a human-readable
    /// interface to access and mutate Ts with ease. There is no need to manage memory by the end user,
    /// reshuffle items when resizing, or update hash map keys.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <class K, class T, order_pfn_t<T> order_fn = nullptr>
    struct AcceleratedLinkedList
    {

        struct iterator;
        struct reverse_iterator;
        iterator begin();
        iterator end();
        reverse_iterator rbegin();
        reverse_iterator rend();
        /// <summary>
        /// Linked list node
        /// </summary>
        struct Node
        {
            std::unique_ptr<T> value = nullptr;
            index_t prev = NULL_LINK;
            index_t next = NULL_LINK;

            T* get() noexcept;
            T& operator*() noexcept;
        };

        /// The pool of nodes
        std::vector<Node> m_Nodes;

        // Lookup table
        std::unordered_map<K, index_t> m_LUT;

        // Head of the linked list
        index_t m_Head = NULL_LINK;

        // Tail of the linked list
        index_t m_Tail = NULL_LINK;

        // Number of nodes actively in use
        size_t m_AliveCount = 0;

        // Number of alive and dead nodes in the linked list
        size_t size() const;
        // Number of alive nodes in the linked list
        size_t live_size() const;

        // Gets the number of slots allocated before a resize is needed
        size_t capacity() const;

        // Finds where in the vector to insert another node.
        index_t findInsertionIndex();

        // Finds where in the linked list to insert in front of
        index_t findInsertionNeighbors(const T& item) const;

        // Gets a node by ID
        T* get(const K& id);

        bool contains(const K& id);

        // Inserts a node by ID
        void insert(const K& id, std::unique_ptr<T>&& obj);

        // Removes a node by ID
        void remove(const K& id);

        // Clears the linked list
        void clear();

        std::unique_ptr<T> take(const K& id);

        // Array access operator, by ID
        T* operator[](const K& id);

        class iterator {
            AcceleratedLinkedList* m_List = nullptr;
            index_t                 m_Index = NULL_LINK;
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            iterator(AcceleratedLinkedList* list, index_t idx);

            reference operator*() const;

            pointer operator->() const;

            iterator& operator++();

            iterator operator++(int);

            bool operator==(const iterator& other) const;

            bool operator!=(const iterator& other) const;


        };

        struct reverse_iterator {
            AcceleratedLinkedList* m_List;
            index_t m_Index;

            reverse_iterator(AcceleratedLinkedList* list, index_t idx);

            T& operator*() const;
            T* operator->() const;

            reverse_iterator& operator++();       // pre-increment
            reverse_iterator operator++(int);     // post-increment

            bool operator==(const reverse_iterator& other) const;
            bool operator!=(const reverse_iterator& other) const;
        };



    };

} // namespace qk::util

#include "AcceleratedLinkedList.inl"
