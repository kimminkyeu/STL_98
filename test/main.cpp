
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

// --- Class foo
template <typename T>
class foo {
	public:
		typedef T	value_type;

		foo(void) : value(), _verbose(false) { };
		foo(value_type src, const bool verbose = false) : value(src), _verbose(verbose) { };
		foo(foo const &src, const bool verbose = false) : value(src.value), _verbose(verbose) { };
		~foo(void) { if (this->_verbose) std::cout << "~foo::foo()" << std::endl; };
		void m(void) { std::cout << "foo::m called [" << this->value << "]" << std::endl; };
		void m(void) const { std::cout << "foo::m const called [" << this->value << "]" << std::endl; };
		foo &operator=(value_type src) { this->value = src; return *this; };
		foo &operator=(foo const &src) {
			if (this->_verbose || src._verbose)
				std::cout << "foo::operator=(foo) CALLED" << std::endl;
			this->value = src.value;
			return *this;
		};
		value_type	getValue(void) const { return this->value; };
		void		switchVerbose(void) { this->_verbose = !(this->_verbose); };

		operator value_type(void) const {
			return value_type(this->value);
		}
	private:
		value_type	value;
		bool		_verbose;
};

template <typename T>
std::ostream	&operator<<(std::ostream &o, foo<T> const &bar) {
	o << bar.getValue();
	return o;
}
// --- End of class foo

template <typename T>
T	inc(T it, int n)
{
	while (n-- > 0)
		++it;
	return (it);
}

template <typename T>
T	dec(T it, int n)
{
	while (n-- > 0)
		--it;
	return (it);
}


#define TESTED_NAMESPACE FT
#define _pair TESTED_NAMESPACE::pair

template <typename T>
std::string	printPair(const T &iterator, bool nl = true, std::ostream &o = std::cout)
{
	o << "key: " << iterator->first << " | value: " << iterator->second;
	if (nl)
		o << std::endl;
	return ("");
}











template <typename T_MAP>
void	printSize(T_MAP const &mp, bool print_content = 1)
{
	std::cout << "size: " << mp.size() << std::endl;
	std::cout << "max_size: " << mp.max_size() << std::endl;
	if (print_content)
	{
		typename T_MAP::const_iterator it = mp.begin(), ite = mp.end();
		std::cout << std::endl << "Content is:" << std::endl;
		for (; it != ite; ++it)
			std::cout << "- " << printPair(it, false) << std::endl;
	}
	std::cout << "###############################################" << std::endl;
}







#include <list>

#define T1 float
#define T2 foo<int>
typedef _pair<const T1, T2> T3;

int		main(void)
{
	std::list<T3> lst;
	unsigned int lst_size = 5;
	for (unsigned int i = 0; i < lst_size; ++i)
		lst.push_back(T3(2.5 - i, (i + 1) * 7));

	TESTED_NAMESPACE::map<T1, T2> mp(lst.begin(), lst.end());
	TESTED_NAMESPACE::map<T1, T2>::reverse_iterator it(mp.rbegin());
	TESTED_NAMESPACE::map<T1, T2>::const_reverse_iterator ite(mp.rbegin());
	printSize(mp);
	std::cout << "-------------------------------------------------" << std::endl;
	// printPair(++ite);
	// std::cout << "-------------------------------------------------" << std::endl;
	// printPair(ite++);
	// std::cout << "-------------------------------------------------" << std::endl;
	// printPair(ite++);
	// std::cout << "-------------------------------------------------" << std::endl;
	// printPair(++ite);


	// it->second.m();
	// ite->second.m();

	// printPair(++it);
	// printPair(it++);
	// printPair(it++);
	// printPair(++it);

	// printPair(--ite);
	// printPair(ite--);
	// printPair(--ite);
	// printPair(ite--);


	// (*it).second.m();
	// (*ite).second.m();

	// printPair(--it);
	// printPair(it--);
	// printPair(it--);
	// printPair(--it);

	return (0);
}

