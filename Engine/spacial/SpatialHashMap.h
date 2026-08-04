/// **************************************** QUAKASTER ENGINE **************************************** 
/// spacial/SpatialHashMap.h 
/// Purpose: Defines a spacial hash map for optimized spacial queries
/// **************************************************************************************************
#pragma once
#include <../vendor/emhash/hash_table6.hpp>
#include <../vendor/emhash/hash_set4.hpp>
namespace qk {
    /*
        You MUST implement all of these functions using template specialization in order for spacial hash maps to work!!!
    
    
    */
    template <class Cell_t, class Position_t>
    struct SpatialTraits {
        static Cell_t to_cell(const Position_t& position, size_t CellSize);

        class iterator
        {
            Cell_t m_Min;
            Cell_t m_Max;
            Cell_t m_Current;
        public:
            iterator(Cell_t min, Cell_t max);



            Cell_t operator*() const;
            iterator& operator++();

            bool operator==(const iterator&) const;
        };

        iterator begin(Cell_t min, Cell_t max);
        iterator end(Cell_t min, Cell_t max);

    };

    // todo: consider making CellSize a Position_t scalar so we can buckets
    template <class Position_t, class Cell_t, class Value_t, size_t CellSize>
    struct SpatialHashMap {
        using Bucket_t = emhash4::HashSet<Value_t>;
    private:
        emhash6::HashMap<Cell_t, Bucket_t> m_Map;

        public:

            inline Cell_t get_cell(const Position_t& position) {
                return SpatialTraits<Cell_t, Position_t>::to_cell(position, CellSize);
            }

            // single-cell insertion
            void insert(const Cell_t& cell, const Value_t& item);
            // single-cell insertion
            void insert(const Position_t& position, const Value_t& item);

            // bounding box insertion
            void insert(const Position_t& min, const Position_t& max, const Value_t& item);

            // single-cell removal
            void remove(const Cell_t& cell, const Value_t& item);

            // single-cell removal
            void remove(const Position_t& cell, const Value_t& item);

            // bounding box deletion
            void remove(const Cell_t& min, const Cell_t& max, const Value_t& item);

            // bounding box deletion
            void remove(const Position_t& min, const Position_t& max, const Value_t& item);

            bool contains(const Cell_t& cell, const Value_t& item);

            // Visitor must be callable as:
            //
            // void(const Cell_t&, Bucket_t&)
            template <class Visitor>
            void for_each_cell(Visitor&& visitor);

            const auto& values_at(const Cell_t& cell);


    };



    

} // namespace qk
#include "SpatialHashMap.inl"