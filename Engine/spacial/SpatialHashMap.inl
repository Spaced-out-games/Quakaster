#pragma once
#include <iostream> // temporary

namespace qk
{

    template<class Position_t, class Cell_t, class Value_t, size_t CellSize>
    void SpatialHashMap<Position_t, Cell_t, Value_t, CellSize>::insert(
        const Cell_t& cell,
        const Value_t& item)
    {
        m_Map[cell].insert(item);
    }

    template<class Position_t, class Cell_t, class Value_t, size_t CellSize>
    void SpatialHashMap<Position_t, Cell_t, Value_t, CellSize>::insert(
        const Position_t& position,
        const Value_t& item)
    {
        insert(get_cell(position), item);
    }

    template<class Position_t, class Cell_t, class Value_t, size_t CellSize>
    void SpatialHashMap<Position_t, Cell_t, Value_t, CellSize>::insert(
        const Position_t& min,
        const Position_t& max,
        const Value_t& item)
    {
        const Cell_t minCell = get_cell(min);
        const Cell_t maxCell = get_cell(max);

        auto begin = SpatialTraits<Cell_t, Position_t>::begin(minCell, maxCell);
        auto end = SpatialTraits<Cell_t, Position_t>::end(minCell, maxCell);

        for (auto it = begin; it != end; ++it)
            insert(*it, item);
    }

    //------------------------------------------------------------

    template<class Position_t, class Cell_t, class Value_t, size_t CellSize>
    void SpatialHashMap<Position_t, Cell_t, Value_t, CellSize>::remove(
        const Cell_t& cell,
        const Value_t& item)
    {
        auto mapIt = m_Map.find(cell);

        if (mapIt == m_Map.end())
            return;

        mapIt->second.erase(item);

        if (mapIt->second.empty())
            m_Map.erase(mapIt);
    }

    template<class Position_t, class Cell_t, class Value_t, size_t CellSize>
    void SpatialHashMap<Position_t, Cell_t, Value_t, CellSize>::remove(
        const Position_t& position,
        const Value_t& item)
    {
        remove(get_cell(position), item);
    }

    template<class Position_t, class Cell_t, class Value_t, size_t CellSize>
    void SpatialHashMap<Position_t, Cell_t, Value_t, CellSize>::remove(
        const Cell_t& min,
        const Cell_t& max,
        const Value_t& item)
    {
        auto begin = SpatialTraits<Cell_t, Position_t>::begin(min, max);
        auto end = SpatialTraits<Cell_t, Position_t>::end(min, max);

        for (auto it = begin; it != end; ++it)
            remove(*it, item);
    }

    template<class Position_t, class Cell_t, class Value_t, size_t CellSize>
    void SpatialHashMap<Position_t, Cell_t, Value_t, CellSize>::remove(
        const Position_t& min,
        const Position_t& max,
        const Value_t& item)
    {
        remove(get_cell(min), get_cell(max), item);
    }

    //------------------------------------------------------------

    template<class Position_t, class Cell_t, class Value_t, size_t CellSize>
    bool SpatialHashMap<Position_t, Cell_t, Value_t, CellSize>::contains(
        const Cell_t& cell,
        const Value_t& item)
    {
        auto mapIt = m_Map.find(cell);

        if (mapIt == m_Map.end())
            return false;

        return mapIt->second.contains(item);
    }

    //------------------------------------------------------------

    template<class Position_t, class Cell_t, class Value_t, size_t CellSize>
    template<class Visitor>
    void SpatialHashMap<Position_t, Cell_t, Value_t, CellSize>::for_each_cell(
        Visitor&& visitor)
    {
        for (auto it = m_Map.begin(); it != m_Map.end(); ++it)
        {
            auto& entry = *it;
            visitor(entry.first, entry.second);
        }

    }



    template<class Position_t, class Cell_t, class Value_t, size_t CellSize>
    const auto& SpatialHashMap<Position_t, Cell_t, Value_t, CellSize>::values_at(
        const Cell_t& cell)
    {
        auto mapIt = m_Map.find(cell);

        assert(mapIt != m_Map.end());

        return mapIt->second;
    }


} // namespace qk