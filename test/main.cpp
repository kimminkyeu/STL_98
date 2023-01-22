
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>
#include "__config.hpp"
#include "iterator.hpp"
#include "vector.hpp"
#include <map>
// #include <stack> // temp for print function
#include "__Left_Leaning_RedBlack.hpp"
// #include <map>
// #include <functional>

// #define TEST FT::_PRIVATE
#define TEST FT

int		main(void)
{
	TEST::LeftLeaningRedBlack<int> test;

	test.put(2);
	test.put(3);
	test.put(10);
	test.put(8);
	test.put(1);
	test.put(12);
	TEST::printTree(test);
	test.erase(3);
	TEST::printTree(test);
	test.erase(10);
	TEST::printTree(test);
	test.erase(12);
	TEST::printTree(test);

	return (0);
}

