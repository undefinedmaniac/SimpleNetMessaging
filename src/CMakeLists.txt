set(HEADER_LIST "${SimpleNetMessaging_SOURCE_DIR}/include/SimpleNetMessaging/Message.h" "${SimpleNetMessaging_SOURCE_DIR}/include/SimpleNetMessaging/DataType.h" "${SimpleNetMessaging_SOURCE_DIR}/include/SimpleNetMessaging/Types.h" "${SimpleNetMessaging_SOURCE_DIR}/include/SimpleNetMessaging/MessageSerializer.h")

add_library(simple_net_messaging Message.cpp MessageSerializer.cpp ${HEADER_LIST})
target_include_directories(simple_net_messaging PUBLIC ../include)
target_link_libraries(simple_net_messaging PUBLIC Boost::boost)