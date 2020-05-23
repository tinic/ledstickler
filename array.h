#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <array>

namespace ledstickler {

    template <typename T, std::size_t n, std::size_t... ns>
    struct multi_array_ {
        using type = std::array<typename multi_array_<T, ns...>::type, n>;
    };

    template <typename T, std::size_t n>
    struct multi_array_<T, n> {
        using type = std::array<T, n>;
    };

    template <typename T, std::size_t... ns>
    using multi_array = typename multi_array_<T, ns...>::type;

}

#endif  // #ifndef _ARRAY_H_
