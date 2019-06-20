#ifndef __INVOCER_HEADER_INCLUDED__
#define __INVOCER_HEADER_INCLUDED__

#include <type_traits>

template <class Function>
class Invocer
{
	static_assert(std::is_nothrow_invocable_v<Function>,
		          "Function must be nothrow-invocable!");

public:
	explicit Invocer(Function f)
	noexcept(std::is_nothrow_copy_constructible_v<Function>) :
		function(f)
	{
	}

	Invocer(const Invocer&) = delete;
	Invocer& operator=(const Invocer&) = delete;

	~Invocer()
	{
		function();
	}

private:
	Function function;
};

#endif //__INVOCER_HEADER_INCLUDED__