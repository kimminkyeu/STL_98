
#include <iostream>
#include "ft_vector.hpp"

template<class Iter>
void print_all(Iter begin, Iter end)
{
    // 이 알고리즘 작성자는 컨테이너가 벡터인지 모른다. 몰라도 되도록 추상화해야 한다.
    // 그래서 iterator_traits가 존재한다.
    typename ft::iterator_traits<Iter>::value_type temp = *begin;

    // c++11 이후에 생긴 allocator_traits도 같은 목적이다.
    // 그 이전에는 알고리즘에서 서로 다른 custom_allocator 를 쓰는 컨테이너간의 알고리즘 적용은 undefined_behavior였으나
    // 이후엔 이것에 대한 해결법이 제안되면서, allocator_traits가 등장하였다.

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
    vec.pop_back();
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
