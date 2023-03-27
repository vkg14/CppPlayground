cmake_minimum_required(VERSION 3.24)
project(CppPlayground)

set(CMAKE_CXX_STANDARD 20)

add_executable(HelloWorld main.cpp vkg_shared_ptr.h ctc_codility.cpp ctc_codility.h)
