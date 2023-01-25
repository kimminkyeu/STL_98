cmake_minimum_required(VERSION 3.13)

project("ft_container")

set(CMAKE_CXX_STANDARD 98)

add_compile_options(-Wall -Wextra -Wall -g3 -fsanitize=address)
add_link_options(-Wall -Wextra -Wall -g3 -fsanitize=address)

set(TEST_SRC_FILES
        test_2/main.cpp)

add_executable(tester
        ${TEST_SRC_FILES}
        )

target_include_directories(tester
        PUBLIC
        ft_container
        )
