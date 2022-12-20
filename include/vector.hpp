// --------------------------------
// Created by 김민규 on 2022/12/21.
// --------------------------------

#ifndef FT_CONTAINER_VECTOR_HPP
#define FT_CONTAINER_VECTOR_HPP

namespace ft
{


template<typename T, class Allocator = std::allocator<T> >
class vector
{
public:
	vector()
	{
	}

private:
	T*      m_Data;
	size_t  m_Size;


};






} // namespace ft
#endif //FT_CONTAINER_VECTOR_HPP
