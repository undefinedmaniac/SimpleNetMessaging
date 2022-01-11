#include "SimpleNetMessaging/Message.h"

#include <iostream>

int main(int argc, char const *argv[])
{
    using namespace snm;

    Message::Builder messageBuilder;
    MessagePtr msg = messageBuilder.start().addBool(true).addString("Hi").addInt(10).addString("Another String").finish();

    return 0;
}
