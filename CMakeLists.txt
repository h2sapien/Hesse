cmake_minimum_required(VERSION 3.17)
project(Hesse C)
find_package(OpenSSL REQUIRED)

set(CMAKE_C_STANDARD 99)

add_executable(Hesse main.c funk.h funk.c const.h)
target_link_libraries(Hesse OpenSSL::Crypto)