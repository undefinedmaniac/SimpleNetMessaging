#include "SimpleNetMessaging/Message.h"
#include "SimpleNetMessaging/MessageSerializer.h"

#include "SimpleNetMessaging/Variable.h"
#include "SimpleNetMessaging/VariableDisplay.h"

#include <bitset>
#include <iostream>

int main(int argc, char const *argv[])
{
    using namespace snm;

    // std::vector<std::string> strVector = {"a", "b", "cd"};
    // std::vector<unsigned short> shortVector = {5, 1, 3, 8, 5, 6};
    // Vector<bool> boolVector = {true, true, false, false, true, true, true, false, false, false, true, true};

    // Vector<std::string> stringVector = {"Hi", "There", "Garon"};

    // Message::Builder messageBuilder;
    // MessagePtr msg = messageBuilder.start().addBool(true).addChar('a').addDouble(1.5).addInt(10).addLong(10000)
    //     .addShort(5).addString("Hello Garon!").addUShortVector(shortVector).addStringVector(stringVector).addBoolVector(boolVector).finish();

    // std::vector<unsigned char> bytes = serializeMessage(msg);
    // MessagePtr msg2 = deserializeMessage(bytes);

    // // for (unsigned char c : bytes) {
    // //     std::bitset<8> set(c);
    // //     std::cout << set << std::endl;
    // // }

    // // for (int i = 0; i < msg2->size(); i++)
    // //     std::cout << msg2->getType(i) << std::endl;

    // std::cout << msg2->getData<bool>(0) << std::endl;
    // std::cout << msg2->getData<unsigned char>(1) << std::endl;
    // std::cout << msg2->getData<double>(2) << std::endl;
    // std::cout << msg2->getData<int>(3) << std::endl;
    // std::cout << msg2->getData<long long>(4) << std::endl;
    // std::cout << msg2->getData<short>(5) << std::endl;
    // std::cout << msg2->getData<std::string>(6) << std::endl;

    // const Vector<unsigned short>& shortVector2 = msg2->getData<Vector<unsigned short>>(7);

    // const Vector<std::string>& stringVector2 = msg2->getData<Vector<std::string>>(8);

    // const Vector<bool>& boolVector2 = msg2->getData<Vector<bool>>(9);

    // std::cout << boolVector2.size() << std::endl;

    // for (auto s : boolVector2)
    //     std::cout << s << std::endl;

    struct Observer : public VariableObserver
    {
        void updated(VariablePtr var) override {
            std::cout << "VO: " << var->getValue<int>() << std::endl;
        }
    };

    struct Observer2 : public RemoteVariableObserver
    {
        void updated(RemoteVariablePtr var) override {
            std::cout << "RVO: " << var->getValue<int>() << std::endl;
        }
    };

    int myInt = 15;
    int& myIntRef = myInt;

    Observer ob;
    Observer2 ob2;

    VariablePtr rv = RemoteVariable::create(10, true, &ob2);
    rv->addObserver(ob);

    std::cout << rv->getValue<int>() << std::endl;
    std::cout << rv->isWritable() << std::endl;

    rv->setValue(15);

    std::cout << rv->getValue<int>() << std::endl;

    return 0;
}
