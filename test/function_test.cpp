
#include <boost/test/unit_test.hpp>
#include "checks.hpp"
#include "function.hpp"

BOOST_AUTO_TEST_SUITE(function_tests);

int fun(bool, long) {
	return 0;
}

BOOST_AUTO_TEST_CASE(function) {
	check_fun(fun);
}

BOOST_AUTO_TEST_CASE(function_pointer) {
	int (*funp)(bool, long) = fun;
	check_fun(funp);
	check_fun(&fun);
}

BOOST_AUTO_TEST_CASE(function_pointer_cvref) {
	int (*funp)(bool, long) = fun;
	const auto cfunp = funp;
	const auto &cfunpr = funp;
	check_fun(cfunp);
	check_fun(cfunpr);
	check_fun(std::move(funp));
}

BOOST_AUTO_TEST_SUITE_END()

