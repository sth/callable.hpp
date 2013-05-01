
#include <boost/test/unit_test.hpp>
#include "callable.hpp"

BOOST_AUTO_TEST_SUITE(callable_tests);

// Helpers

template<typename A, typename B>
struct same_type {
	static const bool value = false;
};

template<typename A>
struct same_type<A, A> {
	static const bool value = true;
};


template<typename Callable>
void check(Callable c) {
	BOOST_CHECK((same_type<typename callable_traits<Callable>::return_type, int>::value));
	BOOST_CHECK_EQUAL(callable_traits<Callable>::argc, 2);
	BOOST_CHECK((same_type<typename callable_traits<Callable>::template argument_type<0>, bool>::value));
	BOOST_CHECK((same_type<typename callable_traits<Callable>::template argument_type<1>, long>::value));
	BOOST_CHECK((same_type<typename callable_traits<Callable>::function_type, int(bool, long)>::value));
}

// Types

int fun(bool, long) {
	return 0;
}

BOOST_AUTO_TEST_CASE(function) {
	check(fun);
}

BOOST_AUTO_TEST_CASE(function_pointer) {
	int (*funp)(bool, long) = fun;
	check(funp);
}

struct Func {
	int operator()(bool, long) {
		return 0;
	}
};

BOOST_AUTO_TEST_CASE(functor_object) {
	Func func;
	check(func);
}

struct CFunc {
	int operator()(bool, long) const {
		return 0;
	}
};

BOOST_AUTO_TEST_CASE(functor_object_const) {
	CFunc cfunc;
	check(cfunc);
}

BOOST_AUTO_TEST_CASE(lambda) {
	auto l = [](bool, long) -> int { return 0; };
	check(l);
}

BOOST_AUTO_TEST_CASE(stdfunction) {
	std::function<int (bool, long)> sf;
	check(sf);
}

BOOST_AUTO_TEST_SUITE_END()

