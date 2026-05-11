#pragma once
namespace qk::util {





    template <class K, class T, order_pfn_t<T> order_fn>
    T* AcceleratedLinkedList<K, T, order_fn>::Node::get() noexcept
    {
        return value.get();
    }
    template <class K, class T, order_pfn_t<T> order_fn>
    T& AcceleratedLinkedList<K, T, order_fn>::Node::operator*() noexcept {
        if (!value) spdlog::critical("AcceleratedLinkedList::Node::operator*() at 0x{:x}: `value` was nullptr", (uintptr_t)this);
        return *value;
    }





    template <class K, class T, order_pfn_t<T> order_fn>
    size_t AcceleratedLinkedList<K, T, order_fn>::size() const
    {
        return m_Nodes.size();
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    size_t AcceleratedLinkedList<K, T, order_fn>::live_size() const
    {
        return m_AliveCount;
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    size_t AcceleratedLinkedList<K, T, order_fn>::capacity() const
    {
        return m_Nodes.capacity();
    }




    template <class K, class T, order_pfn_t<T> order_fn>
    index_t AcceleratedLinkedList<K, T, order_fn>::findInsertionIndex()
    {
        if (m_Tail == NULL_LINK || m_Nodes[m_Tail].next == NULL_LINK) {
            m_Nodes.emplace_back();
            return m_Nodes.size() - 1;
        }
        return m_Nodes[m_Tail].next;
    }


    template <class K, class T, order_pfn_t<T> order_fn>
    index_t AcceleratedLinkedList<K, T, order_fn>::findInsertionNeighbors(const T& item) const
    {
        if constexpr (order_fn == nullptr) return m_Tail;

        index_t prev_candidate = NULL_LINK;
        index_t current = m_Head;


        while (current != NULL_LINK)
        {
            if (order_fn(item, *m_Nodes[current].value)) break;

            prev_candidate = current;
            current = m_Nodes[current].next;
        }


        return prev_candidate;
    }


    // --------------------------------------------------
    // Lookup
    // --------------------------------------------------

    template <class K, class T, order_pfn_t<T> order_fn>
    T* AcceleratedLinkedList<K, T, order_fn>::get(const K& id)
    {
        auto it = m_LUT.find(id);
        if (it == m_LUT.end()) {
            spdlog::warn("AcceleratedLinkedList::get() at 0x{:x}: `value` was nullptr", (uintptr_t)this);
            return nullptr;
        }
        return m_Nodes[it->second].get();
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    T* AcceleratedLinkedList<K, T, order_fn>::operator[](const K& id) {
        return get(id);
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    bool AcceleratedLinkedList<K, T, order_fn>::contains(const K& id) {
        return m_LUT.contains(id);
    }



    // --------------------------------------------------
    // Insert
    // --------------------------------------------------

    template <class K, class T, order_pfn_t<T> order_fn>
    void AcceleratedLinkedList<K, T, order_fn>::insert(const K& id, std::unique_ptr<T>&& obj)
    {

        if (!obj) {
            spdlog::warn("AcceleratedLinkedList::insert() at 0x{:x}: `obj` was nullptr", (uintptr_t)this);
            return;
        }

        if (m_LUT.contains(id)) {
            spdlog::warn("AcceleratedLinkedList::insert() at 0x{:x}: Item `{}` already exists; not inserting.", (uintptr_t)this, id);
            return;
        }


        index_t target = findInsertionIndex();

        m_Nodes[target].value = std::move(obj);

        m_LUT[id] = target;

        index_t prev = findInsertionNeighbors(*m_Nodes[target].value);

        if (prev == NULL_LINK)
        {
            m_Nodes[target].prev = NULL_LINK;
            m_Nodes[target].next = m_Head;

            if (m_Head != NULL_LINK)
                m_Nodes[m_Head].prev = target;

            m_Head = target;
            if (m_Tail == NULL_LINK) m_Tail = target;
        }
        else
        {
            index_t next = m_Nodes[prev].next;
            m_Nodes[target].prev = prev;
            m_Nodes[target].next = next;
            m_Nodes[prev].next = target;

            if (next != NULL_LINK)
                m_Nodes[next].prev = target;
            else
                m_Tail = target;
        }

        ++m_AliveCount;
    }



    template <class K, class T, order_pfn_t<T> order_fn>
    void AcceleratedLinkedList<K, T, order_fn>::remove(const K& id)
    {

        if (!m_LUT.contains(id)) {
            spdlog::warn("AcceleratedLinkedList::remove() at 0x{:x}: Attempted to delete non-existent element of ID `{}`", (uintptr_t)this, id);
            return;
        }



        index_t target = m_LUT[id];
        index_t prev = m_Nodes[target].prev;
        index_t next = m_Nodes[target].next;

        if (prev != NULL_LINK) m_Nodes[prev].next = next;
        if (next != NULL_LINK) m_Nodes[next].prev = prev;

        if (target == m_Head) m_Head = m_Nodes[m_Head].next;
        if (target == m_Tail) m_Tail = m_Nodes[m_Tail].prev;

        m_Nodes[target].value.reset();

        index_t tail_next = NULL_LINK;
        if (m_Tail != NULL_LINK) {
            tail_next = m_Nodes[m_Tail].next;
            m_Nodes[m_Tail].next = target;
        }
        if (tail_next != NULL_LINK) m_Nodes[tail_next].prev = target;

        m_Nodes[target].prev = m_Tail;
        m_Nodes[target].next = tail_next;


        m_LUT.erase(id);
        m_AliveCount--;
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    void AcceleratedLinkedList<K, T, order_fn>::clear()
    {
        m_LUT.clear();
        m_Nodes.clear();
        m_Head = NULL_LINK;
        m_Tail = NULL_LINK;

    }

    template <class K, class T, order_pfn_t<T> order_fn>
    std::unique_ptr<T> AcceleratedLinkedList<K, T, order_fn>::take(const K& id)
    {
        auto it = m_LUT.find(id);
        if (it == m_LUT.end()) {
            spdlog::warn("AcceleratedLinkedList::take() at 0x{:x}: Item `{}` does not exist; returning nullptr.", (uintptr_t)this, id);
            return nullptr;
        }

        index_t target = it->second;
        index_t prev = m_Nodes[target].prev;
        index_t next = m_Nodes[target].next;

        if (prev != NULL_LINK) m_Nodes[prev].next = next;
        if (next != NULL_LINK) m_Nodes[next].prev = prev;

        if (target == m_Head) m_Head = next;
        if (target == m_Tail) m_Tail = prev;

        m_LUT.erase(id);

        --m_AliveCount;

        std::unique_ptr<T> result = std::move(m_Nodes[target].value);

        index_t tail_next = NULL_LINK;
        if (m_Tail != NULL_LINK) {
            tail_next = m_Nodes[m_Tail].next;
            m_Nodes[m_Tail].next = target;
        }
        if (tail_next != NULL_LINK) m_Nodes[tail_next].prev = target;

        m_Nodes[target].prev = m_Tail;
        m_Nodes[target].next = tail_next;

        return result;
    }





    template <class K, class T, order_pfn_t<T> order_fn>
    AcceleratedLinkedList<K, T, order_fn>::iterator::iterator(AcceleratedLinkedList* list, index_t idx) :
        m_List(list),
        m_Index(idx)
    {}

    template <class K, class T, order_pfn_t<T> order_fn>
    typename AcceleratedLinkedList<K, T, order_fn>::iterator::reference
        AcceleratedLinkedList<K, T, order_fn>::iterator::operator*() const
    {
        if (!m_List)
        {
            spdlog::critical(
                "AcceleratedLinkedList::iterator::operator*() at 0x{:x}: "
                "m_List was nullptr",
                (uintptr_t)this
            );
        }

        if (m_Index == NULL_LINK)
        {
            spdlog::critical(
                "AcceleratedLinkedList::iterator::operator*() at 0x{:x}: "
                "attempted to dereference end()/NULL_LINK iterator (list 0x{:x})",
                (uintptr_t)this,
                (uintptr_t)m_List
            );
        }

        auto& node = m_List->m_Nodes[m_Index];

        if (!node.value)
        {
            spdlog::critical(
                "AcceleratedLinkedList::iterator::operator*() at 0x{:x}: "
                "node index {} has nullptr value (list 0x{:x})",
                (uintptr_t)this,
                m_Index,
                (uintptr_t)m_List
            );
        }

        return *node.value;
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    typename AcceleratedLinkedList<K, T, order_fn>::iterator::pointer
        AcceleratedLinkedList<K, T, order_fn>::iterator::operator->() const {

        T* target = &(**this);

        assert(target);

        return target;
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    typename AcceleratedLinkedList<K, T, order_fn>::iterator&
        AcceleratedLinkedList<K, T, order_fn>::iterator::operator++() {
        assert(m_List);
        if (m_Index != NULL_LINK) {
            m_Index = m_List->m_Nodes[m_Index].next;
        }


        return *this;
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    typename AcceleratedLinkedList<K, T, order_fn>::iterator
        AcceleratedLinkedList<K, T, order_fn>::iterator::operator++(int) {
        iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    bool AcceleratedLinkedList<K, T, order_fn>::iterator::operator==(
        const iterator& other
        ) const {
        return m_List == other.m_List && m_Index == other.m_Index;
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    bool AcceleratedLinkedList<K, T, order_fn>::iterator::operator!=(
        const iterator& other
        ) const {
        return !(*this == other);
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    typename AcceleratedLinkedList<K, T, order_fn>::iterator AcceleratedLinkedList<K, T, order_fn>::begin() {
        return iterator(this, m_Head);
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    typename AcceleratedLinkedList<K, T, order_fn>::iterator AcceleratedLinkedList<K, T, order_fn>::end() {
        return iterator(this, NULL_LINK); // was NULL_LINK
    }



    template <class K, class T, order_pfn_t<T> order_fn>
    AcceleratedLinkedList<K, T, order_fn>::reverse_iterator::reverse_iterator(AcceleratedLinkedList* list, index_t idx)
        : m_List(list), m_Index(idx) {}


    template <class K, class T, order_pfn_t<T> order_fn>
    T& AcceleratedLinkedList<K, T, order_fn>::reverse_iterator::operator*() const {
        assert(m_List && m_Index != NULL_LINK);
        T* ptr = m_List->m_Nodes[m_Index].value.get();
        assert(ptr != nullptr);
        return *ptr;
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    T* AcceleratedLinkedList<K, T, order_fn>::reverse_iterator::operator->() const {
        return m_List->m_Nodes[m_Index].value.get();
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    typename AcceleratedLinkedList<K, T, order_fn>::reverse_iterator&
        AcceleratedLinkedList<K, T, order_fn>::reverse_iterator::operator++() {
        m_Index = m_List->m_Nodes[m_Index].prev;
        return *this;
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    typename AcceleratedLinkedList<K, T, order_fn>::reverse_iterator
        AcceleratedLinkedList<K, T, order_fn>::reverse_iterator::operator++(int) {
        reverse_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    bool AcceleratedLinkedList<K, T, order_fn>::reverse_iterator::operator==(const reverse_iterator& other) const {
        return m_List == other.m_List && m_Index == other.m_Index;
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    bool AcceleratedLinkedList<K, T, order_fn>::reverse_iterator::operator!=(const reverse_iterator& other) const {
        return !(*this == other);
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    typename AcceleratedLinkedList<K, T, order_fn>::reverse_iterator
        AcceleratedLinkedList<K, T, order_fn>::rbegin() {
        return reverse_iterator(this, m_Tail);
    }

    template <class K, class T, order_pfn_t<T> order_fn>
    typename AcceleratedLinkedList<K, T, order_fn>::reverse_iterator
        AcceleratedLinkedList<K, T, order_fn>::rend() {
        return reverse_iterator(this, NULL_LINK);
    }




} // namespace qk::util
