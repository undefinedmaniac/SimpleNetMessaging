#include "SimpleNetMessaging/MessageSerializer.h"

#include <numeric>
#include <boost/endian.hpp>

namespace snm
{
    static_assert(std::is_same<unsigned short, uint16_t>::value, "Serialization will not work correctly due to type mismatch");
    static_assert(std::is_same<unsigned int, uint32_t>::value, "Serialization will not work correctly due to type mismatch");
    static_assert(std::is_same<unsigned long long, uint64_t>::value, "Serialization will not work correctly due to type mismatch");
    static_assert(std::is_same<short, int16_t>::value, "Serialization will not work correctly due to type mismatch");
    static_assert(std::is_same<int, int32_t>::value, "Serialization will not work correctly due to type mismatch");
    static_assert(std::is_same<long long, int64_t>::value, "Serialization will not work correctly due to type mismatch");
    static_assert(sizeof(double) == sizeof(uint64_t), "Serialization will not work correctly due to type mismatch");

    void writeChar(unsigned char* writePos, unsigned char value) { *writePos = value; }
    void writeString(unsigned char* writePos, const std::string& value) { 
        // Check if string is too long to be serialized with our format
        if (value.length() > std::numeric_limits<uint16_t>::max())
            throw SerializationError("String is over " + std::to_string(std::numeric_limits<uint16_t>::max()) + " characters long and cannot be serialized!");

        // Store the length of the string
        boost::endian::store_big_u16(writePos, value.length());
        writePos += sizeof(uint16_t);

        // Store the string content
        std::copy(value.begin(), value.end(), writePos);
    }

    template <typename T, std::size_t S = sizeof(T)>
    std::size_t constantSize(T input) { return S; }

    class MessageSerializer : public snm::Message::ContentIterator
    {
        Vector<unsigned char> _binaryMessage;

        template <typename F, typename T>
        void write(F writeFunction, const T& data, std::size_t dataSize = sizeof(T));

        template <typename F, typename T, typename S>
        void write(F writeFunction, const Vector<T>& vector, S sizeFunction);

        void write(const Vector<bool>& vector);
    public:
        void operator()(bool data) override;
        void operator()(unsigned short data) override;
        void operator()(unsigned int data) override;
        void operator()(unsigned long long data) override;
        void operator()(short data) override;
        void operator()(int data) override;
        void operator()(long long data) override;
        void operator()(double data) override;
        void operator()(unsigned char data) override;
        void operator()(const std::string& data) override;

        void operator()(const std::vector<bool>& data) override;
        void operator()(const std::vector<unsigned short>& data) override;
        void operator()(const std::vector<unsigned int>& data) override;
        void operator()(const std::vector<unsigned long long>& data) override;
        void operator()(const std::vector<short>& data) override;
        void operator()(const std::vector<int>& data) override;
        void operator()(const std::vector<long long>& data) override;
        void operator()(const std::vector<double>& data) override;
        void operator()(const std::vector<unsigned char>& data) override;
        void operator()(const std::vector<std::string>& data) override;

        Vector<unsigned char> serialize(const Message& message);
    };

    template <typename F, typename T>
    void MessageSerializer::write(F writeFunction, const T& data, std::size_t dataSize) {
        // Set the typedata byte for this content
        _binaryMessage[index()] = type();

        // Resize the message and write in the new data
        std::size_t size = _binaryMessage.size();
        _binaryMessage.resize(size + dataSize);
        writeFunction(_binaryMessage.data() + size, data);
    }

    template <typename F, typename T, typename S>
    void MessageSerializer::write(F writeFunction, const Vector<T>& vector, S sizeFunction) {
        // Set the typedata byte for this content
        _binaryMessage[index()] = type();

        // Check if vector is too long to be serialized with our format
        if (vector.size() > std::numeric_limits<uint16_t>::max())
            throw SerializationError("Vector is over " + std::to_string(std::numeric_limits<uint16_t>::max()) + " elements long and cannot be serialized!");

        // Resize the message and write in the new data
        std::size_t size = _binaryMessage.size();
        _binaryMessage.resize(std::accumulate(vector.begin(), vector.end(), size + sizeof(uint16_t), 
            [&sizeFunction](std::size_t a, const T& b){ return a + sizeFunction(b); }));

        unsigned char* writePos = _binaryMessage.data() + size;

        // Store the size of the vector as a 16 bit unsigned value
        boost::endian::store_big_u16(writePos, vector.size());
        writePos += sizeof(uint16_t);

        // Write all the data from the vector into the message
        for (const T& data : vector) {
            writeFunction(writePos, data);
            writePos += sizeFunction(data);
        }
    }

    void MessageSerializer::write(const Vector<bool>& vector) {
        // Set the typedata byte for this content
        _binaryMessage[index()] = type();

        // Resize the message and write in the new data
        const std::size_t BITS_PER_BYTE = 8;

        std::size_t size = _binaryMessage.size();
        std::size_t contentBytesNeeded = ((vector.size() + BITS_PER_BYTE - 1) / BITS_PER_BYTE);
        _binaryMessage.resize(size + sizeof(uint16_t) + contentBytesNeeded);

        unsigned char* writePos = _binaryMessage.data() + size;

        // Store the size of the vector as a 16 bit unsigned value
        boost::endian::store_big_u16(writePos, vector.size());
        writePos += sizeof(uint16_t);

        // Write all the data from the vector into the message
        unsigned short bitPos = 0;
        for (bool data : vector) {
            if (bitPos >= BITS_PER_BYTE) {
                bitPos = 0;
                writePos++;
            }

            if (data)
                *writePos |= (0x80 >> bitPos);

            bitPos++;
        }
    }

    void MessageSerializer::operator()(bool data) { write(writeChar, static_cast<unsigned char>(data)); }
    void MessageSerializer::operator()(unsigned short data) { write(boost::endian::store_big_u16, static_cast<uint16_t>(data)); }
    void MessageSerializer::operator()(unsigned int data) { write(boost::endian::store_big_u32, static_cast<uint32_t>(data)); }
    void MessageSerializer::operator()(unsigned long long data) { write(boost::endian::store_big_u64, static_cast<uint64_t>(data)); }
    void MessageSerializer::operator()(short data) { write(boost::endian::store_big_s16, static_cast<int16_t>(data)); }
    void MessageSerializer::operator()(int data) { write(boost::endian::store_big_s32, static_cast<int32_t>(data)); }
    void MessageSerializer::operator()(long long data) { write(boost::endian::store_big_s64, static_cast<int64_t>(data)); }
    void MessageSerializer::operator()(double data) { write(boost::endian::store_big_u64, reinterpret_cast<uint64_t&>(data)); }
    void MessageSerializer::operator()(unsigned char data) { write(writeChar, data); }
    void MessageSerializer::operator()(const std::string& data) { write(writeString, data, sizeof(uint16_t) + data.length()); }

    void MessageSerializer::operator()(const std::vector<bool>& data) { write(data); }
    void MessageSerializer::operator()(const std::vector<unsigned short>& data) { write(boost::endian::store_big_u16, data, constantSize<uint16_t>); }
    void MessageSerializer::operator()(const std::vector<unsigned int>& data) { write(boost::endian::store_big_u32, data, constantSize<uint32_t>); }
    void MessageSerializer::operator()(const std::vector<unsigned long long>& data) { write(boost::endian::store_big_u64, data, constantSize<uint64_t>); }
    void MessageSerializer::operator()(const std::vector<short>& data) { write(boost::endian::store_big_s16, data, constantSize<int16_t>); }
    void MessageSerializer::operator()(const std::vector<int>& data) { write(boost::endian::store_big_s32, data, constantSize<int32_t>); }
    void MessageSerializer::operator()(const std::vector<long long>& data) { write(boost::endian::store_big_s64, data, constantSize<int64_t>); }
    void MessageSerializer::operator()(const std::vector<double>& data) {
        write([](unsigned char* writePos, double value)
            { boost::endian::store_big_u64(writePos, reinterpret_cast<uint64_t&>(value)); }, data, constantSize<uint64_t>);
    }
    void MessageSerializer::operator()(const std::vector<unsigned char>& data) { write(writeChar, data, constantSize<unsigned char>); }
    void MessageSerializer::operator()(const std::vector<std::string>& data) { 
        write(writeString, data, [](const std::string& str){ return sizeof(uint16_t) + str.length(); }); 
    }

    Vector<unsigned char> MessageSerializer::serialize(const Message& message) {
        _binaryMessage.clear();
        _binaryMessage.resize(message.size() + 1);
        message.iterate(*this);
        return _binaryMessage;
    }

    Vector<unsigned char> serializeMessage(const Message& message) {
        MessageSerializer serializer;
        return serializer.serialize(message);
    }

    unsigned char readChar(const unsigned char* readPtr) { return *readPtr; }

    double readDouble(const unsigned char* readPtr)
    { 
        uint64_t data = boost::endian::load_big_u64(readPtr);
        return reinterpret_cast<double&>(data);
    }

    template <typename T, typename F>
    T deserialize(const unsigned char*& readPtr, const unsigned char* messageEnd, F readFunction, std::size_t readSize = sizeof(T)) {
        if (messageEnd - readPtr < readSize)
            throw SerializationError("Invalid content data");

        T value = readFunction(readPtr);
        readPtr += readSize;
        return value;
    }

    using namespace boost::endian;
    using namespace std::placeholders;
    auto deserializeBool = std::bind(deserialize<bool, decltype(readChar)>, _1, _2, readChar, sizeof(char));
    auto deserializeUShort = std::bind(deserialize<unsigned short, decltype(load_big_u16)>, _1, _2, load_big_u16, sizeof(uint16_t));
    auto deserializeUInt = std::bind(deserialize<unsigned int, decltype(load_big_u32)>, _1, _2, load_big_u32, sizeof(uint32_t));
    auto deserializeULong = std::bind(deserialize<unsigned long long, decltype(load_big_u64)>, _1, _2, load_big_u64, sizeof(uint64_t));
    auto deserializeShort = std::bind(deserialize<short, decltype(load_big_s16)>, _1, _2, load_big_s16, sizeof(int16_t));
    auto deserializeInt = std::bind(deserialize<int, decltype(load_big_s32)>, _1, _2, load_big_s32, sizeof(int32_t));
    auto deserializeLong = std::bind(deserialize<long long, decltype(load_big_s64)>, _1, _2, load_big_s64, sizeof(int64_t));
    auto deserializeDouble = std::bind(deserialize<double, decltype(readDouble)>, _1, _2, readDouble, sizeof(double));
    auto deserializeChar = std::bind(deserialize<unsigned char, decltype(readChar)>, _1, _2, readChar, sizeof(char));

    std::string deserializeString(const unsigned char*& readPtr, const unsigned char* messageEnd) {
        unsigned short stringSize = deserializeUShort(readPtr, messageEnd);
        return deserialize<std::string>(readPtr, messageEnd, 
            [stringSize](const unsigned char* readPtr){ return std::string(reinterpret_cast<const char*>(readPtr), stringSize); }, stringSize);
    }

    template <typename T, typename F>
    Vector<T> deserializeVector(const unsigned char*& readPtr, const unsigned char* messageEnd, F readFunction) {
        unsigned short vectorSize = deserializeUShort(readPtr, messageEnd);

        Vector<T> vector;
        vector.reserve(vectorSize);
        for (unsigned short i = 0; i < vectorSize; i++)
            vector.push_back(readFunction(readPtr, messageEnd));

        return vector;
    }

    Vector<bool> deserializeBoolVector(const unsigned char*& readPtr, const unsigned char* messageEnd) {
        const std::size_t BITS_PER_BYTE = 8;

        unsigned short vectorSize = deserializeUShort(readPtr, messageEnd);

        Vector<bool> vector;
        vector.reserve(vectorSize);
        for (unsigned short i = 0; i < vectorSize; i++)
            vector.push_back(*(readPtr + i / BITS_PER_BYTE) & (0x80 >> i % 8));

        readPtr += ((vectorSize + BITS_PER_BYTE - 1) / BITS_PER_BYTE);

        return vector;
    }

    void deserializeBuild(Message::Builder& builder, const Vector<unsigned char>& messageData) {
        // Do nothing for an empty message
        if (messageData.empty())
            return;

        const unsigned char* readPtr = messageData.data();
        const unsigned char* messageEnd = readPtr + messageData.size();

        // Parse the type data out of the message data
        Vector<DataType> typeData;
        {
            DataType type;
            while ((type = charToDataType(*(readPtr++))) != DataType::Null) {
                if (type == DataType::Invalid)
                    throw SerializationError("Invalid type data");

                typeData.push_back(type);

                if (readPtr == messageEnd)
                    throw SerializationError("Invalid type data");
            }
        }

        // Parse each data type and insert it into the builder
        for (DataType type : typeData) {
            switch (type) {
                case Bool:
                    builder.addBool(deserializeBool(readPtr, messageEnd));
                    break;
                case UShort:
                    builder.addUShort(deserializeUShort(readPtr, messageEnd));
                    break;
                case UInt:
                    builder.addUInt(deserializeUInt(readPtr, messageEnd));
                    break;
                case ULong:
                    builder.addULong(deserializeULong(readPtr, messageEnd));
                    break;
                case Short:
                    builder.addShort(deserializeShort(readPtr, messageEnd));
                    break;
                case Int:
                    builder.addInt(deserializeInt(readPtr, messageEnd));
                    break;
                case Long:
                    builder.addLong(deserializeLong(readPtr, messageEnd));
                    break;
                case Double:
                    builder.addDouble(deserializeDouble(readPtr, messageEnd));
                    break;
                case Char:
                    builder.addChar(deserializeChar(readPtr, messageEnd));
                    break;
                case String:
                    builder.addString(deserializeString(readPtr, messageEnd));
                    break;
                case BoolList:
                    builder.addBoolVector(deserializeBoolVector(readPtr, messageEnd));
                    break;
                case UShortList:
                    builder.addUShortVector(deserializeVector<unsigned short>(readPtr, messageEnd, deserializeUShort));
                    break;
                case UIntList:
                    builder.addUIntVector(deserializeVector<unsigned int>(readPtr, messageEnd, deserializeUInt));
                    break;
                case ULongList:
                    builder.addULongVector(deserializeVector<unsigned long long>(readPtr, messageEnd, deserializeULong));
                    break;
                case ShortList:
                    builder.addShortVector(deserializeVector<short>(readPtr, messageEnd, deserializeShort));
                    break;
                case IntList:
                    builder.addIntVector(deserializeVector<int>(readPtr, messageEnd, deserializeInt));
                    break;
                case LongList:
                    builder.addLongVector(deserializeVector<long long>(readPtr, messageEnd, deserializeLong));
                    break;
                case DoubleList:
                    builder.addDoubleVector(deserializeVector<double>(readPtr, messageEnd, deserializeDouble));
                    break;
                case CharList:
                    builder.addCharVector(deserializeVector<unsigned char>(readPtr, messageEnd, deserializeChar));
                    break;
                case StringList:
                    builder.addStringVector(deserializeVector<std::string>(readPtr, messageEnd, deserializeString));
                    break;
            }
        }
    }

    Message deserializeMessage(const Vector<unsigned char>& messageData) {
        Message::Builder builder;
        return deserializeMessage(builder, messageData);
    }
    Message deserializeMessage(Message::Builder& builder, const Vector<unsigned char>& messageData) {
        deserializeBuild(builder.start(), messageData);
        return builder.finish();
    }
}