
#include <spdlog/spdlog.h>
#include <format>

namespace qk {

    template <class Resource_t, Resource_dtor_pfn_t<Resource_t> Resource_dtor, Resource_free_pfn_t<Resource_t> Resource_free>
    void ResourceRegistry<Resource_t, Resource_dtor, Resource_free>::insert(const Resource_key_t& key, Resource_t* pResource) {

        QK_ASSERT(pResource, "pResource was nullptr.");

        Resource* pResourceBase = Resource_cast(pResource);
        m_Resources[key] = pResource;




        #if QK_DEBUG
            QK_ASSERT(!m_Resources.contains(key), std::format("ResourceRegistry::insert hash collision for resource with key: {}", *(pResourceBase->label())));
        #else
            QK_ASSERT(!m_Resources.contains(key), std::format("ResourceRegistry::insert hash collision for resource with key: {}", key));
        #endif





        // We know from the previous asserts that pResource is always non-null but we can't gaurantee Resource_cast(Resource_t*) never returns nullptr so we aught put an assert here to catch some bizarre edge cases

        QK_ASSERT(pResourceBase, "Resource_cast(pResource) returned nullptr");


        // Not a strict invariant but is the most hygeinic practice. Feel free to comment if you must.  
        QK_ASSERT(pResourceBase->refcount() == 0, "[PROGRAMMER WARNING]: pResourceBase->refcount() was non-zero.");


        // hold a lock on lifecycle
        pResourceBase->acquire();

    }



    template <class Resource_t, Resource_dtor_pfn_t<Resource_t> Resource_dtor, Resource_free_pfn_t<Resource_t> Resource_free>
    bool ResourceRegistry<Resource_t, Resource_dtor, Resource_free>::try_remove(const Resource_key_t& key) {
        QK_ASSERT(m_Resources.contains(key), std::format("Key {} does not exist", key));

        Resource_t* pResource = m_Resources[key];

        Resource* pResourceBase = Resource_cast(pResource);
        // We know from the previous asserts that pResource is always non-null but we can't gaurantee Resource_cast(Resource_t*) never returns nullptr so we aught put an assert here to catch some bizarre edge cases
        QK_ASSERT(pResourceBase, "Resource_cast(pResource) returned nullptr");

        // only delete and free if this is the only owner AND it's ready to go
        if (pResourceBase->refcount() == 1 && pResourceBase->flagged()) {
            pResourceBase->release();
            Resource_dtor(pResource);
            m_Resources.erase(key);
            Resource_free(pResource);
            return true;
        }

        return false;

    }


    template <class Resource_t, Resource_dtor_pfn_t<Resource_t> Resource_dtor, Resource_free_pfn_t<Resource_t> Resource_free>
    Resource_t* ResourceRegistry<Resource_t, Resource_dtor, Resource_free>::peek(const Resource_key_t& key) {

        if (m_Resources.contains(key))
            return m_Resources.at(key);
        return nullptr;
    }

    template <class Resource_t, Resource_dtor_pfn_t<Resource_t> Resource_dtor, Resource_free_pfn_t<Resource_t> Resource_free>
    Resource_t* ResourceRegistry<Resource_t, Resource_dtor, Resource_free>::acquire(const Resource_key_t& key) {
        QK_ASSERT(m_Resources.contains(key), std::format("Resource with key {} does not exist", key));
        Resource_t* pResource = m_Resources.at(key);
        Resource* pResourceBase = Resource_cast(pResource);

        // We know from the previous asserts that pResource is always non-null but we can't gaurantee Resource_cast(Resource_t*) never returns nullptr so we aught put an assert here to catch some bizarre edge cases
        QK_ASSERT(pResourceBase, "Resource_cast(pResource) returned nullptr");

        if (pResourceBase->flagged()) return nullptr;

        QK_ASSERT(pResourceBase->refcount(), "pResourceBase->refcount() was too high");


        pResourceBase->acquire();

        return pResource;

    }

    template <class Resource_t, Resource_dtor_pfn_t<Resource_t> Resource_dtor, Resource_free_pfn_t<Resource_t> Resource_free>
    bool ResourceRegistry<Resource_t, Resource_dtor, Resource_free>::try_release(const Resource_key_t& key)
    {
        QK_ASSERT(m_Resources.contains(key), std::format("Resource with key {} does not exist", key));


        Resource_t* pResource = m_Resources.at(key);
        Resource* pResourceBase = Resource_cast(pResource);

        QK_ASSERT(pResourceBase, "pResourceBase was nullptr");
        QK_ASSERT(pResourceBase, "pResourceBase->refcount() was 0");

        if (pResourceBase->refcount() == 1)
            return false;

        pResourceBase->release();
        return true;
    }

    template <class Resource_t, Resource_dtor_pfn_t<Resource_t> Resource_dtor, Resource_free_pfn_t<Resource_t> Resource_free>
    void ResourceRegistry<Resource_t, Resource_dtor, Resource_free>::collect_pending() {
        for (auto it = m_Resources.begin(); it != m_Resources.end(); )
        {
            auto* pResource = it->second;
            auto* pBase = Resource_cast(pResource);

            if (pBase->refcount() == 1 && pBase->flagged())
            {
                pBase->release();
                Resource_dtor(pResource);
                Resource_free(pResource);

                it = m_Resources.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }


    template <class Resource_t, Resource_dtor_pfn_t<Resource_t> Resource_dtor, Resource_free_pfn_t<Resource_t> Resource_free>
    bool ResourceRegistry<Resource_t, Resource_dtor, Resource_free>::has(Resource_key_t key) const {
        return m_Resources.contains(key);
    }


    template <class Resource_t, Resource_dtor_pfn_t<Resource_t> Resource_dtor, Resource_free_pfn_t<Resource_t> Resource_free>
    void ResourceRegistry<Resource_t, Resource_dtor, Resource_free>::destroy_everything() {
        Resource* pResourceBase;

        for (auto it = m_Resources.begin(); it != m_Resources.end(); ) {
            auto* pResource = it->second;
            auto* pBase = Resource_cast(pResource);
            const str* pLabel = pBase->label();
            if (pBase->refcount() > 1) {
                if (pLabel)

                    spdlog::warn("~ResourceRegistry()@{}: Resource `{}` was freed despite existing co-owners", (const void*)this, *pLabel);
                else
                    spdlog::warn("~ResourceRegistry()@{}: Resource `{}` was freed despite existing co-owners", (const void*)this, (const void*)pBase);


            }

            pBase->flag_for_deletion();
            pBase->refcount_override(0);
            Resource_dtor(pResource);
            it = m_Resources.erase(it);
            Resource_free(pResource);


            

        }
    }

    template <class Resource_t, Resource_dtor_pfn_t<Resource_t> Resource_dtor, Resource_free_pfn_t<Resource_t> Resource_free>
    ResourceRegistry<Resource_t, Resource_dtor, Resource_free>::~ResourceRegistry() {
        destroy_everything();
    }

}