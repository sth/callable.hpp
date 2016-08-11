
#if !defined(CALLABLE_TEST_CHECKS_HPP_INCLUDED)
#define CALLABLE_TEST_CHECKS_HPP_INCLUDED

#include "function.hpp"
//#include "member_function.hpp"
#include "callable.hpp"

template<typename A, typename B>
struct same_type {
	static const bool value = false;
};

template<typename A>
struct same_type<A, A> {
	static const bool value = true;
};

template<typename Callable>
void check_call(Callable c) {
	BOOST_CHECK((same_type<typename callable_traits<Callable>::return_type, int>::value));
	BOOST_CHECK_EQUAL(callable_traits<Callable>::argc, 2);
	BOOST_CHECK((same_type<typename callable_traits<Callable>::template argument_type<0>, bool>::value));
	BOOST_CHECK((same_type<typename callable_traits<Callable>::template argument_type<1>, long>::value));
	BOOST_CHECK((same_type<typename callable_traits<Callable>::function_type, int(bool, long)>::value));
}

template<typename Fun>
void check_fun(Fun c) {
	BOOST_CHECK((same_type<typename function_traits<Fun>::return_type, int>::value));
	BOOST_CHECK_EQUAL(function_traits<Fun>::argc, 2);
	BOOST_CHECK((same_type<typename function_traits<Fun>::template argument_type<0>, bool>::value));
	BOOST_CHECK((same_type<typename function_traits<Fun>::template argument_type<1>, long>::value));
	BOOST_CHECK((same_type<typename function_traits<Fun>::function_type, int(bool, long)>::value));

	BOOST_CHECK((same_type<typename callable_traits<Fun>::callable_category, function_ptr_tag>::value));
	check_call(std::forward<Fun>(c));
}

#endif // CALLABLE_TEST_CHECKS_HPP_INCLUDED

