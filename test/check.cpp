
#include <iostream>

class A
{
public:
	A(){ std::cout << "A Constructor called\n";}
	~A(){ std::cout << "A Destructor called\n";}
};

int main(void)
{
	// 이렇게 객체 배열을 할당하면, 공간 10개가 전부 생성자가 호출되어 초기화된다. 
	A* arr_of_a = new A[10];	

	void* ptr = ::operator new(10);

	delete[] arr_of_a;
/*	[실행 결과]
	A Constructor called
	A Constructor called
	A Constructor called
	A Constructor called
	A Constructor called
	A Constructor called
	A Constructor called
	A Constructor called
	A Constructor called
	A Constructor called
	A Destructor called
	A Destructor called
	A Destructor called
	A Destructor called
	A Destructor called
	A Destructor called
	A Destructor called
	A Destructor called
	A Destructor called
	A Destructor called */
	return (0);
}