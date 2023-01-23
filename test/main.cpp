
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>
#include "__config.hpp"
#include "iterator.hpp"
#include "vector.hpp"
#include <map>

// #include <stack> // temp for print function
//#include "__Left_Leaning_RedBlack.hpp"
// #include <map>
// #include <functional>
#include "map.hpp"

// #define TEST FT::_PRIVATE
#define TEST FT
#define CONTAINER TEST::map

int		main()
{
    FT::map<int, int> m;
    FT::map<int, int>::iterator itr = m.begin();



	return (0);
}

