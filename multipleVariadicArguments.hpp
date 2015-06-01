#ifndef MULTIPLE_VARIADIC_ARGUMENTS_HPP
#define MULTIPLE_VARIADIC_ARGUMENTS_HPP

struct MultipleVariadicArgumentSeparator {
	;
} multipleVariadicArgumentSeparator;

template<typename... T>
struct MultipleVariadicArgumentSeparatorHandler {
	enum { SeparatorPosition = MultipleVariadicArgumentSeparatorHandler<T...>::SeparatorPosition };
};

template<>
struct MultipleVariadicArgumentSeparatorHandler<MultipleVariadicArgumentSeparator> {
	enum { SeparatorPosition = 0 };
};

template<typename T>
struct MultipleVariadicArgumentSeparatorHandler<T> {
	enum { SeparatorPosition = 1 + MultipleVariadicArgumentSeparatorHandler<MultipleVariadicArgumentSeparator>::SeparatorPosition };
};

template<typename... Tail>
struct MultipleVariadicArgumentSeparatorHandler<MultipleVariadicArgumentSeparator, Tail...> {
	enum { SeparatorPosition = MultipleVariadicArgumentSeparatorHandler<MultipleVariadicArgumentSeparator>::SeparatorPosition };
};

template<typename T, typename... Tail>
struct MultipleVariadicArgumentSeparatorHandler<T, Tail...> {
	enum { SeparatorPosition = MultipleVariadicArgumentSeparatorHandler<Tail...>::SeparatorPosition + 1 };
};

template<typename... T>
constexpr auto findSeparatorPosition(const T...) -> unsigned int {
	return MultipleVariadicArgumentSeparatorHandler<T...>::SeparatorPosition;
}

template<typename... T>
constexpr auto hasSeparator(const T...) -> bool {
	return (MultipleVariadicArgumentSeparatorHandler<T...>::SeparatorPosition < sizeof...(T));
}

#endif
