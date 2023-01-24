
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

#define CONTAINER   FT::map<int, std::string>
#define ITERATOR    CONTAINER::iterator

int		main()
{
    FT::map<int, std::string> m;

    std::cout << PRINT_BLUE << "------------------------------" << PRINT_RESET << std::endl;
    std::cout << PRINT_BLUE << "|     Testing insertion      |" << PRINT_RESET << std::endl;
    std::cout << PRINT_BLUE << "------------------------------" << PRINT_RESET << std::endl;
    m[0] = "a";
    m[1] = "b";
    m[2] = "c";
//    m.insert(CONTAINER::value_type(3, "hello"));
//    m.insert(FT::make_pair(3, "hello"));
//    CONTAINER::value_type pair_type = CONTAINER::value_type(FT::make_pair(3, "hi"));
    m.insert(FT::make_pair(3, "hi"));

    std::cout << PRINT_BLUE << "------------------------------" << PRINT_RESET << std::endl;
    std::cout << PRINT_BLUE << "|      Testing find()       |" << PRINT_RESET << std::endl;
    std::cout << PRINT_BLUE << "------------------------------" << PRINT_RESET << std::endl;

    std::cout << "0 : " << m.find(0)->second << std::endl;
    std::cout << "1 : " << m.find(1)->second << std::endl;
    std::cout << "2 : " << m.find(2)->second << std::endl;
    std::cout << "size of map : " << m.size() << std::endl;





    std::cout << PRINT_BLUE << "------------------------------" << PRINT_RESET << std::endl;
    std::cout << PRINT_BLUE << "|      Testing erase()       |" << PRINT_RESET << std::endl;
    std::cout << PRINT_BLUE << "------------------------------" << PRINT_RESET << std::endl;

    m.erase(0);

    std::cout << "data in map" << std::endl;
    for (auto & itr : m) { std::cout << itr.second << " "; };
    std::cout << "\n\n";

    m.erase(1);
    m.erase(3);

    std::cout << "data in map" << std::endl;
    for (auto & itr : m) { std::cout << itr.second << " "; };
    std::cout << "\n\n";

    std::cout << "size of map : " << m.size() << std::endl;


	return (0);
}

