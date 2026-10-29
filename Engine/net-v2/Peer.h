
#pragma once

#include <type_traits>
#include <tuple>
#include <core.h>

namespace qk::net_v2 {

    enum ChannelReliability : u8 {
        NET_UNRELIABLE = 0,
        NET_RELIABLE = 1,
    };

    template<typename T, ChannelReliability R>
    struct NetChannel
    {
        using type = T;
        static constexpr ChannelReliability reliability = R;
    };

    template<typename... Channels>
    struct ChannelMap
    {
        static_assert(sizeof...(Channels) <= 256);

        using Types = std::tuple<Channels...>;

        template<typename T>
        static constexpr std::size_t channel = []() consteval {
            constexpr bool matches[] = {
                std::is_same_v<T, typename Channels::type>...
            };

            std::size_t result = 0;
            std::size_t count = 0;

            for (std::size_t i = 0; i < sizeof...(Channels); ++i) {
                if (matches[i]) {
                    result = i;
                    ++count;
                }
            }

            if (count != 1)
                throw "Type must be registered exactly once";

            return result;
        }();

        template<std::size_t Channel>
        using channel_type = std::tuple_element_t<Channel, Types>;

        template<std::size_t Channel>
        using type = typename channel_type<Channel>::type;

        template<std::size_t Channel>
        static constexpr ChannelReliability reliability =
            channel_type<Channel>::reliability;

        template<typename T>
        static constexpr ChannelReliability type_reliability =
            reliability<channel<T>>;
    };

    template<typename T>
    struct isProtocol : std::false_type {};

    template<typename... Channels>
    struct isProtocol<ChannelMap<Channels...>> : std::true_type {};

    template<typename T>
    concept Protocol = isProtocol<T>::value;

    


}
