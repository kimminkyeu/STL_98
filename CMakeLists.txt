cmake_minimum_required(VERSION 3.13)

project("webserv")

set(CMAKE_CXX_STANDARD 98)

add_compile_options(-Wall -Wextra -Wall -g3 -fsanitize=address)
add_link_options(-Wall -Wextra -Wall -g3 -fsanitize=address)

set(TEST_FILES
        test/main.cpp
        )

set(SOURCE_FILES
        src/vector.cpp 
)

add_executable(test
        ${SOURCE_FILES}
        )

target_include_directories(
        mySTL
        PUBLIC
        include
        )