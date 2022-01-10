#include "SimpleNetMessaging/Message.h"

#include <iostream>
#include <bitset>
#include <vector>

#include <boost/variant.hpp>

int main(int argc, char const *argv[])
{
    // bool myBool = true;
    // unsigned short myUShort = 5;
    // unsigned int myUInt = 100001;
    // unsigned long long myULong = 100000000000001;
    // short myShort = -5;
    // int myInt = -100001;
    // long long myLong = -100000000000001;
    // double myDouble = 1.5;
    // char myChar = 'a';
    // std::string myString = "Hi Tanner";

    // snm::MessageBuilder builder;
    
    // auto message;

    // std::vector<unsigned char> data = snm::messageToBytes(*message);

    // for (auto c : data) {
    //     std::bitset<8> set(c);
    //     std::cout << set << std::endl;
    // }

    // std::vector<int> myInts = {1, 2, 3, 4, 5};

    snm::MessageBuilder builder;

    snm::Ptr<snm::Message> message1 = builder.start().addString("Garon").addString(" Is").finish();
    snm::Ptr<snm::Message> message2 = builder.start().addString(" A").addString(" Great").finish();
    snm::Ptr<snm::Message> message3 = builder.start().addString(" Friend").finish();

    snm::Ptr<snm::Message> finalMessage = builder.start().addMessage(*message1).addMessage(*message2).addMessage(*message3).addString(". Don't").addString(" You think?").finish();
    
    snm::Ptr<snm::Message> subMessage = finalMessage->getSubmessage(2, 4);

    std::cout << subMessage->size() << std::endl;

    for (int i = 0; i < subMessage->size(); i++)
        std::cout << subMessage->getData<std::string>(i);

    std::cout << std::endl;

    // std::vector<int> one = {1, 2, 3, 4};
    // std::vector<int> two = {5, 6, 7, 8};

    // one.insert(one.end(), two.begin(), two.end());

    // for (int i : one)
    //     std::cout << i << std::endl;

    return 0;
}
