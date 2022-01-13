#pragma once

#include "SimpleNetMessaging/Types.h"
#include "SimpleNetMessaging/Message.h"

#include <stdexcept>

namespace snm
{
    class SerializationError : public std::runtime_error
    {
    public:
        SerializationError(const std::string& errorMessage) : std::runtime_error(errorMessage) {}
    };

    Vector<unsigned char> serializeMessage(const Message& message);
    Vector<unsigned char> serializeMessage(const MessagePtr message);

    void deserializeBuild(Message::Builder& builder, const Vector<unsigned char>& messageData);
    MessagePtr deserializeMessage(const Vector<unsigned char>& messageData);
    MessagePtr deserializeMessage(Message::Builder& builder, const Vector<unsigned char>& messageData);
}