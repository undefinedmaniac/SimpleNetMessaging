#include "SimpleNetMessaging/DataType.h"

namespace snm
{
    DataType charToDataType(unsigned char character) {
        if ((character > DataType::String && character < DataType::BoolList) || character > DataType::StringList)
            return DataType::Invalid;

        return static_cast<DataType>(character);
    }
}