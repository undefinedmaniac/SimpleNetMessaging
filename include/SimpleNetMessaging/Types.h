#pragma once

#include <memory>
#include <vector>
#include <limits>
#include <type_traits>

namespace snm
{
    template <typename T>
    using Vector = std::vector<T>;

    template <typename T>
    using Ptr = std::shared_ptr<T>;

    template <typename T, typename T2>
    using Pair = std::pair<T, T2>;

    template <typename T1, typename T2>
    using FitsInto = std::integral_constant<bool,  
        std::numeric_limits<T1>::is_specialized &&
        std::numeric_limits<T2>::is_specialized &&
        (std::numeric_limits<T1>::max() <= std::numeric_limits<T2>::max()) && 
        (std::numeric_limits<T1>::min() >= std::numeric_limits<T2>::min())>;
}