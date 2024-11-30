#include <cstdlib>
#include <stdexcept>
#include <new>
#include <iostream>
#include <vector>
#include <utility>
#include <chrono>
#include <unordered_map>
#include <typeindex>
#include <stdexcept>
#include <cstdlib>
#include <random>
#include <cstddef> // For size_t
#include <stdexcept> // For exceptions

template <typename Ret, typename... Args>
using func_ptr_t = Ret(*)(Args...);









template <typename T>
class aligned_ptr {
public:
    aligned_ptr(void* data = nullptr, size_t stride = 0) : ptr((uint8_t*)data), element_size(stride)
    {
        if (reinterpret_cast<uintptr_t>(ptr) % alignof(T) != 0) {
            throw std::runtime_error("Pointer is not aligned correctly.");
        }
    }

    T& operator*() {
        return *reinterpret_cast<T*>(ptr);
    }

    T* operator->() {
        return reinterpret_cast<T*>(ptr);
    }

    T& operator[](size_t index) {
        return *reinterpret_cast<T*>(ptr + index * element_size);
    }

    aligned_ptr& operator++() {
        ptr += element_size;
        return *this;
    }

    aligned_ptr operator++(int) {
        aligned_ptr temp = *this;
        ++(*this);
        return temp;
    }

private:
    uint8_t* ptr;
    size_t element_size;
};


// Type-erased array class definition (from your code)
struct typeless_array {
public:
    template <typename Base>
    friend class Scene;

    typeless_array() : capacity(0), T_Size(0), size(0) {}

    // Factory method to create a typeless array
    template <typename T>
    static typeless_array create(size_t capacity) {
        return typeless_array(capacity, sizeof(T));
    }

    // Destructor to clean up allocated memory
    ~typeless_array() = default;

    // Access an element at a specific index
    void* operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return static_cast<char*>(data) + T_Size * index;
    }

    // Add an element to the array
    template <typename T>
    void push_back(const T& value) {
        if (T_Size != sizeof(T)) {
            throw std::runtime_error("Type mismatch in push_back");
        }
        if (size >= capacity) {
            throw std::out_of_range("Array capacity exceeded");
        }
        void* dest = static_cast<char*>(data) + T_Size * size;
        new (dest) T(value); // Placement new to copy-construct the value
        ++size;
    }

    // Emplace an element in the array
    template <typename T, typename... Args>
    void emplace_back(Args&&... args) {
        if (T_Size != sizeof(T)) {
            throw std::runtime_error("Type mismatch in emplace_back");
        }
        if (size >= capacity) {
            throw std::out_of_range("Array capacity exceeded");
        }
        void* dest = static_cast<char*>(data) + T_Size * size;
        new (dest) T(std::forward<Args>(args)...); // Placement new to construct in-place
        ++size;
    }


    // Get a pointer to the start of the array as a specific type
    template <typename T>
    aligned_ptr<T> get_start() {
        if (data == nullptr) {
            throw std::runtime_error("Data is uninitialized");
        }
        return aligned_ptr<T>(data, T_Size);
    }

    // Getters for size and capacity
    size_t get_size() const { return size; }
    size_t get_capacity() const { return capacity; }
    size_t get_elem_size() const { return T_Size; }
    //private: commented for now
        // Private constructor to enforce usage of the factory method
    typeless_array(size_t capacity, size_t size_of_T)
        : capacity(capacity), T_Size(size_of_T), size(0) {}

    void* data = nullptr;
    size_t size = 0;
    size_t capacity = 0;
    size_t T_Size = 0;
};



// TODO: Add a means to resize

// Holds an array of many objects, all of which derive from Base
template <typename Base = ent>
struct Scene {
    std::vector<typeless_array> arrays;

    // Typename as a string to array index map
    std::unordered_map<std::string, size_t> type_name_map;

    // Type index to array index map
    std::unordered_map<std::type_index, size_t> type_index_map;

    // Add an array of a certain type
    template <typename Derived, size_t capacity>
    void add_array() {
        type_name_map[Derived::get_name()] = arrays.size();
        type_index_map[typeid(Derived)] = arrays.size();
        arrays.push_back(typeless_array::create<Derived>(capacity));
    }
    // Creates a contiguous buffer containing all of the entities. It also updates the arrays to point to their subsections
    void init()
    {


        size_t total_size = 0;
        size_t alignment = alignof(std::max_align_t);

        for (const auto& array : arrays) {
            size_t array_size = array.capacity * array.T_Size;
            total_size += array_size;
            capacity += array.capacity;
        }

        void* buffer = malloc(total_size);
        if (!buffer) {
            throw std::bad_alloc();
        }

        char* start = static_cast<char*>(buffer);
        for (auto& array : arrays) {
            array.data = start;
            size_t array_size = array.capacity * array.T_Size;
            start += array_size;
        }
    }

    // Gets a sub-array by raw index
    typeless_array& operator[](size_t index)
    {
        return arrays[index];
    }

    // Gets a sub-array by it's type name string
    typeless_array& operator[](const std::string& type_name)
    {
        // get the correct index in the hash map
        auto it = type_name_map.find(type_name);

        // Throw an error if the array doesn't exist
        if (it == type_name_map.end()) {
            throw std::runtime_error("Type not found in Scene. Add an add_scene<T> to the bootstrap step");
        }
        // get the index from the hash map and return the appropriate typeless_array
        return arrays[it->second];
    }

    // Gets a sub-array by it's type_index
    template <typename T>
    typeless_array& from_typename() {
        auto it = type_index_map.find(std::type_index(typeid(T)));
        if (it == type_index_map.end()) {
            throw std::runtime_error("Array for type not found in Scene");
        }
        return arrays[it->second];
    }





    // Adds an array
    template <typename Derived, typename... Args>
    void add_entity(Args&&... args) {
        auto it = type_index_map.find(typeid(Derived));
        if (it == type_index_map.end()) {
            throw std::runtime_error("Array for type not found in Scene");
        }

        typeless_array& array = arrays[it->second];
        array.emplace_back<Derived>(std::forward<Args>(args)...);
        size++;
    }

    size_t size = 0;
    size_t capacity = 0;


};

template <typename Base>
struct SceneIterator {
    Scene<Base>* mScene = nullptr;   // Pointer to the Scene
    size_t which_array = 0;           // Current array index in the Scene
    size_t which_index = 0;           // Current index within the current array
    aligned_ptr<Base> current_ptr = nullptr; // Pointer to the current array

    SceneIterator(Scene<Base>& scene)
        : mScene(&scene),
        which_array(0),
        which_index(0),
        current_ptr(nullptr, 0)
    {
        if (mScene->arrays.empty()) {
            throw std::runtime_error("Cannot initialize SceneIterator: Scene is empty. Call add_array or init() first.");
        }

        // Point to the first array (not element)
        current_ptr = (*mScene)[0].get_start<Base>();
    }

    Base& next() {
        if (!has_next()) {
            // Wrap around to the first element if we've exhausted the array list
            which_array = 0;
            which_index = 0;
            current_ptr = (*mScene)[0].get_start<Base>();
        }

        // Access the current object within the current array (use current_ptr + which_index)
        Base& result = current_ptr[which_index];

        // Increment the index within the current array
        ++which_index;

        // If the index is out of range for the current array, move to the next array
        if (which_index >= (*mScene)[which_array].get_size()) {
            ++which_array;
            which_index = 0;

            // If we've reached the end of all arrays, wrap back to the first array
            if (which_array >= mScene->arrays.size()) {
                which_array = 0;  // Wrap around to the first array
            }

            // Update current_ptr to the start of the next array (or first array if wrapped)
            current_ptr = (*mScene)[which_array].get_start<Base>();
        }

        return result;
    }

    bool has_next() const {
        // We always have a next element unless we're stuck in an empty scene
        return !mScene->arrays.empty();
    }
};




// TODO: add "aligned_ptr<T>" type. It will be a wrapper for the pointer returned by typeless_array::operator[].
// Subsequently, Scene::operator[] should return one of these automatically casted to a Base*
// Maybe consider having aligned_ptr's constructor to take in an aligned_array& so that Scene can create and disperse them easily.
// Make sure the aligned_ptr doesn't free the pointer, as IT DOES NOT OWN IT!!!
// Also, consider creating iterators to go through the Scene and return Base& with .begin() to reset the offset to 0.
// It stores a num_elements internally with hoe many elements are left
// It stores an array index
// it stores an ent* to the first array, incrementing by sizeof(current_array_type)
// Use .next() and .prev() to move forwards and backwards
// This allows us to easily iterate over it all in one loop, not the shit that we have above
