#pragma once

#include <memory>
#include <vector>
#include <type_traits>

#include <boost/variant.hpp>

namespace snm
{
    template <typename T>
    using Vector = std::vector<T>;

    template <typename T>
    using Ptr = std::shared_ptr<T>;

    template <typename T, typename T2>
    using Pair = std::pair<T, T2>;

    using DataVariant = boost::variant<bool, unsigned short, unsigned int, unsigned long long, 
            short, int, long long, double, unsigned char, std::string, Vector<bool>, Vector<unsigned short>, 
            Vector<unsigned int>, Vector<unsigned long long>, Vector<short>, Vector<int>, Vector<long long>,
            Vector<double>, Vector<unsigned char>, Vector<std::string>>;
}