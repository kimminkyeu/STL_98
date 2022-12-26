
/** -----------------------------------------------------
 *  | Implementation of memory-pool for personal study. |
 *  -----------------------------------------------------
 *  Github link : https://github.com/kimminkyeu
 */


#pragma once

#ifndef MEMORY_POOL_HPP
#define MEMORY_POOL_HPP


class PoolAllocator
{

private:


public: // contructor & destructor
	PoolAllocator() 
	{}

	~PoolAllocator()
	{}

public: // interface function

	void*	allocate(size_t size)
	{}

	void	deallocate(void* chunk, size_t size)
	{}


};



#endif // MEMORY_POOL_HPP