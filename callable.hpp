
#if !defined(STHCXX_CALLABLE_HPP_INCLUDED)
#define STHCXX_CALLABLE_HPP_INCLUDED

#include <stddef.h>
#include <functional>

namespace detail {

/** Count the number of types given to the template */
template<typename... Types>
struct tva_count;

template<>
struct tva_count<> {
	static const size_t value = 0;
};

template<typename Type, typename... Types>
struct tva_count<Type, Types...> {
	static const size_t value = tva_count<Types...>::value + 1;
};


/** Get the nth type given to the template */
template<size_t n, typename... Types>
struct tva_n;

template<size_t N, typename Type, typename... Types>
struct tva_n<N, Type, Types...> : tva_n<N-1, Types...> {
};

template<typename Type, typename... Types>
struct tva_n<0, Type, Types...> {
	typedef Type type;
};


/** Define traits for a function type */
template<typename Fun>
struct callable_traits_fn;

template<typename Ret, typename... Args>
struct callable_traits_fn<Ret (Args...)> {
	typedef Ret function_type(Args...);
	typedef Ret return_type;
	static const size_t argc;

	template<size_t N>
	using argument_type = typename tva_n<N, Args...>::type;
};

template<typename Ret, typename... Args>
const size_t callable_traits_fn<Ret (Args...)>::argc = tva_count<Args...>::value;


/** Define traits for a operator() member function pointer type */
template<typename MemFun>
struct callable_traits_memfnp;

template<typename Class, typename Ret, typename... Args>
struct callable_traits_memfnp<Ret (Class::*)(Args...)> : callable_traits_fn<Ret (Args...)> {
};

template<typename Class, typename Ret, typename... Args>
struct callable_traits_memfnp<Ret (Class::*)(Args...) const> : callable_traits_fn<Ret (Args...)> {
};


// classes with operator()
template<typename Callable>
struct callable_traits_d : detail::callable_traits_memfnp<decltype(&Callable::operator())> {
};

// functions
template<typename Ret, typename... Args>
struct callable_traits_d<Ret (Args...)> : detail::callable_traits_fn<Ret (Args...)> {
};

// function pointers
template<typename Ret, typename... Args>
struct callable_traits_d<Ret (*)(Args...)> : detail::callable_traits_fn<Ret (Args...)> {
};

// std::function specializations
template<typename Ret, typename... Args>
struct callable_traits_d<std::function<Ret (Args...)>> : detail::callable_traits_fn<Ret (Args...)> {
};

} // namespace detail


// Main template

/** Traits for a callable (function/functor/lambda/...) */
template<typename Callable>
struct callable_traits : detail::callable_traits_d<typename std::remove_reference<Callable>::type> {
};

/** Convert a callable to a std::function<> */
template<typename Callable>
std::function<typename callable_traits<Callable>::function_type> to_stdfunction(Callable fun) {
	std::function<typename callable_traits<Callable>::function_type> stdfun(std::forward<Callable>(fun));
	return stdfun;
}

#endif // STHCXX_CALLABLE_HPP_INCLUDED

