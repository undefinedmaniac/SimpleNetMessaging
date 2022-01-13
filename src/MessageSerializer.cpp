#include "SimpleNetMessaging/MessageSerializer.h"

#include <numeric>
#include <boost/endian.hpp>

namespace snm
{
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
        static_assert(FitsInto<unsigned short, uint16_t>::value, "The unsigned short type cannot fit into an uint16_t!");
        static_assert(FitsInto<unsigned int, uint32_t>::value, "The unsigned int type cannot fit into an uint32_t!");
        static_assert(FitsInto<unsigned long long, uint64_t>::value, "The unsigned long long type cannot fit into an uint64_t!");
        static_assert(FitsInto<short, int16_t>::value, "The short type cannot fit into an int16_t!");
        static_assert(FitsInto<int, int32_t>::value, "The int type cannot fit into an int32_t!");
        static_assert(FitsInto<long long, int64_t>::value, "The long long type cannot fit into an int64_t!");
        static_assert(sizeof(double) == 8, "Serialize will not work correctly without 8 byte doubles");

        _binaryMessage.clear();
        _binaryMessage.resize(message.size() + 1);
        message.iterate(*this);
        return std::move(_binaryMessage);
    }

    Vector<unsigned char> serializeMessage(const Message& message) {
        static MessageSerializer serializer;
        return std::move(serializer.serialize(message));
    }

    Vector<unsigned char> serializeMessage(const MessagePtr message) {
        return std::move(serializeMessage(*message));
    }

    template <typename T>
    T deserialize(std::size_t messageSize, const unsigned char*& readPtr, std::size_t& readBytes, std::function<T(const unsigned char*)> readFunction) {
        if (messageSize - readBytes < sizeof(T))
            throw SerializationError("Invalid content data");

        T value = readFunction(readPtr);
        readPtr += sizeof(T);
        readBytes += sizeof(T);
        return value;
    }

    std::string deserialize(std::size_t messageSize, const unsigned char*& readPtr, std::size_t& readBytes) {
        unsigned short stringSize = deserialize<uint16_t>(messageSize, readPtr, readBytes, boost::endian::load_big_u16);
        if (messageSize - readBytes < stringSize)
            throw SerializationError("Invalid content data");

        std::string str = std::string(reinterpret_cast<const char*>(readPtr), stringSize);
        readPtr += stringSize;
        readBytes += stringSize;
        return std::move(str);
    }

    template <typename T, typename F>
    Vector<T> deserializeVector(std::size_t messageSize, const unsigned char*& readPtr, std::size_t& readBytes, F readFunction) {
        unsigned short vectorSize = deserialize<uint16_t>(messageSize, readPtr, readBytes, boost::endian::load_big_u16);

        Vector<T> vector;
        vector.reserve(vectorSize);
        for (unsigned short i = 0; i < vectorSize; i++)
            vector.push_back(readFunction(messageSize, readPtr, readBytes));

        return std::move(vector);
    }

    void deserializeBuild(Message::Builder& builder, const Vector<unsigned char>& messageData) {
        static_assert(FitsInto<uint16_t, unsigned short>::value, "The uint16_t type cannot fit into an unsigned short!");
        static_assert(FitsInto<uint32_t, unsigned int>::value, "The uint32_t type cannot fit into an unsigned int!");
        static_assert(FitsInto<uint64_t, unsigned long long>::value, "The uint64_t type cannot fit into an unsigned long long!");
        static_assert(FitsInto<int16_t, short>::value, "The int16_t type cannot fit into a short!");
        static_assert(FitsInto<int32_t, int>::value, "The int32_t type cannot fit into an int!");
        static_assert(FitsInto<int64_t, long long>::value, "The int64_t type cannot fit into a long long!");
        static_assert(sizeof(double) == 8, "Deserialize will not work correctly without 8 byte doubles");

        // Do nothing for an empty message
        if (messageData.empty())
            return;

        const std::size_t messageSize = messageData.size();
        const unsigned char* readPtr = messageData.data();
        std::size_t readBytes = 0;

        // Parse the type data out of the message data
        Vector<DataType> typeData;
        {
            DataType type;
            while ((type = charToDataType(*(readPtr++))) != DataType::Null) {
                if (type == DataType::Invalid)
                    throw SerializationError("Invalid type data");

                typeData.push_back(type);

                if (++readBytes >= messageSize)
                    throw SerializationError("Invalid type data");
            }
        }

        // Parse each data type and insert it into the builder
        for (DataType type : typeData) {
            switch (type) {
                using namespace std::placeholders;
                case Bool: {
                    builder.addBool(deserialize<char>(messageSize, readPtr, readBytes, [](const unsigned char* readPtr){ return *readPtr; }));
                    break;
                }
                case UShort: {
                    builder.addUShort(deserialize<uint16_t>(messageSize, readPtr, readBytes, boost::endian::load_big_u16));
                    break;
                }
                case UInt: {
                    builder.addUInt(deserialize<uint32_t>(messageSize, readPtr, readBytes, boost::endian::load_big_u32));
                    break;
                }
                case ULong: {
                    builder.addULong(deserialize<uint64_t>(messageSize, readPtr, readBytes, boost::endian::load_big_u64));
                    break;
                }
                case Short: {
                    builder.addShort(deserialize<int16_t>(messageSize, readPtr, readBytes, boost::endian::load_big_s16));
                    break;
                }
                case Int: {
                    builder.addInt(deserialize<int32_t>(messageSize, readPtr, readBytes, boost::endian::load_big_s32));
                    break;
                }
                case Long: {
                    builder.addLong(deserialize<int64_t>(messageSize, readPtr, readBytes, boost::endian::load_big_s64));
                    break;
                }
                case Double: {
                    uint64_t data = deserialize<uint64_t>(messageSize, readPtr, readBytes, boost::endian::load_big_u64);
                    builder.addDouble(reinterpret_cast<double&>(data));
                    break;
                }
                case Char: {
                    builder.addChar(deserialize<unsigned char>(messageSize, readPtr, readBytes, [](const unsigned char* readPtr){ return *readPtr; }));
                    break;
                }
                case String: {
                    builder.addString(deserialize(messageSize, readPtr, readBytes));
                    break;
                }
                case BoolList: {
                    break;
                }
                case UShortList: {
                    builder.addUShortVector(deserializeVector<unsigned short>(messageSize, readPtr, readBytes, 
                        std::bind(deserialize<uint16_t>, _1, _2, _3, boost::endian::load_big_u16)));
                    break;
                }
                case UIntList: {
                    builder.addUIntVector(deserializeVector<unsigned int>(messageSize, readPtr, readBytes, 
                        std::bind(deserialize<uint32_t>, _1, _2, _3, boost::endian::load_big_u32)));
                    break;
                }
                case ULongList: {
                    builder.addULongVector(deserializeVector<unsigned long long>(messageSize, readPtr, readBytes, 
                        std::bind(deserialize<uint64_t>, _1, _2, _3, boost::endian::load_big_u64)));
                    break;
                }
                case ShortList: {
                    builder.addShortVector(deserializeVector<short>(messageSize, readPtr, readBytes, 
                        std::bind(deserialize<int16_t>, _1, _2, _3, boost::endian::load_big_s16)));
                    break;
                }
                case IntList: {
                    builder.addIntVector(deserializeVector<int>(messageSize, readPtr, readBytes, 
                        std::bind(deserialize<int32_t>, _1, _2, _3, boost::endian::load_big_s32)));
                    break;
                }
                case LongList: {
                    builder.addLongVector(deserializeVector<long long>(messageSize, readPtr, readBytes, 
                        std::bind(deserialize<int64_t>, _1, _2, _3, boost::endian::load_big_s64)));
                    break;
                }
                case DoubleList: {
                    builder.addDoubleVector(deserializeVector<double>(messageSize, readPtr, readBytes, 
                        [](std::size_t messageSize, const unsigned char*& readPtr, std::size_t& readBytes)
                        { 
                            uint64_t data = deserialize<uint64_t>(messageSize, readPtr, readBytes, boost::endian::load_big_u64);
                            return reinterpret_cast<double&>(data);
                        }));
                    break;
                }
                case CharList: {
                    builder.addCharVector(deserializeVector<unsigned char>(messageSize, readPtr, readBytes, 
                        std::bind(deserialize<unsigned char>, _1, _2, _3, [](const unsigned char* readPtr){ return *readPtr; })));
                    break;
                }
                case StringList: {
                    std::string (*functionPtr)(std::size_t, const unsigned char*&, std::size_t&) = &deserialize;
                    builder.addStringVector(deserializeVector<std::string>(messageSize, readPtr, readBytes, functionPtr));
                    break;
                }
            }
        }
    }

    MessagePtr deserializeMessage(const Vector<unsigned char>& messageData) {
        static Message::Builder builder;
        return deserializeMessage(builder, messageData);
    }
    MessagePtr deserializeMessage(Message::Builder& builder, const Vector<unsigned char>& messageData) {
        deserializeBuild(builder.start(), messageData);
        return builder.finish();
    }
}