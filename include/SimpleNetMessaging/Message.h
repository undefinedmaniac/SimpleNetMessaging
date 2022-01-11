#pragma once

#include "SimpleNetMessaging/Types.h"
#include "SimpleNetMessaging/DataType.h"

#include <boost/variant.hpp>

namespace snm
{
// std::vector<unsigned char> messageToBytes(const IMessage& message);
// std::shared_ptr<IMessage> bytesToMessage(const std::vector<unsigned char>& bytes);
    class Message;
    using MessagePtr = Ptr<Message>;

    class Message
    {
        using Variant = boost::variant<bool, unsigned short, unsigned int, unsigned long long, 
            short, int, long long, double, char, std::string, Vector<bool>, Vector<unsigned short>, 
            Vector<unsigned int>, Vector<unsigned long long>, Vector<short>, Vector<int>, Vector<long long>,
            Vector<double>, Vector<char>, Vector<std::string>>;

        using MessageData = Pair<DataType, Variant>;
        using MessageDataPtr = Ptr<MessageData>;
    public:
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
        MessagePtr getSubmessage(unsigned int startIndex, unsigned int length) const;
        MessagePtr getLeft(unsigned int length) const;
        MessagePtr getRight(unsigned int length) const;

        class ContentIterator : public boost::static_visitor<>
        {
            unsigned int _index = 0;
            DataType _type = DataType::Null;
        protected:
            unsigned int index() const { return _index; }
            DataType type() const { return _type; }
        public:
            virtual ~ContentIterator() {}

            virtual void operator()(bool data) {}
            virtual void operator()(unsigned short data) {}
            virtual void operator()(unsigned int data) {}
            virtual void operator()(unsigned long long data) {}
            virtual void operator()(short data) {}
            virtual void operator()(int data) {}
            virtual void operator()(long long data) {}
            virtual void operator()(double data) {}
            virtual void operator()(char data) {}
            virtual void operator()(const std::string& data) {}

            virtual void operator()(const std::vector<bool>& data) {}
            virtual void operator()(const std::vector<unsigned short>& data) {}
            virtual void operator()(const std::vector<unsigned int>& data) {}
            virtual void operator()(const std::vector<unsigned long long>& data) {}
            virtual void operator()(const std::vector<short>& data) {}
            virtual void operator()(const std::vector<int>& data) {}
            virtual void operator()(const std::vector<long long>& data) {}
            virtual void operator()(const std::vector<double>& data) {}
            virtual void operator()(const std::vector<char>& data) {}
            virtual void operator()(const std::vector<std::string>& data) {}

            void update(unsigned int index, DataType type) { _index = index; _type = type; }
        };

        void iterate(ContentIterator& iterator) const;

        class Builder
        {
            template <typename T>
            void addData(T data);

            template <typename T>
            void addDataRef(const T& data);

            MessagePtr _message;
        public:
            Builder& start();

            Builder& addBool(bool data);
            Builder& addUShort(unsigned short data);
            Builder& addUInt(unsigned int data);
            Builder& addULong(unsigned long long data);
            Builder& addShort(short data);
            Builder& addInt(int data);
            Builder& addLong(long long data);
            Builder& addDouble(double data);
            Builder& addChar(char data);
            Builder& addString(const char data[]);
            Builder& addString(const std::string& data);

            Builder& addBoolList(const std::vector<bool>& data);
            Builder& addUShortList(const std::vector<unsigned short>& data);
            Builder& addUIntList(const std::vector<unsigned int>& data);
            Builder& addULongList(const std::vector<unsigned long long>& data);
            Builder& addShortList(const std::vector<short>& data);
            Builder& addIntList(const std::vector<int>& data);
            Builder& addLongList(const std::vector<long long>& data);
            Builder& addDoubleList(const std::vector<double>& data);
            Builder& addCharList(const std::vector<char>& data);
            Builder& addStringList(const std::vector<std::string>& data);

            Builder& addMessage(const Message& message);

            MessagePtr finish();
        };
    private:
        Message();

        Vector<MessageDataPtr> _data;
    };


}