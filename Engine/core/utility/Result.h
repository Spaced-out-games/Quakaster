#pragma once
#include <cstdint>
#include <utility>
#include "Handle.h"

namespace qk {

    using Error_t = uint64_t;


    template <class Tag>
    const char* to_string(Error_t code);

    template <class T, class Tag>
    bool ok(Error_t code);

    template<class T, class Tag>
    class Result {

    public:


        explicit operator bool() const noexcept {
            return qk::ok<Tag>(m_Error);
        }

        const char* to_string() const {
            return qk::to_string<Tag>(m_Error);
        }

        T value() && noexcept {
            return std::move(m_Value);
        }

        const T& value() const& noexcept {
            return m_Value;
        }
    private:
        
        T       m_Value;
        Error_t m_Error;
    };

} // namespace qk