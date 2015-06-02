#ifndef MULTIPLE_VARIADIC_ARGUMENTS_HPP
#define MULTIPLE_VARIADIC_ARGUMENTS_HPP

#if __cplusplus >= 201103L
#define USE_CPP11
#endif

#ifdef USE_CPP11
#include <tuple>
#endif

#include "multipleVariadicArguments_Impl.hpp"

#ifdef USE_CPP11

template<typename... T>
struct MultipleVariadicArgumentSeparatorHandler {
	enum { SeparatorPosition = detail::MultipleVariadicArgumentSeparatorHandler_Impl<T...>::SeparatorPosition };
};

template<typename... T>
constexpr auto findSeparatorPosition(const T...) -> unsigned int {
	return MultipleVariadicArgumentSeparatorHandler<T...>::SeparatorPosition;
}

template<typename... T>
constexpr auto hasSeparator(const T...) -> bool {
	return (MultipleVariadicArgumentSeparatorHandler<T...>::SeparatorPosition < sizeof...(T));
}

template <int nbOfArgumentsToTake, typename F, typename Tuple>
void callMultipleArgumentFunctionTuple(F& f, Tuple&& t)
{
    detail::MultipleVariadicArgumentCall_Impl<0 == nbOfArgumentsToTake, F, Tuple, nbOfArgumentsToTake>::call(f, std::forward<Tuple>(t));
}

template<typename F, typename... Args>
auto callMultipleArgumentFunction(F& f, Args... args) -> void {
	auto newTuple = std::make_tuple(args...);
	callMultipleArgumentFunctionTuple<MultipleVariadicArgumentSeparatorHandler<Args...>::SeparatorPosition>(f, newTuple);
}

#endif
#endif
