
#include <iostream>
#include "MemPool.hpp"

// (1) 주소를 직접 찍어보기
// (2) leak 체크
// (3) 퍼포먼스, valgrind를 통한 cache hit rate 체크.

class Animal
{
private:
    uint64_t data; // data size of 64-bit

public:
    Animal()
    {
        std::cout << "Animal Contructor called\n";
    }

    ~Animal()
    {
        std::cout << "Animal destructor called\n";
    }
};

int main(void)
{
    // (0) create memory-bucket
    PoolAllocator pool;

    // (1) 얼만큼 풀을 사용할 건지 예약. (default memory block)

    
    // (2) allocate chunck from pool.
    Animal* ptr = pool.allocate(sizeof(Animal));

    // (3) deallocate pointer.
    pool.deallocate(ptr);

    return (0);
}
