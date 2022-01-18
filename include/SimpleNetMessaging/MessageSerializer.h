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

    void deserializeBuild(Message::Builder& builder, const Vector<unsigned char>& messageData);
    Message deserializeMessage(const Vector<unsigned char>& messageData);
    Message deserializeMessage(Message::Builder& builder, const Vector<unsigned char>& messageData);
}