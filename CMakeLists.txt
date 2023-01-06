cmake_minimum_required(VERSION 3.13)

project("ft_container")

set(CMAKE_CXX_STANDARD 98)

add_compile_options(-Wall -Wextra -Wall -g3 -fsanitize=address)
add_link_options(-Wall -Wextra -Wall -g3 -fsanitize=address)

# set(STL_SRC_FILES
        # )

set(TEST_SRC_FILES
        test/main.cpp
        include/ft_utility.hpp include/__config.hpp include/ft_vector_base.hpp)

add_executable(tester
        # ${STL_SRC_FILES}
        ${TEST_SRC_FILES}
        )

target_include_directories(tester
        PUBLIC
        include
        )
