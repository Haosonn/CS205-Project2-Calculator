cmake_minimum_required(VERSION 3.23)
project(Project2)

set(CMAKE_CXX_STANDARD 11)

add_executable(main main.cpp LargeNumber.cpp Expression.cpp)
add_executable(test LargeNumber.cpp test.cpp Expression.cpp)

target_include_directories(main PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_include_directories(test PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )