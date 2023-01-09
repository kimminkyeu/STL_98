
#include <string>
#include <iostream>
#include <vector>
// #include "ft_vector.hpp"

int main()
{
	std::vector<int>::allocator_type alloc;
	std::allocator<int>().allocate(10);

	// ... test code here ...

	return (0);
}
