// Template and inline definitions for qk::KeyValue / qk::TKeyValues.
// Include this file from KeyValues.h; do not compile it separately.

namespace qk {

    // ------------------------------------------------------------
    // KeyValue
    // ------------------------------------------------------------

    template <class T>
    std::optional<T> KeyValue::as() const {
        if (const auto* value = std::get_if<T>(static_cast<const Base*>(this)))
            return *value;

        return std::nullopt;
    }


    template <class T>
    bool KeyValue::is() const {
        return std::holds_alternative<T>(*this);
    }

    // ------------------------------------------------------------
    // TKeyValues - Create
    // ------------------------------------------------------------

    template <class Key_t>
    void TKeyValues<Key_t>::insert(
        const Key_t& key,
        const ValueVariant_t& value
    ) {
        assert(!contains(key));

        // A duplicate must not leave an unindexed value in m_Values,
        // including in builds where assertions are disabled.
        if (contains(key))
            return;

        const size_type index = m_Values.size();

        m_Values.push_back(value);

        try {
            m_ValueLookup.insert({ key, index });
        }
        catch (...) {
            m_Values.pop_back();
            throw;
        }
    }


    template <class Key_t>
    void TKeyValues<Key_t>::insert(const ValueVariant_t& value) {
        m_Values.push_back(value);
    }





    // ------------------------------------------------------------
    // TKeyValues - Read / update
    // ------------------------------------------------------------

    template <class Key_t>
    typename TKeyValues<Key_t>::ValueVariant_t&
        TKeyValues<Key_t>::operator[](size_type index) {
        assert(index < m_Values.size());
        return m_Values[index];
    }


    template <class Key_t>
    const typename TKeyValues<Key_t>::ValueVariant_t&
        TKeyValues<Key_t>::operator[](size_type index) const {
        assert(index < m_Values.size());
        return m_Values[index];
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::ValueVariant_t&
        TKeyValues<Key_t>::operator[](const Key_t& key) {
        auto it = m_ValueLookup.find(key);
        assert(it != m_ValueLookup.end());

        return m_Values[it->second];
    }


    template <class Key_t>
    const typename TKeyValues<Key_t>::ValueVariant_t&
        TKeyValues<Key_t>::operator[](const Key_t& key) const {
        auto it = m_ValueLookup.find(key);
        assert(it != m_ValueLookup.end());

        return m_Values[it->second];
    }


    // ------------------------------------------------------------
    // TKeyValues - Delete
    // ------------------------------------------------------------

    template <class Key_t>
    void TKeyValues<Key_t>::remove(const Key_t& key) {
        auto it = m_ValueLookup.find(key);
        assert(it != m_ValueLookup.end());

        if (it == m_ValueLookup.end())
            return;

        remove(it->second);
    }


    template <class Key_t>
    void TKeyValues<Key_t>::remove(size_type index) {
        assert(index < m_Values.size());

        if (index >= m_Values.size())
            return;

        // Erase the name associated with this value and repair all
        // indices referring to values after the erased position.
        for (auto it = m_ValueLookup.begin(); it != m_ValueLookup.end();) {
            if (it->second == index) {
                it = m_ValueLookup.erase(it);
            }
            else {
                if (it->second > index)
                    --it->second;

                ++it;
            }
        }

        m_Values.erase(m_Values.begin() + static_cast<std::ptrdiff_t>(index));
    }


    // ------------------------------------------------------------
    // TKeyValues - Queries
    // ------------------------------------------------------------

    template <class Key_t>
    bool TKeyValues<Key_t>::contains(const Key_t& key) const {
        return m_ValueLookup.find(key) != m_ValueLookup.end();
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::size_type
        TKeyValues<Key_t>::size() const noexcept {
        return m_Values.size();
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::size_type
        TKeyValues<Key_t>::named_size() const noexcept {
        return m_ValueLookup.size();
    }


    template <class Key_t>
    bool TKeyValues<Key_t>::empty() const noexcept {
        return m_Values.empty();
    }


    template <class Key_t>
    void TKeyValues<Key_t>::clear() noexcept {
        m_Values.clear();
        m_ValueLookup.clear();
    }


    // ------------------------------------------------------------
    // TKeyValues - Positional iteration
    // ------------------------------------------------------------

    template <class Key_t>
    typename TKeyValues<Key_t>::iterator
        TKeyValues<Key_t>::begin() noexcept {
        return m_Values.begin();
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::iterator
        TKeyValues<Key_t>::end() noexcept {
        return m_Values.end();
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::const_iterator
        TKeyValues<Key_t>::begin() const noexcept {
        return m_Values.begin();
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::const_iterator
        TKeyValues<Key_t>::end() const noexcept {
        return m_Values.end();
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::const_iterator
        TKeyValues<Key_t>::cbegin() const noexcept {
        return m_Values.cbegin();
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::const_iterator
        TKeyValues<Key_t>::cend() const noexcept {
        return m_Values.cend();
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::reverse_iterator
        TKeyValues<Key_t>::rbegin() noexcept {
        return m_Values.rbegin();
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::reverse_iterator
        TKeyValues<Key_t>::rend() noexcept {
        return m_Values.rend();
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::const_reverse_iterator
        TKeyValues<Key_t>::rbegin() const noexcept {
        return m_Values.rbegin();
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::const_reverse_iterator
        TKeyValues<Key_t>::rend() const noexcept {
        return m_Values.rend();
    }


    // ------------------------------------------------------------
    // TKeyValues - ValueIterator
    // ------------------------------------------------------------

    template <class Key_t>
    typename TKeyValues<Key_t>::ValueIterator::reference
        TKeyValues<Key_t>::ValueIterator::operator*() const {
        return *m_Iterator;
    }




    template <class Key_t>
    typename TKeyValues<Key_t>::ValueIterator&
        TKeyValues<Key_t>::ValueIterator::operator++() {
        ++m_Iterator;
        return *this;
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::ValueIterator
        TKeyValues<Key_t>::ValueIterator::operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::ValueIterator&
        TKeyValues<Key_t>::ValueIterator::operator--() {
        --m_Iterator;
        return *this;
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::ValueIterator
        TKeyValues<Key_t>::ValueIterator::operator--(int) {
        auto copy = *this;
        --(*this);
        return copy;
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::ValueIterator&
        TKeyValues<Key_t>::ValueIterator::operator+=(difference_type n) {
        m_Iterator += n;
        return *this;
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::ValueIterator&
        TKeyValues<Key_t>::ValueIterator::operator-=(difference_type n) {
        m_Iterator -= n;
        return *this;
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::ValueIterator
        TKeyValues<Key_t>::ValueIterator::operator+(difference_type n) const {
        return { m_Iterator + n };
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::ValueIterator
        TKeyValues<Key_t>::ValueIterator::operator-(difference_type n) const {
        return { m_Iterator - n };
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::ValueIterator::difference_type
        TKeyValues<Key_t>::ValueIterator::operator-(
            const ValueIterator& other
            ) const {
        return m_Iterator - other.m_Iterator;
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::ValueIterator::reference
        TKeyValues<Key_t>::ValueIterator::operator[](difference_type n) const {
        return m_Iterator[n];
    }


    // ------------------------------------------------------------
    // TKeyValues - KeyIterator
    // ------------------------------------------------------------

    template <class Key_t>
    typename TKeyValues<Key_t>::KeyValueEntry
        TKeyValues<Key_t>::KeyIterator::operator*() const {
        return {
            m_Iterator->first,
            m_Owner->m_Values[m_Iterator->second]
        };
    }






    template <class Key_t>
    typename TKeyValues<Key_t>::KeyIterator&
        TKeyValues<Key_t>::KeyIterator::operator++() {
        ++m_Iterator;
        return *this;
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::KeyIterator
        TKeyValues<Key_t>::KeyIterator::operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }


    template <class Key_t>
    bool TKeyValues<Key_t>::KeyIterator::operator==(
        const KeyIterator& other
        ) const {
        if (m_Owner != other.m_Owner)
            return false;

        return m_Iterator == other.m_Iterator;
    }


    // ------------------------------------------------------------
    // TKeyValues - Iterator access
    // ------------------------------------------------------------

    template <class Key_t>
    typename TKeyValues<Key_t>::ValueIterator
        TKeyValues<Key_t>::value_begin() noexcept {
        return { m_Values.begin() };
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::ValueIterator
        TKeyValues<Key_t>::value_end() noexcept {
        return { m_Values.end() };
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::KeyIterator
        TKeyValues<Key_t>::key_begin() noexcept {
        return { this, m_ValueLookup.begin() };
    }


    template <class Key_t>
    typename TKeyValues<Key_t>::KeyIterator
        TKeyValues<Key_t>::key_end() noexcept {
        return { this, m_ValueLookup.end() };
    }


    // ------------------------------------------------------------
    // JSON parser
    // ------------------------------------------------------------

    template <class Key_t>
    std::expected<void, KeyValuesParseError>
        KeyValues_fromJSON_object(
            TKeyValues<Key_t>& result,
            const json& object,
            const std::string& prefix,
            char delimiter
        ) {
        if constexpr (!std::constructible_from<Key_t, const std::string&>) {
            return std::unexpected(KeyValuesParseError::UnsupportedType);
        }
        else {
            for (auto it = object.begin(); it != object.end(); ++it) {

                // Construct the fully qualified key.
                std::string key = prefix;

                if (!key.empty())
                    key += delimiter;

                key += it.key();

                // Recursively flatten nested objects.
                if (it.value().is_object()) {
                    auto nested = KeyValues_fromJSON_object(
                        result,
                        it.value(),
                        key,
                        delimiter
                    );

                    if (!nested)
                        return std::unexpected(nested.error());

                    continue;
                }

                // Convert the leaf value.
                auto value = KeyValue_fromJSON(it.value());

                if (!value)
                    return std::unexpected(value.error());

                Key_t finalKey(key);

                if (result.contains(finalKey))
                    return std::unexpected(KeyValuesParseError::DuplicateKey);

                result.insert(finalKey, std::move(*value));
            }

            return {};
        }
    }



    template <class Key_t>
    std::expected<TKeyValues<Key_t>, KeyValuesParseError>
        KeyValues_fromJSON(const json& j, char delimiter) {

        TKeyValues<Key_t> result;

        // Positional array.
        if (j.is_array()) {
            for (const auto& element : j) {
                auto value = KeyValue_fromJSON(element);

                if (!value)
                    return std::unexpected(value.error());

                result.insert(std::move(*value));
            }

            return result;
        }

        // Named object, recursively flattened.
        if (j.is_object()) {
            auto parsed = KeyValues_fromJSON_object(result, j, "", delimiter);

            if (!parsed)
                return std::unexpected(parsed.error());

            return result;
        }

        return std::unexpected(KeyValuesParseError::UnsupportedType);
    }



} // namespace qk
