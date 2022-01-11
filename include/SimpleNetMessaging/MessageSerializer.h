#pragma once

#include "SimpleNetMessaging/Types.h"
#include "SimpleNetMessaging/Message.h"

namespace snm
{
    class MessageSerializer : public snm::Message::ContentIterator
    {
        Vector<unsigned char> _binaryMessage;

        template <typename F, typename T>
        void write(F writeFunction, T data);
    public:
        void operator()(bool data) override;
        void operator()(unsigned short data) override;
        void operator()(unsigned int data) override;
        void operator()(unsigned long long data) override;
        void operator()(short data) override;
        void operator()(int data) override;
        void operator()(long long data) override;
        void operator()(double data) override;
        void operator()(char data) override;
        void operator()(const std::string& data) override;

        void operator()(const std::vector<bool>& data) override;
        void operator()(const std::vector<unsigned short>& data) override;
        void operator()(const std::vector<unsigned int>& data) override;
        void operator()(const std::vector<unsigned long long>& data) override;
        void operator()(const std::vector<short>& data) override;
        void operator()(const std::vector<int>& data) override;
        void operator()(const std::vector<long long>& data) override;
        void operator()(const std::vector<double>& data) override;
        void operator()(const std::vector<char>& data) override;
        void operator()(const std::vector<std::string>& data) override;

        Vector<unsigned char> serialize(const Message& message);
    };
}