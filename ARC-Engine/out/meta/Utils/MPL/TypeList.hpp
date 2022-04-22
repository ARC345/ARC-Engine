#pragma once

namespace MPL
{
    namespace Impl
    {
        // Compile-time list of types.
        template <typename... Ts>
        struct TypeList
        {
            static constexpr std::size_t size{sizeof...(Ts)};
        };
    }
}
