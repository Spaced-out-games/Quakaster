#include "Resource.h"
#include "fnv1a.h"
#include <spdlog/spdlog.h>

#ifdef _DEBUG
#include <iostream>
#endif

namespace qk {
	
	void Resource::destroy() {

	}

	// Whether or not this resource is ready to be deleted. 
	bool Resource::can_destroy() {
		return m_RefCount == 0 && m_FlaggedForDeletion;
	}

	// Try to destroy the resource
	bool Resource::try_destroy() {
		if (!can_destroy()) return false;

		destroy();
		return true;
	}

	// Sets the label in debug builds
	void Resource::set_label(const str& label) {
		#ifdef _DEBUG
		m_Label = label;
		m_Hash = (u64)hash_fnv1a(label);
		#endif
	}

    // Returns the string value in debug builds, nullptr in release
    const str* Resource::label() const {
        #ifdef _DEBUG
        return &m_Label;
        #else
        return nullptr;
        #endif

    }

    // Returns the hash code in debug builds, nullptr in release
    const u64* Resource::hash_code() {
        #ifdef _DEBUG
        return &m_Hash;
        #else
        return nullptr;
        #endif

    }

    u32 Resource::refcount() const {
        return m_RefCount;
    }

    bool Resource::flagged() const {
        return m_FlaggedForDeletion;
    }

    void Resource::flag_for_deletion() {
        m_FlaggedForDeletion = true;
    }
    void Resource::acquire() {
        m_RefCount++;
    }

    void Resource::release() {
        m_RefCount--;
    }

    Resource::~Resource() {
        #ifdef _DEBUG
            spdlog::info("Destroyed resource `{}` at ", m_Label);
        #endif
    }

}