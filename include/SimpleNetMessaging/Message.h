#pragma once

#include "SimpleNetMessaging/Types.h"
#include "SimpleNetMessaging/DataType.h"

#include <boost/variant.hpp>

namespace snm
{
    class Message;
    using MessagePtr = Ptr<Message>;

    class Message
    {
        using Variant = boost::variant<bool, unsigned short, unsigned int, unsigned long long, 
            short, int, long long, double, unsigned char, std::string, Vector<bool>, Vector<unsigned short>, 
            Vector<unsigned int>, Vector<unsigned long long>, Vector<short>, Vector<int>, Vector<long long>,
            Vector<double>, Vector<unsigned char>, Vector<std::string>>;

        using MessageData = Pair<DataType, Variant>;
        using MessageDataPtr = Ptr<MessageData>;

        Message();

        Vector<MessageDataPtr> _data;
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
            virtual void operator()(unsigned char data) {}
            virtual void operator()(const std::string& data) {}

            virtual void operator()(const Vector<bool>& data) {}
            virtual void operator()(const Vector<unsigned short>& data) {}
            virtual void operator()(const Vector<unsigned int>& data) {}
            virtual void operator()(const Vector<unsigned long long>& data) {}
            virtual void operator()(const Vector<short>& data) {}
            virtual void operator()(const Vector<int>& data) {}
            virtual void operator()(const Vector<long long>& data) {}
            virtual void operator()(const Vector<double>& data) {}
            virtual void operator()(const Vector<unsigned char>& data) {}
            virtual void operator()(const Vector<std::string>& data) {}

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
            Builder& addChar(unsigned char data);
            Builder& addString(const char data[]);
            Builder& addString(const std::string& data);

            Builder& addBoolVector(const Vector<bool>& data);
            Builder& addUShortVector(const Vector<unsigned short>& data);
            Builder& addUIntVector(const Vector<unsigned int>& data);
            Builder& addULongVector(const Vector<unsigned long long>& data);
            Builder& addShortVector(const Vector<short>& data);
            Builder& addIntVector(const Vector<int>& data);
            Builder& addLongVector(const Vector<long long>& data);
            Builder& addDoubleVector(const Vector<double>& data);
            Builder& addCharVector(const Vector<unsigned char>& data);
            Builder& addStringVector(const Vector<std::string>& data);

            Builder& addMessage(const Message& message);

            MessagePtr finish();
        };
    };
}