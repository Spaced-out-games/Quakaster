#include "Resource.h"
#include "fnv1a.h"
#include <spdlog/spdlog.h>

#if QK_DEBUG
#include <iostream>
#endif

namespace qk {
	






	// Sets the label in debug builds
	void Resource::label(const str& name) {
		#if QK_DEBUG
		m_Name = name;
		#endif
	}

    // Returns the string value in debug builds, nullptr in release
    const str* Resource::label() const {
        #if QK_DEBUG
        return &m_Name;
        #else
        return nullptr;
        #endif

    }



    u32 Resource::refcount() const {
        return m_RefCount;
    }

    void Resource::refcount_override(u32 new_refcount) {
        m_RefCount = new_refcount;
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
		QK_ASSERT(m_RefCount > 0, "Resource release was attempted with m_RefCount == 0.");
        m_RefCount--;
    }

	Resource::Resource(const str& ResourceTypeName) {


		#if QK_DEBUG
        m_Name = std::format("{}@{:p}", ResourceTypeName, static_cast<const void*>(this));
		//spdlog::info("`{}`: ctor called.", m_Name);
		#endif
	}

	Resource::~Resource() {
		#if QK_DEBUG
        //spdlog::info("`{}`: dtor called.", m_Name);

		#endif
	}

}