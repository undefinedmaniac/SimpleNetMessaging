cmake_minimum_required(VERSION 3.21)

project(
  SimpleNetMessagingDemo
  VERSION 1.0
  DESCRIPTION "A library for basic network messaging"
  LANGUAGES CXX)

# set(BUILD_SHARED_LIBS OFF)
# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

find_package(Boost 1.77 REQUIRED)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

add_subdirectory(src)
add_subdirectory(apps)