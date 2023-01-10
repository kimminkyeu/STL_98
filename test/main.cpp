
#include <iostream>
#include "ft_vector.hpp"


template<class Iter>
void print_all(Iter begin, Iter end)
{
	for (; begin != end; ++begin)
	{
		std::cout << *begin << " ";
	}
	std::cout << "\n";
}

template<typename T>
void print_data(ft::vector<T>& vec)
{
	std::cout << "size : " << vec.size() << std::endl;
	std::cout << "capa : " << vec.capacity() << std::endl;
	print_all(vec.begin(), vec.end());
}

int main()
{
    ft::vector<int> vec(10, 3);
    vec.push_back(3);
	print_data(vec);

/*
Candidate template ignored:
substitution failure [with InputIterator = ft::random_access_iterator<int *, ft::vector<int> >]
: argument may not have 'void' type
*/

	ft::vector<int> vec_cpy(vec.begin(), vec.end());
	print_data(vec_cpy);

	return (0);
}
