set(HEADER_LIST Message.h MessageSerializer.h DataType.h Variable.h VariableDisplay.h Types.h)
set(SOURCE_LIST Message.cpp MessageSerializer.cpp DataType.cpp Variable.cpp VariableDisplay.cpp)

set(INCLUDE_DIR ../include/SimpleNetMessaging/)
list(TRANSFORM HEADER_LIST PREPEND ${INCLUDE_DIR})
add_library(SimpleNetMessaging ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(SimpleNetMessaging PUBLIC ../include)
target_link_libraries(SimpleNetMessaging PUBLIC Boost::boost)