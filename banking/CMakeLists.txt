cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(
    banking
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  

add_library(
    banking
    STATIC
    Account.cpp
    Account.h
    Transaction.cpp
    Transaction.h
)
