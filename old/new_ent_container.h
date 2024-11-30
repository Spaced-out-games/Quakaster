#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include <type_traits>
#include "Inspectible.h"

// Helper trait to check if a class has a to_string method
template<typename T, typename = void>
struct has_to_string : std::false_type {};

template<typename T>
struct has_to_string<T, std::void_t<decltype(std::declval<T>().to_string())>> : std::true_type {};

// Helper trait to check if a class has a get_name method
template<typename T, typename = void>
struct has_get_name : std::false_type {};

template<typename T>
struct has_get_name<T, std::void_t<decltype(T::get_name())>> : std::true_type {};

template <typename T>
class ent_container {

public:
    // Constructors
    ent_container() = default;
    ent_container(size_t initial_size) : container(initial_size) {}

    // Wrap std::vector's push_back
    void push_back(const T& value) {
        container.push_back(value);
    }

    void push_back(T&& value) {
        container.push_back(std::move(value));
    }

    // Wrap std::vector's emplace_back
    template <typename... Args>
    void emplace_back(Args&&... args) {
        container.emplace_back(std::forward<Args>(args)...);
    }

    // Wrap std::vector's operator[]
    T& operator[](size_t index) {
        return container[index];
    }

    const T& operator[](size_t index) const {
        return container[index];
    }

    // Wrap std::vector's size method
    size_t size() const {
        return container.size();
    }

    // Wrap std::vector's capacity method
    size_t capacity() const {
        return container.capacity();
    }

    // Wrap std::vector's resize method
    void resize(size_t new_size) {
        container.resize(new_size);
    }

    // Wrap std::vector's clear method
    void clear() {
        container.clear();
    }

    // Wrap std::vector's begin and end for iterators
    typename std::vector<T>::iterator begin() {
        return container.begin();
    }

    typename std::vector<T>::const_iterator begin() const {
        return container.begin();
    }

    typename std::vector<T>::iterator end() {
        return container.end();
    }

    typename std::vector<T>::const_iterator end() const {
        return container.end();
    }

    // Add any other custom methods you'd like to extend functionality.
    void remove_at(size_t index) {
        if (index < container.size()) {
            container.erase(container.begin() + index);
        }
    }

    // Expose the underlying container if absolutely necessary
    std::vector<T>& get_container() {
        return container;
    }
    template<typename U>
    ent_container<U> reinterpret_as() const {
        ent_container<U> converted_container;

        // Reinterpret the vector's data pointer and size
        converted_container.container = std::vector<U>(
            reinterpret_cast<U*>(container.data()),
            reinterpret_cast<U*>(container.data()) + (container.size() * sizeof(T) / sizeof(U))
            );

        return converted_container;
    }
    std::string to_string() const {
        std::ostringstream oss;
        for (const auto& entity : container) {
            oss << entity_to_string(entity) << "\n"; // Call helper function for string conversion
        }
        return oss.str(); // Return the aggregated string
    }
    private:
        std::vector<T> container;  // The underlying vector


        // Helper function to determine how to convert entity to string
        std::string entity_to_string(const T& entity) const {
            if constexpr (has_to_string<T>::value) {
                return entity.to_string(); // Use T::to_string() if available
            }
            else if constexpr (has_get_name<T>::value) {
                return T::get_name(); // Use T::get_name() if available
            }
            else {
                return std::to_string(reinterpret_cast<std::uintptr_t>(&entity)); // Fallback to address representation
            }
        }
};