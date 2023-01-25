#ifndef FT_CONTAINER_STACK_HPP
#define FT_CONTAINER_STACK_HPP

#include "__config.hpp"
#include "vector.hpp"

FT_BEGIN_GLOBAL_NAMESPACE

template<class _T, class _Container = FT::vector<_T> >
class stack
{
public:
	typedef typename _Container::value_type			value_type;
	typedef typename _Container::size_type			size_type;
	typedef 		 _Container 					container_type;

protected:
	_Container c;

public:
	explicit stack(const _Container& ctnr = _Container()) : c(ctnr) {}
    stack(const stack& __q) : c(__q.c) {}
    stack& operator=(const stack& __q) {c = __q.c; return *this;}


	bool				empty(void) const 				{ return c.empty(); }
	size_type			size() const					{ return c.size(); }
	value_type&			top(void)						{ return c.back(); }
	const value_type&	top(void) const					{ return c.back(); }
	void				push(const value_type& value)	{ c.push_back(value); }
    void				pop(void)						{ c.pop_back(); }

private:
	template<class _T1, class _Container1>
	friend bool operator==(const stack<_T1, _Container1>&, const stack<_T1, _Container1>&);

	template<class _T1, class _Container1>
	friend bool operator<(const stack<_T1, _Container1>&, const stack<_T1, _Container1>&);
};

template<class _T, class _Container>
	inline bool
	operator==(const stack<_T, _Container>& lhs, const stack<_T, _Container>& rhs)
	{ return lhs.c == rhs.c; }

template<class _T, class _Container>
	inline bool
	operator<(const stack<_T, _Container>& lhs, const stack<_T, _Container>& rhs)
	{ return lhs.c < rhs.c; }

template<class _T, class _Container>
	inline bool
	operator!=(const stack<_T, _Container>& lhs, const stack<_T, _Container>& rhs)
	{ return !(lhs == rhs); }

template<class _T, class _Container>
	inline bool
	operator<=(const stack<_T, _Container>& lhs, const stack<_T, _Container>& rhs)
	{ return !(rhs < lhs); }

template<class _T, class _Container>
	inline bool
	operator>(const stack<_T, _Container>& lhs, const stack<_T, _Container>& rhs)
	{ return rhs < lhs; }

template<class _T, class _Container>
	inline bool
	operator>=(const stack<_T, _Container>& lhs, const stack<_T, _Container>& rhs)
	{ return !(lhs < rhs); }


FT_END_GLOBAL_NAMESPACE

#endif // FT_CONTAINER_STACK_HPP
