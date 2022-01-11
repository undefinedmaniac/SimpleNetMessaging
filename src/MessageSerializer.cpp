#include "SimpleNetMessaging/MessageSerializer.h"

#include <boost/endian.hpp>

namespace snm
{
    template <typename F, typename T>
    void MessageSerializer::write(F writeFunction, T data) {
        _binaryMessage[index()] = type();
        std::size_t size = _binaryMessage.size();
        _binaryMessage.resize(size + sizeof(T));
        writeFunction(_binaryMessage.data() + size, data);
    }

    void MessageSerializer::operator()(bool data) { write([](unsigned char* writePos, unsigned char data){ *writePos = data; }, static_cast<unsigned char>(data)); }
    void MessageSerializer::operator()(unsigned short data) { write(boost::endian::store_big_u16, static_cast<uint16_t>(data)); }
    void MessageSerializer::operator()(unsigned int data) { write(boost::endian::store_big_u32, static_cast<uint32_t>(data)); }
    void MessageSerializer::operator()(unsigned long long data) { write(boost::endian::store_big_u64, static_cast<uint64_t>(data)); }
    void MessageSerializer::operator()(short data) { write(boost::endian::store_big_s16, static_cast<int16_t>(data)); }
    void MessageSerializer::operator()(int data) { write(boost::endian::store_big_s32, static_cast<int32_t>(data)); }
    void MessageSerializer::operator()(long long data) { write(boost::endian::store_big_s64, static_cast<int64_t>(data)); }
    void MessageSerializer::operator()(double data) {
        
    }
    void MessageSerializer::operator()(char data) { write([](unsigned char* writePos, unsigned char data){ *writePos = data; }, static_cast<unsigned char>(data)); }
    void MessageSerializer::operator()(const std::string& data) {
        
    }

    void MessageSerializer::operator()(const std::vector<bool>& data) {

    }
    void MessageSerializer::operator()(const std::vector<unsigned short>& data) {

    }
    void MessageSerializer::operator()(const std::vector<unsigned int>& data) {

    }
    void MessageSerializer::operator()(const std::vector<unsigned long long>& data) {

    }
    void MessageSerializer::operator()(const std::vector<short>& data) {

    }
    void MessageSerializer::operator()(const std::vector<int>& data) {

    }
    void MessageSerializer::operator()(const std::vector<long long>& data) {

    }
    void MessageSerializer::operator()(const std::vector<double>& data) {

    }
    void MessageSerializer::operator()(const std::vector<char>& data) {

    }
    void MessageSerializer::operator()(const std::vector<std::string>& data) {

    }

    Vector<unsigned char> MessageSerializer::serialize(const Message& message) {
        _binaryMessage.clear();
        _binaryMessage.resize(message.size() + 1);
        message.iterate(*this);
        return std::move(_binaryMessage);
    }
}