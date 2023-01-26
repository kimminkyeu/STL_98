#include "../ft_container/vector.hpp"
#include "../ft_container/stack.hpp"
#include "../ft_container/map.hpp"
#include "../ft_container/set.hpp"

#include <iostream>
#include <string>
#include <deque>
#include <utility>
#if 1 //CREATE A REAL STL EXAMPLE
	#include <map>
	#include <stack>
	#include <vector>
	// namespace ft = std;
#else
	#include <map.hpp>
	#include <stack.hpp>
	#include <vector.hpp>
#endif

#include <stdlib.h>
#include <chrono>

#define MAX_RAM 12945
#define BUFFER_SIZE 4096
struct Buffer
{
	int idx;
	char buff[BUFFER_SIZE];
};

/*
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    int sum = 0;
    for (int i = 0;i < 999999999;i++)
        sum += i;
    std::chrono::duration<double>sec = std::chrono::system_clock::now() - start;
    std::cout << "for문을 돌리는데 걸리는 시간(초) : " << sec.count() <<"seconds"<< std::endl;
*/

#define COUNT (MAX_RAM / (int)sizeof(Buffer))

template<typename T>
class MutantStack : public ft::stack<T>
{
public:
	MutantStack() {}
	MutantStack(const MutantStack<T>& src) { *this = src; }
	MutantStack<T>& operator=(const MutantStack<T>& rhs)
	{
		this->c = rhs.c;
		return *this;
	}
	~MutantStack() {}

	typedef typename ft::stack<T>::container_type::iterator iterator;

	iterator begin() { return this->c.begin(); }
	iterator end() { return this->c.end(); }
};

int main(int argc, char** argv) {
	if (argc != 2)
	{
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}
	const int seed = atoi(argv[1]);
	srand(seed);

	ft::vector<std::string> vector_str;
	ft::vector<int> vector_int;
	ft::stack<int> stack_int;
	ft::vector<Buffer> vector_buffer;
	ft::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;
	ft::map<int, int> map_int;

	for (int i = 0; i < COUNT; i++)
	{
		vector_buffer.push_back(Buffer());
	}

	for (int i = 0; i < COUNT; i++)
	{
		const int idx = rand() % COUNT;
		vector_buffer[idx].idx = 5;
	}
	ft::vector<Buffer>().swap(vector_buffer);

	try
	{
		for (int i = 0; i < COUNT; i++)
		{
			const int idx = rand() % COUNT;
			vector_buffer.at(idx);
			std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" <<std::endl;
		}
	}
	catch(const std::exception& e)
	{
		//NORMAL ! :P
	}

	for (int i = 0; i < COUNT; ++i)
	{
		int a = rand();
		int b = rand();
		map_int.insert(ft::make_pair(a, b));
	}



//-----------------------------------------------------------------------
{ // * test 2

#define CONTAINER 	ft::map<std::string, std::string>
#define CONST_ITER 	ft::map<std::string, std::string>::const_iterator
#define ITER 		ft::map<std::string, std::string>::iterator

//----------------------------------------------------------
	ft::map<std::string, std::string> symbol_table;
	symbol_table["mike"] = "hello";
	symbol_table["jane"] = "hello";
	symbol_table["peter"] = "john";
	symbol_table["apple"] = "11111";
	symbol_table["bear"] = "india";

	for (CONST_ITER itr = symbol_table.begin(); itr != symbol_table.end(); ++itr)
	{ std::cout << itr->first << " : " << itr->second << std::endl; }

	std::cout << "\n";
	std::cout << "-----------------------------------------------\n";
	std::cout << "Testing insertion on same key\n";
	std::cout << "-----------------------------------------------\n";

	symbol_table.insert(ft::make_pair("mike", "woops!"));

//----------------------------------------------------------

	ITER backup_iter = symbol_table.find("peter");

	std::cout << "\n";
	std::cout << "-----------------------------------------------\n";
	std::cout << "Before insert and delete : invalidation \n";
	std::cout << "-----------------------------------------------\n";
	std::cout << backup_iter->first << " : " << backup_iter->second << std::endl;

	symbol_table.insert(ft::make_pair("Kim", "is korean"));
	symbol_table.erase("bear");
	symbol_table.erase("mike");
	symbol_table.erase("Kim");

	std::cout << "\n";
	std::cout << "-----------------------------------------------\n";
	std::cout << "After insert and delete : invalidation \n";
	std::cout << "-----------------------------------------------\n";
	std::cout << backup_iter->first << " : " << backup_iter->second << std::endl;


	std::cout << "\n";
	std::cout << "\n";
	for (CONST_ITER itr = symbol_table.begin(); itr != symbol_table.end(); ++itr)
	{ std::cout << itr->first << " : " << itr->second << std::endl; }

//----------------------------------------------------------
	std::cout << "\n";
	std::cout << "-----------------------------------------------\n";
	std::cout << "Testing erase\n";
	std::cout << "-----------------------------------------------\n";

	symbol_table.erase("key that doesn't exist");
	symbol_table.erase("apple");

	std::cout << "\n";
	for (CONST_ITER itr = symbol_table.begin(); itr != symbol_table.end(); ++itr)
	{ std::cout << itr->first << " : " << itr->second << std::endl; }

	std::cout << "\n";

	std::cout << "\n";
	std::cout << "-----------------------------------------------\n";
	std::cout << "Copy Data to another map\n";
	std::cout << "-----------------------------------------------\n";

	CONTAINER other(symbol_table.begin(), symbol_table.end());
	std::cout << "\n";
	for (CONST_ITER itr = other.begin(); itr != other.end(); ++itr)
	{ std::cout << itr->first << " : " << itr->second << std::endl; }

	std::cout << "\n";
//----------------------------------------------------------
}
	std::cout << "\n";
	std::cout << "-----------------------------------------------\n";
	std::cout << "Map Performance check\n";
	std::cout << "-----------------------------------------------\n";


#define STL   std::map<int, int>
#define CNT   ft::map<int, int>

	STL stl_map;
	CNT ft_map;
	const int total = 1000000;

        {
        // --------------------------------------------------------------
        std::chrono::system_clock::time_point start =
            std::chrono::system_clock::now();
        for (int i = 0; i < total; ++i) {
                        stl_map.insert(std::make_pair(i, i));
        }
		for (STL::iterator iter = stl_map.begin(); iter != stl_map.end(); ++iter) {
			// ...
		}
        for (int i = 0; i < total; ++i) {
                        stl_map.erase(i);
        }
        std::chrono::duration<double> sec =
            std::chrono::system_clock::now() - start;
        std::cout << PRINT_BLUE
                  << "STD's insert & delete " << total << "data : " << sec.count()
                  << " seconds " << PRINT_RESET << std::endl;
        // --------------------------------------------------------------

        // --------------------------------------------------------------
        std::chrono::system_clock::time_point start2 =
            std::chrono::system_clock::now();
        for (int i = 0; i < total; ++i) {
                        ft_map.insert(ft::make_pair(i, i));
        }
		for (CNT::iterator iter = ft_map.begin(); iter != ft_map.end(); ++iter) {
			// ...
		}
        for (int i = 0; i < total; ++i) {
                        ft_map.erase(i);
        }
        std::chrono::duration<double> sec2 = std::chrono::system_clock::now() - start2;
        std::cout << PRINT_GREEN
                  << "FT's  insert & delete " << total << "data : " << sec2.count()
                  << " seconds " << PRINT_RESET << std::endl;
        // --------------------------------------------------------------
        }


	std::cout << "\n";
	system("leaks test > leaks_result_temp; cat leaks_result_temp | grep leaked && rm -rf leaks_result_temp");
	return (0);
}
