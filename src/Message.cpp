#include "SimpleNetMessaging/Message.h"

#include <vector>

namespace snm
{
    Message::Message() {}

    // Check if the message is empty
    bool Message::isEmpty() const { return _data.empty(); }

    // Get the number of parts in the message
    unsigned int Message::size() const { return _data.size(); }

    // Get the type information for a part of the message
    DataType Message::getType(unsigned int index) const { return _data[index]->first; }

    MessagePtr Message::getSubmessage(unsigned int startIndex, unsigned int length) const {
        MessagePtr newMessage(new Message());
        newMessage->_data.insert(newMessage->_data.end(), _data.begin() + startIndex, _data.begin() + startIndex + length);
        return newMessage;
    }

    MessagePtr Message::getLeft(unsigned int length) const {
        return getSubmessage(0, length);
    }

    MessagePtr Message::getRight(unsigned int length) const {
        return getSubmessage(size() - length, length);
    }

    void Message::iterate(ContentIterator& iterator) const {
        for (unsigned int i = 0; i < size(); i++) {
            iterator.update(i, getType(i));
            boost::apply_visitor(iterator, std::get<1>(*_data[i]));
        }
    }

    typedef Message::Builder Builder;

    template <typename T>
    void Builder::addData(T data) {
        _message->_data.push_back(std::make_shared<Message::MessageData>(GetDataType<T>::value, data));
    }

    template <typename T>
    void Builder::addDataRef(const T& data) {
        _message->_data.push_back(std::make_shared<Message::MessageData>(GetDataType<T>::value, data));
    }

    Builder& Builder::start() { _message = MessagePtr(new Message()); return *this; }

    Builder& Builder::addBool(bool data) { addData(data); return *this; }
    Builder& Builder::addUShort(unsigned short data) { addData(data); return *this; }
    Builder& Builder::addUInt(unsigned int data) { addData(data); return *this; }
    Builder& Builder::addULong(unsigned long long data) { addData(data); return *this; }
    Builder& Builder::addShort(short data) { addData(data); return *this; }
    Builder& Builder::addInt(int data) { addData(data); return *this; }
    Builder& Builder::addLong(long long data) { addData(data); return *this; }
    Builder& Builder::addDouble(double data) { addData(data); return *this; }
    Builder& Builder::addChar(unsigned char data) { addData(data); return *this; }
    Builder& Builder::addString(const char data[]) { addDataRef(std::string(data)); return *this; }
    Builder& Builder::addString(const std::string& data) { addDataRef(data); return *this; }

    Builder& Builder::addBoolVector(const std::vector<bool>& data) { addDataRef(data); return *this; }
    Builder& Builder::addUShortVector(const std::vector<unsigned short>& data) { addDataRef(data); return *this; }
    Builder& Builder::addUIntVector(const std::vector<unsigned int>& data) { addDataRef(data); return *this; }
    Builder& Builder::addULongVector(const std::vector<unsigned long long>& data) { addDataRef(data); return *this; }
    Builder& Builder::addShortVector(const std::vector<short>& data) { addDataRef(data); return *this; }
    Builder& Builder::addIntVector(const std::vector<int>& data) { addDataRef(data); return *this; }
    Builder& Builder::addLongVector(const std::vector<long long>& data) { addDataRef(data); return *this; }
    Builder& Builder::addDoubleVector(const std::vector<double>& data) { addDataRef(data); return *this; }
    Builder& Builder::addCharVector(const std::vector<unsigned char>& data) { addDataRef(data); return *this; }
    Builder& Builder::addStringVector(const std::vector<std::string>& data) { addDataRef(data); return *this; }

    Builder& Builder::addMessage(const Message& message) {
        _message->_data.insert(_message->_data.end(), message._data.begin(), message._data.end());
        return *this;
    }

    MessagePtr Builder::finish() { return _message; }
}