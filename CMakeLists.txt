cmake_minimum_required(VERSION 3.17)
project(MyAkinaTor)

set(CMAKE_CXX_STANDARD 14)

add_executable(MyAkinaTor main.cpp akinator/akinator.h akinator/akinator.cpp tree/tree.h tree/tree.cpp textLib/textLib.h textLib/textLib.cpp stack/stack.h stack/stack.cpp)