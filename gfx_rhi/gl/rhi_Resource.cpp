#include "rhi_Resource.h" // just to convince Intellisense to give a shit.



namespace qk::gfx::rhi {

    Resource::Resource() = default;

    Resource::Resource(u64 id)
        : hResource(id)
    {

    }

    Resource::Resource(qk::string_hash label)
        : hResource(label.hash)
    {
    }

    Resource::operator u64() const {
        return hResource;
    }

    Resource::operator bool() const {
        return hResource != NULL_RESOURCE;
    }
}