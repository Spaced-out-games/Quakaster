#pragma once


#include "../vendor/emhash/hash_table5.hpp"
#include <type_traits>
#include <media/json.h>
#include <expected>
#include <iterator>
#include <optional>
#include <concepts>
#include <utility>
#include <variant>
#include <compare>
#include <cstddef>
#include <vector>
#include <core.h>

namespace qk {

    using media::json;

    enum class KeyValuesParseError : u8 {
        UnsupportedType,
        MixedArrayTypes,
        MissingKey,
        InvalidIndex,
        DuplicateKey,
    };

    using KeyValueVariant_t = std::variant<
        str,
        u32,
        f32,
        bool,
        std::vector<u32>,
        std::vector<f32>,
        std::vector<str>,
        std::vector<bool>
    >;

    struct KeyValue : KeyValueVariant_t {
        using Base = KeyValueVariant_t;
        using Base::Base;
        using Base::operator=;

        template <class T>
        std::optional<T> as() const;

        template <class T>
        bool is() const;
    };

    template <class Key_t>
    struct TKeyValues {

        using ValueVariant_t = KeyValue;
        using ValueStorage_t = std::vector<ValueVariant_t>;
        using ValueLookup_t = emhash5::HashMap<Key_t, size_t>;

        using size_type = size_t;
        using key_type = Key_t;
        using value_type = ValueVariant_t;

        using iterator = ValueStorage_t::iterator;
        using const_iterator = ValueStorage_t::const_iterator;
        using reverse_iterator = ValueStorage_t::reverse_iterator;
        using const_reverse_iterator = ValueStorage_t::const_reverse_iterator;

        using key_iterator = typename ValueLookup_t::iterator;
        using const_key_iterator = typename ValueLookup_t::const_iterator;

        ValueStorage_t m_Values;
        ValueLookup_t m_ValueLookup;

        // Create
        void insert(const Key_t& key, const ValueVariant_t& value);
        void insert(const ValueVariant_t& value);



        // Read / update
        ValueVariant_t& operator[](size_type index);
        const ValueVariant_t& operator[](size_type index) const;

        ValueVariant_t& operator[](const Key_t& key);
        const ValueVariant_t& operator[](const Key_t& key) const;

        // Delete
        void remove(const Key_t& key);
        void remove(size_type index);

        // Queries
        bool contains(const Key_t& key) const;
        size_type size() const noexcept;
        size_type named_size() const noexcept;
        bool empty() const noexcept;
        void clear() noexcept;

        // Positional iteration
        iterator begin() noexcept;
        iterator end() noexcept;

        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;

        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;

        reverse_iterator rbegin() noexcept;
        reverse_iterator rend() noexcept;

        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator rend() const noexcept;

        // Value iterator
        struct ValueIterator {
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = ValueVariant_t;
            using reference = value_type&;
            using pointer = value_type*;

            iterator m_Iterator;

            reference operator*() const;

            ValueIterator& operator++();
            ValueIterator operator++(int);

            ValueIterator& operator--();
            ValueIterator operator--(int);

            ValueIterator& operator+=(difference_type n);
            ValueIterator& operator-=(difference_type n);

            ValueIterator operator+(difference_type n) const;
            ValueIterator operator-(difference_type n) const;

            difference_type operator-(const ValueIterator& other) const;

            reference operator[](difference_type n) const;

            bool operator==(const ValueIterator&) const = default;
            auto operator<=>(const ValueIterator&) const = default;
        };

        // Key iterator
        struct KeyValueEntry {
            const Key_t& key;
            ValueVariant_t& value;
        };

        struct KeyIterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using MapIterator = typename ValueLookup_t::iterator;

            TKeyValues* m_Owner;
            MapIterator m_Iterator;

            KeyValueEntry operator*() const;


            KeyIterator& operator++();
            KeyIterator operator++(int);

            bool operator==(const KeyIterator& other) const;
        };

        // Iterator access
        ValueIterator value_begin() noexcept;
        ValueIterator value_end() noexcept;

        KeyIterator key_begin() noexcept;
        KeyIterator key_end() noexcept;
    };

    using KeyValues = TKeyValues<str>;

    // Concrete, non-template parser.
    std::expected<KeyValue, KeyValuesParseError>
        KeyValue_fromJSON(const json& j);

    // Template parser.
    template <class Key_t>
    std::expected<TKeyValues<Key_t>, KeyValuesParseError>
    KeyValues_fromJSON(const json& j, char delimiter = '.');

}



#include "KeyValues.inl"