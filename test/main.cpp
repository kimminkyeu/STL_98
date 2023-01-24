
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
 #include <map>
// #include <functional>
#include "map.hpp"

// #define TEST FT::_PRIVATE
#define TEST FT
#define CONTAINER TEST::map

int		main()
{
    FT::map<int, int> m;
    FT::map<int, int>::iterator itr = m.begin();
    m.max_size();
    m[3] = 5;
    m.at(3) = 4;

    FT::map<std::string, std::string> dict;

    auto itr3 = dict.begin();
    FT::map<std::string, std::string>::iterator itr2 = dict.begin();
    itr2->second = "hi";

    itr2 = --(dict.end());
    itr2->second = "hello";

//    FT::map<std::string, std::string>::const_iterator const_itr2 = dict.begin();
//    const_itr2->second = "bew";



	return (0);
}

