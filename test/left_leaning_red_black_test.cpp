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

//    TEST::LeftLeaningRedBlack<int>::iterator t = test.begin();
    TEST::LeftLeaningRedBlack<int>::iterator t = test.begin();
    TEST::LeftLeaningRedBlack<int>::const_iterator t2 = test.begin();


    std::cout << "\n--------------------\n";
    std::cout << "Testing copy... \n";
    std::cout << "         copied tree: \n";
    std::cout << "--------------------\n";

    auto i = *(test.begin());
    auto i2 = test.begin();
    auto i3 = *i2;
    int p = i3.key;
    // TEST::LeftLeaningRedBlack<int> other(test.begin(), test.end());
    // TEST::printTree(other);



//    std::cout << "\n--------------------\n";
//    std::cout << "Testing erase... \n";
//    std::cout << "--------------------\n";
//
//    test.erase(3);
//    TEST::printTree(test);
//    test.erase(10);
//    TEST::printTree(test);
//    test.erase(12);
//    TEST::printTree(test);
//    test.erase(8);
//    TEST::printTree(test);


    return (0);
}

