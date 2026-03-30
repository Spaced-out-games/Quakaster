/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/utility/ResourceLock.h
/// Purpose: Locks a resource for reading and writing
/// ************************************************************************************************** 

#pragma once
#include <stdint.h>
#include <concepts>
#include <utility>


namespace qk::util {
	template <class T>
	class ResourceLock;

	template<class T>
	concept Resource =
		requires(T * resource, ResourceLock<T> && lock) {
			{ resource->release(std::move(lock)) } -> std::same_as<void>;
	};

    template<class T>
    class ResourceLock {
        static constexpr uint32_t INVALID_LOCK = UINT32_MAX;

        T* m_Resource = nullptr;
        uint32_t m_ID = INVALID_LOCK;
        // 0 for read, 1 for write
        bool    m_Mode = 0;

    public:

        ResourceLock() = default;

        ResourceLock(T* resource, uint32_t id)
            : m_Resource(resource), m_ID(id) {}

        bool operator()() const {
            return (m_ID != INVALID_LOCK) && (m_Resource != nullptr);
        }

        ~ResourceLock() requires Resource<T> {
            if (*this) {
                m_Resource->release(std::move(*this));
            }
        }

        ResourceLock(ResourceLock&& other) noexcept
            : m_Resource(other.m_Resource), m_ID(other.m_ID)
        {
            other.m_Resource = nullptr;
            other.m_ID = INVALID_LOCK;
        }

        ResourceLock& operator=(ResourceLock&& other) noexcept
            requires Resource<T>
        {
            if (this != &other) {

                if (*this) {
                    m_Resource->release(std::move(*this));
                }

                m_Resource = other.m_Resource;
                m_ID = other.m_ID;

                other.m_Resource = nullptr;
                other.m_ID = INVALID_LOCK;
            }

            return *this;
        }


        ResourceLock(const ResourceLock&) = delete;
        ResourceLock& operator=(const ResourceLock&) = delete;
        ~ResourceLock() {
            m_Resource->release(std::move(*this));
        }
    };
}