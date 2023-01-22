//
// Created by 김민규 on 2023/01/23.
//

#include <iostream>
#include "__config.hpp"
#include "iterator.hpp"
#include "vector.hpp"
#include <map>
#include "__Left_Leaning_RedBlack.hpp"

// #define TEST FT::_PRIVATE
#define TEST FT::_PRIVATE

int		main()
{
    TEST::LeftLeaningRedBlack<int> test;

    test.put(2);
    TEST::printTree(test);
    test.put(3);
    TEST::printTree(test);
    test.put(10);
    TEST::printTree(test);
    test.put(8);
    TEST::printTree(test);
    test.put(1);
    TEST::printTree(test);
    test.put(12);
    TEST::printTree(test);
    test.put(5);
    TEST::printTree(test);
    test.put(9);
    TEST::printTree(test);
    test.put(27);
    TEST::printTree(test);
    test.put(4);

    TEST::printTree(test);
    TEST::LeftLeaningRedBlack<int>::node_pointer ptr = test.begin();
    while (ptr != test.end())
    {
        std::cout << ptr->key << std::endl;
        ptr = ptr->getSuccessor();
    }

    std::cout << "\n--------------------\n";
    std::cout << "Testing erase... \n";
    std::cout << "--------------------\n";

    test.erase(3);
    TEST::printTree(test);
    test.erase(10);
    TEST::printTree(test);
    test.erase(12);
    TEST::printTree(test);
    test.erase(8);
    TEST::printTree(test);

    ptr = test.begin();
    while (ptr != test.end())
    {
        std::cout << ptr->key << std::endl;
        ptr = ptr->getSuccessor();
    }

    return (0);
}

