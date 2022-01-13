#include "SimpleNetMessaging/Message.h"
#include "SimpleNetMessaging/MessageSerializer.h"

#include <bitset>
#include <iostream>

int main(int argc, char const *argv[])
{
    using namespace snm;

    std::vector<std::string> strVector = {"a", "b", "cd"};
    std::vector<unsigned short> shortVector = {5, 1, 3, 8, 5, 6};

    Vector<std::string> stringVector = {"Hi", "There", "Garon"};

    Message::Builder messageBuilder;
    MessagePtr msg = messageBuilder.start().addBool(true).addChar('a').addDouble(1.5).addInt(10).addLong(10000)
        .addShort(5).addString("Hello Garon!").addUShortVector(shortVector).addStringVector(stringVector).finish();

    std::vector<unsigned char> bytes = serializeMessage(msg);
    MessagePtr msg2 = deserializeMessage(bytes);

    // for (unsigned char c : bytes) {
    //     std::bitset<8> set(c);
    //     std::cout << set << std::endl;
    // }

    // for (int i = 0; i < msg2->size(); i++)
    //     std::cout << msg2->getType(i) << std::endl;

    std::cout << msg2->getData<bool>(0) << std::endl;
    std::cout << msg2->getData<unsigned char>(1) << std::endl;
    std::cout << msg2->getData<double>(2) << std::endl;
    std::cout << msg2->getData<int>(3) << std::endl;
    std::cout << msg2->getData<long long>(4) << std::endl;
    std::cout << msg2->getData<short>(5) << std::endl;
    std::cout << msg2->getData<std::string>(6) << std::endl;

    const Vector<unsigned short>& shortVector2 = msg2->getData<Vector<unsigned short>>(7);

    const Vector<std::string>& stringVector2 = msg2->getData<Vector<std::string>>(8);

    for (auto s : stringVector2)
        std::cout << s << std::endl;

    return 0;
}
