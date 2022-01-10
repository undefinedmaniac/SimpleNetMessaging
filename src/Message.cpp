#include "SimpleNetMessaging/Message.h"

#include <math.h>
#include <boost/endian.hpp>

namespace snm
{
    // template<typename T, typename F>
    // void listToBytesHelper(std::vector<unsigned char>& content, unsigned int contentSize, const std::vector<T>& list, F f, unsigned short typeSize) {
    //     unsigned short listSize = list.size();
    //     content.resize(contentSize + 2 + typeSize*listSize);
    //     boost::endian::store_big_u16(content.data() + contentSize, listSize);
    //     unsigned char* pos = content.data() + contentSize + 2;
    //     for (T data : list) {
    //         f(pos, data);
    //         pos += typeSize;
    //     }
    // }

    // std::vector<unsigned char> messageToBytes(const IMessage& message) {
    //     // Static checks to ensure that type casting will work
    //     static_assert(sizeof(bool) == 1,
    //         "Message serialization requires 1 byte booleans");
    //     static_assert(sizeof(short) == sizeof(int16_t),
    //         "Message serialization requires 2 byte shorts");
    //     static_assert(sizeof(int) == sizeof(int32_t),
    //         "Message serialization requires 4 byte integers");
    //     static_assert(sizeof(long long) == sizeof(int64_t),
    //         "Message serialization requires 8 byte long long type");
    //     static_assert(sizeof(char) == 1,
    //         "Message serialization requires 1 byte characters");
    //     static_assert(sizeof(double) == sizeof(int64_t),
    //         "Message serialization requires 8 byte doubles");

    //     std::vector<unsigned char> typeData, content;

    //     const unsigned int messageSize = message.size();
    //     typeData.reserve(messageSize);
    //     for (unsigned int i = 0; i < messageSize; i++) {
    //         // Store type data in the typeData vector
    //         DataType type = message.getType(i);
    //         typeData.push_back(type);

    //         const unsigned int contentSize = content.size();
    //         switch (type) {
    //             case Bool: {
    //                 content.push_back(message.getBool(i));
    //                 break;
    //             }
    //             case UShort: {
    //                 content.resize(contentSize + 2);
    //                 boost::endian::store_big_u16(content.data() + contentSize, message.getUShort(i));
    //                 break;
    //             }
    //             case UInt: {
    //                 content.resize(contentSize + 4);
    //                 boost::endian::store_big_u32(content.data() + contentSize, message.getUInt(i));
    //                 break;
    //             }
    //             case ULong: {
    //                 content.resize(contentSize + 8);
    //                 boost::endian::store_big_u64(content.data() + contentSize, message.getULong(i));
    //                 break;
    //             }
    //             case Short: {
    //                 content.resize(contentSize + 2);
    //                 boost::endian::store_big_s16(content.data() + contentSize, message.getShort(i));
    //                 break;
    //             }
    //             case Int: {
    //                 content.resize(contentSize + 4);
    //                 boost::endian::store_big_s32(content.data() + contentSize, message.getInt(i));
    //                 break;
    //             }
    //             case Long: {
    //                 content.resize(contentSize + 8);
    //                 boost::endian::store_big_s64(content.data() + contentSize, message.getLong(i));
    //                 break;
    //             }
    //             case Double: {
    //                 content.resize(contentSize + 8);
    //                 double doubleData = message.getDouble(i);
    //                 boost::endian::store_big_u64(content.data() + contentSize, reinterpret_cast<uint64_t&>(doubleData));
    //                 break;
    //             }
    //             case Char: {
    //                 content.push_back(message.getChar(i));
    //                 break;
    //             }
    //             case String: {
    //                 const std::string& stringData = message.getString(i);
    //                 content.insert(content.end(), stringData.c_str(), stringData.c_str() + stringData.size() + 1);
    //                 break;
    //             }
    //             case BoolList: {
    //                 unsigned short listSize = message.getBoolList(i).size();
    //                 unsigned short listBytesNeeded = (listSize + 7) / 8;
    //                 content.resize(contentSize + 2 + listBytesNeeded);
    //                 boost::endian::store_big_u16(content.data() + contentSize, listSize);
    //                 for (unsigned short j = 0; j < listSize; j++) {
    //                     if (message.getBoolList(i)[j])
    //                         content[contentSize + 2 + j / 8] |= (0x80 >> j % 8);
    //                 }
    //                 break;
    //             }
    //             case UShortList: {
    //                 listToBytesHelper(content, contentSize, message.getUShortList(i), boost::endian::store_big_u16, 2);
    //                 break;
    //             }
    //             case UIntList: {
    //                 listToBytesHelper(content, contentSize, message.getUIntList(i), boost::endian::store_big_u32, 4);
    //                 break;
    //             }
    //             case ULongList: {
    //                 listToBytesHelper(content, contentSize, message.getULongList(i), boost::endian::store_big_u64, 8);
    //                 break;
    //             }
    //             case ShortList: {
    //                 listToBytesHelper(content, contentSize, message.getShortList(i), boost::endian::store_big_s16, 2);
    //                 break;
    //             }
    //             case IntList: {
    //                 listToBytesHelper(content, contentSize, message.getIntList(i), boost::endian::store_big_s32, 4);
    //                 break;
    //             }
    //             case LongList: {
    //                 listToBytesHelper(content, contentSize, message.getLongList(i), boost::endian::store_big_s64, 8);
    //                 break;
    //             }
    //             case DoubleList: {
    //                 std::vector<uint64_t> transformedList;
    //                 transformedList.reserve(message.getDoubleList(i).size());
    //                 for (double dbl : message.getDoubleList(i))
    //                     transformedList.push_back(reinterpret_cast<uint64_t&>(dbl));
    //                 listToBytesHelper(content, contentSize, transformedList, boost::endian::store_big_u64, 8);
    //                 break;
    //             }
    //             case CharList: {
    //                 listToBytesHelper(content, contentSize, message.getCharList(i), [](unsigned char* pos, char data){ *pos = data; }, 1);
    //                 break;
    //             }
    //             case StringList: {
    //                 unsigned short totalSize = 0;
    //                 for (const std::string& str : message.getStringList(i))
    //                     totalSize += str.size() + 1;

    //                 unsigned short listSize = message.getStringList(i).size();
    //                 content.resize(contentSize + 2 + totalSize);
    //                 boost::endian::store_big_u16(content.data() + contentSize, listSize);
    //                 unsigned char* pos = content.data() + contentSize + 2;
    //                 for (const std::string& str : message.getStringList(i)) {
    //                     unsigned short stringSize = str.size() + 1;
    //                     std::copy(str.c_str(), str.c_str() + stringSize, pos);
    //                     pos += stringSize;
    //                 }
    //                 break;
    //             }
    //         }
    //     }

    //     // End the type data with a null terminator and then append the content
    //     typeData.reserve(typeData.size() + 1 + content.size());
    //     typeData.push_back(0);
    //     typeData.insert(typeData.end(), content.begin(), content.end());

    //     return typeData;
    // }
    // std::shared_ptr<IMessage> bytesToMessage(const std::vector<unsigned char>& bytes) {
        
    // }

    Message::Message() {}

    // Check if the message is empty
    bool Message::isEmpty() const { return _data.empty(); }

    // Get the number of parts in the message
    unsigned int Message::size() const { return _data.size(); }

    // Get the type information for a part of the message
    DataType Message::getType(unsigned int index) const { return _data[index]->first; }

    Ptr<Message> Message::getSubmessage(unsigned int startIndex, unsigned int length) const {
        Ptr<Message> newMessage(new Message());
        newMessage->_data.insert(newMessage->_data.end(), _data.begin() + startIndex, _data.begin() + startIndex + length);
        return newMessage;
    }

    Ptr<Message> Message::getLeft(unsigned int length) const {
        return getSubmessage(0, length);
    }

    Ptr<Message> Message::getRight(unsigned int length) const {
        return getSubmessage(size() - length, length);
    }

    template <typename T>
    void MessageBuilder::addData(T data) {
        using std::make_shared;
        using std::make_pair;
        _message->_data.push_back(make_shared<Message::MessageData>(make_pair(GetDataType<T>::value, data)));
    }

    template <typename T>
    void MessageBuilder::addDataRef(const T& data) {
        using std::make_shared;
        using std::make_pair;
        _message->_data.push_back(make_shared<Message::MessageData>(make_pair(GetDataType<T>::value, data)));
    }

    MessageBuilder& MessageBuilder::start() { _message = std::shared_ptr<Message>(new Message()); return *this; }

    MessageBuilder& MessageBuilder::addBool(bool data) { addData(data); return *this; }
    MessageBuilder& MessageBuilder::addUShort(unsigned short data) { addData(data); return *this; }
    MessageBuilder& MessageBuilder::addUInt(unsigned int data) { addData(data); return *this; }
    MessageBuilder& MessageBuilder::addULong(unsigned long long data) { addData(data); return *this; }
    MessageBuilder& MessageBuilder::addShort(short data) { addData(data); return *this; }
    MessageBuilder& MessageBuilder::addInt(int data) { addData(data); return *this; }
    MessageBuilder& MessageBuilder::addLong(long long data) { addData(data); return *this; }
    MessageBuilder& MessageBuilder::addDouble(double data) { addData(data); return *this; }
    MessageBuilder& MessageBuilder::addChar(char data) { addData(data); return *this; }
    MessageBuilder& MessageBuilder::addString(const char data[]) { addDataRef(std::string(data)); return *this; }
    MessageBuilder& MessageBuilder::addString(const std::string& data) { addDataRef(data); return *this; }

    MessageBuilder& MessageBuilder::addBoolList(const std::vector<bool>& data) { addDataRef(data); return *this; }
    MessageBuilder& MessageBuilder::addUShortList(const std::vector<unsigned short>& data) { addDataRef(data); return *this; }
    MessageBuilder& MessageBuilder::addUIntList(const std::vector<unsigned int>& data) { addDataRef(data); return *this; }
    MessageBuilder& MessageBuilder::addULongList(const std::vector<unsigned long long>& data) { addDataRef(data); return *this; }
    MessageBuilder& MessageBuilder::addShortList(const std::vector<short>& data) { addDataRef(data); return *this; }
    MessageBuilder& MessageBuilder::addIntList(const std::vector<int>& data) { addDataRef(data); return *this; }
    MessageBuilder& MessageBuilder::addLongList(const std::vector<long long>& data) { addDataRef(data); return *this; }
    MessageBuilder& MessageBuilder::addDoubleList(const std::vector<double>& data) { addDataRef(data); return *this; }
    MessageBuilder& MessageBuilder::addCharList(const std::vector<char>& data) { addDataRef(data); return *this; }
    MessageBuilder& MessageBuilder::addStringList(const std::vector<std::string>& data) { addDataRef(data); return *this; }

    MessageBuilder& MessageBuilder::addMessage(const Message& message) {
        _message->_data.insert(_message->_data.end(), message._data.begin(), message._data.end());
        return *this;
    }

    Ptr<Message> MessageBuilder::finish() { return _message; }
}

// const long long OFFSET = pow(2, 39) - 1;

// unsigned long long doubleToWeirdLongLong(double value)
// {
//     if (value < OFFSET + 2 && value > -OFFSET - 1) {
//         long long whole = value;
//         unsigned int fraction = round(abs(value - whole)*pow(10, 7));
//         unsigned long long whole_offset = whole + OFFSET;
//         unsigned long long composition = (whole_offset << 24) | (fraction & 0x00FFFFFF);
//         return composition;
//     }
    
//     cout << "Cannot convert!" << endl;
//     return 0;
// }

// double weirdLongLongToDouble(unsigned long long value) {
//     unsigned long long whole_offset = (value >> 24);
//     unsigned int fraction = (value & 0x00FFFFFF);
    
//     long long whole = whole_offset - OFFSET;
    
//     return whole + (fraction / pow(10, 7) *(whole > 0 ? 1 : -1));
// }