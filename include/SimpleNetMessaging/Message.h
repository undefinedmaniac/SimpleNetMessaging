#pragma once

#include <string>
#include <vector>
#include <memory>

#include <boost/variant.hpp>

namespace snm
{
// std::vector<unsigned char> messageToBytes(const IMessage& message);
// std::shared_ptr<IMessage> bytesToMessage(const std::vector<unsigned char>& bytes);

    template <typename T>
    using Vector = std::vector<T>;

    template <typename T>
    using Ptr = std::shared_ptr<T>;

    template <typename T, typename T2>
    using Pair = std::pair<T, T2>;

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

    class Message
    {
    public:
        using Variant = boost::variant<bool, unsigned short, unsigned int, unsigned long long, 
            short, int, long long, double, char, std::string, Vector<bool>, Vector<unsigned short>, 
            Vector<unsigned int>, Vector<unsigned long long>, Vector<short>, Vector<int>, Vector<long long>,
            Vector<double>, Vector<char>, Vector<std::string>>;

        using MessageData = Pair<DataType, Variant>;
        using MessageDataPtr = Ptr<MessageData>;

        // Check if the message is empty
        bool isEmpty() const;

        // Get the number of parts in the message
        unsigned int size() const;

        // Get the type information for a part of the message
        DataType getType(unsigned int index) const;

        // Retrieve data from the message
        template <typename T>
        const T& getData(unsigned int index) const { return boost::get<T>(std::get<1>(*_data[index])); }

        // Get submessages
        Ptr<Message> getSubmessage(unsigned int startIndex, unsigned int length) const;
        Ptr<Message> getLeft(unsigned int length) const;
        Ptr<Message> getRight(unsigned int length) const;
    private:
        Message();

        Vector<MessageDataPtr> _data;
        friend class MessageBuilder;
    };

    class MessageBuilder
    {
        std::shared_ptr<Message> _message;

        template <typename T>
        void addData(T data);

        template <typename T>
        void addDataRef(const T& data);
    public:
        MessageBuilder& start();

        MessageBuilder& addBool(bool data);
        MessageBuilder& addUShort(unsigned short data);
        MessageBuilder& addUInt(unsigned int data);
        MessageBuilder& addULong(unsigned long long data);
        MessageBuilder& addShort(short data);
        MessageBuilder& addInt(int data);
        MessageBuilder& addLong(long long data);
        MessageBuilder& addDouble(double data);
        MessageBuilder& addChar(char data);
        MessageBuilder& addString(const char data[]);
        MessageBuilder& addString(const std::string& data);

        MessageBuilder& addBoolList(const std::vector<bool>& data);
        MessageBuilder& addUShortList(const std::vector<unsigned short>& data);
        MessageBuilder& addUIntList(const std::vector<unsigned int>& data);
        MessageBuilder& addULongList(const std::vector<unsigned long long>& data);
        MessageBuilder& addShortList(const std::vector<short>& data);
        MessageBuilder& addIntList(const std::vector<int>& data);
        MessageBuilder& addLongList(const std::vector<long long>& data);
        MessageBuilder& addDoubleList(const std::vector<double>& data);
        MessageBuilder& addCharList(const std::vector<char>& data);
        MessageBuilder& addStringList(const std::vector<std::string>& data);

        MessageBuilder& addMessage(const Message& message);

        std::shared_ptr<Message> finish();
    };
}