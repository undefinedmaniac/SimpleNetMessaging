#pragma once

#include "SimpleNetMessaging/Types.h"

#include <string>
#include <type_traits>

namespace snm
{
    enum DataType : unsigned char
    {
        Null   = 0x00,

        Bool   = 0x01, 
        UShort = 0x02, UInt = 0x03, ULong  = 0x04, 
        Short  = 0x05, Int  = 0x06, Long   = 0x07,
        Double = 0x08, Char = 0x09, String = 0x0A,

        BoolList   = 0x11, 
        UShortList = 0x12, UIntList = 0x13, ULongList  = 0x14, 
        ShortList  = 0x15, IntList  = 0x16, LongList   = 0x17,
        DoubleList = 0x18, CharList = 0x19, StringList = 0x1A,
    };

    template <typename T>
    struct GetDataType : std::integral_constant<DataType, Null> {};
    template <>
    struct GetDataType<bool> : std::integral_constant<DataType, Bool> {};
    template <>
    struct GetDataType<unsigned short> : std::integral_constant<DataType, UShort> {};
    template <>
    struct GetDataType<unsigned int> : std::integral_constant<DataType, UInt> {};
    template <>
    struct GetDataType<unsigned long long> : std::integral_constant<DataType, ULong> {};
    template <>
    struct GetDataType<short> : std::integral_constant<DataType, Short> {};
    template <>
    struct GetDataType<int> : std::integral_constant<DataType, Int> {};
    template <>
    struct GetDataType<long long> : std::integral_constant<DataType, Long> {};
    template <>
    struct GetDataType<double> : std::integral_constant<DataType, Double> {};
    template <>
    struct GetDataType<char> : std::integral_constant<DataType, Char> {};
    template <>
    struct GetDataType<std::string> : std::integral_constant<DataType, String> {};
    template <typename T>
    struct GetDataType<Vector<T>> : std::integral_constant<DataType, 
        std::conditional<GetDataType<T>::value == DataType::Null, GetDataType<T>, 
        std::integral_constant<DataType, 
        static_cast<snm::DataType>(GetDataType<T>::value + DataType::BoolList - DataType::Bool)>>::type::value> {};
}