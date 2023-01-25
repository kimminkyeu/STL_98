
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>
#include "__config.hpp"
#include "iterator.hpp"
#include "vector.hpp"
#include <map>
#include "map.hpp"
#include <list>


int		main(void)
{
	ft::map<int, int> map;
	map[0] = 2;
	map[1] = 3;

	ft::map<int, int>::const_iterator const_begin = map.begin();
	ft::map<int, int>::const_iterator const_end = map.end();

	std::cout << std::distance(const_begin, const_end) << std::endl;

	return (0);
}
