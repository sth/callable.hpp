
#include <boost/test/unit_test.hpp>
#include "checks.hpp"
#include "callable.hpp"

BOOST_AUTO_TEST_SUITE(callable_tests);

int fun(bool, long) {
	return 0;
}

struct Func {
	int operator()(bool, long) {
		return 0;
	}
};

BOOST_AUTO_TEST_CASE(functor_object) {
	Func func;
	check_call(func);
}

struct CFunc {
	int operator()(bool, long) const {
		return 0;
	}
};

BOOST_AUTO_TEST_CASE(functor_object_const) {
	CFunc cfunc;
	check_call(cfunc);
}

BOOST_AUTO_TEST_CASE(lambda) {
	// Lambda as rvalue
	check_call([](bool, long) -> int { return 0; });

	// Lambda lvalue
	auto l = [](bool, long) -> int { return 0; };
	check_call(l);
}

BOOST_AUTO_TEST_CASE(lambda_capture) {
	int i = 0;
	check_call([&i](bool, long) -> int { i++; return 0; });
}

BOOST_AUTO_TEST_CASE(stdfunction) {
	std::function<int (bool, long)> sf;
	check_call(sf);
	check_call(std::function<int (bool, long)>(fun));
	check_call(std::move(sf));
}

struct Scvl {
	int operator()(bool, long) const volatile & {}
};

struct Scvr {
	int operator()(bool, long) const volatile && noexcept {}
};

BOOST_AUTO_TEST_CASE(function_object_qualifiers) {
	// Test
	Scvl scvl;
	Scvr scvr;

	check_call(scvl);
	BOOST_CHECK((same_type<member_function_traits<decltype(&Scvl::operator())>::class_type, Scvl>::value));
	BOOST_CHECK(member_function_traits<decltype(&Scvl::operator())>::is_const);
	BOOST_CHECK(member_function_traits<decltype(&Scvl::operator())>::is_volatile);
	BOOST_CHECK(member_function_traits<decltype(&Scvl::operator())>::is_lref);
	BOOST_CHECK(!member_function_traits<decltype(&Scvl::operator())>::is_rref);

	check_call(scvr);
	BOOST_CHECK(member_function_traits<decltype(&Scvr::operator())>::is_const);
	BOOST_CHECK(member_function_traits<decltype(&Scvr::operator())>::is_volatile);
	BOOST_CHECK(!member_function_traits<decltype(&Scvr::operator())>::is_lref);
	BOOST_CHECK(member_function_traits<decltype(&Scvr::operator())>::is_rref);
#if __cpp_noexcept_function_type
	BOOST_CHECK(member_function_traits<decltype(&Scvr::operator())>::is_noexcept);
#endif
}

BOOST_AUTO_TEST_SUITE_END()

