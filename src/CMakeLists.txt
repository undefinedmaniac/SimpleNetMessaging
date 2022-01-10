set(HEADER_LIST "${SimpleNetMessaging_SOURCE_DIR}/include/SimpleNetMessaging/Message.h")

add_library(simple_net_messaging Message.cpp ${HEADER_LIST})
target_include_directories(simple_net_messaging PUBLIC ../include)
target_link_libraries(simple_net_messaging PUBLIC Boost::boost)