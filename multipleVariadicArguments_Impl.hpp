#ifndef MULTIPLE_VARIADIC_ARGUMENTS_IMPL_HPP
#define MULTIPLE_VARIADIC_ARGUMENTS_IMPL_HPP

#if __cplusplus >= 201103L
#define USE_CPP11
#endif

#ifdef USE_CPP11
#include <tuple>
#include <utility>
#endif

struct Empty {};
struct MultipleVariadicArgumentSeparator {} multipleVariadicArgumentSeparator;

#ifdef USE_CPP11
namespace detail {
	// Implementation details, user should not call any of these
	template<typename... T>
	struct MultipleVariadicArgumentSeparatorHandler_Impl {
		// Less specialized function: we only get here when we have an empty parameter pack T
		enum { SeparatorPosition = 0 };
		typedef std::tuple<Empty> TailType;
	};

	template<>
	struct MultipleVariadicArgumentSeparatorHandler_Impl<MultipleVariadicArgumentSeparator> {
		enum { SeparatorPosition = 0 };
		typedef std::tuple<Empty> TailType;
	};

	template<typename T>
	struct MultipleVariadicArgumentSeparatorHandler_Impl<T> {
		enum { SeparatorPosition = 1 + MultipleVariadicArgumentSeparatorHandler_Impl<MultipleVariadicArgumentSeparator>::SeparatorPosition };
		typedef std::tuple<Empty> TailType;
	};

	template<typename... Tail>
	struct MultipleVariadicArgumentSeparatorHandler_Impl<MultipleVariadicArgumentSeparator, Tail...> {
		enum { SeparatorPosition = MultipleVariadicArgumentSeparatorHandler_Impl<MultipleVariadicArgumentSeparator>::SeparatorPosition };
		typedef std::tuple<Tail...> TailType;
	};

	template<typename T, typename... Tail>
	struct MultipleVariadicArgumentSeparatorHandler_Impl<T, Tail...> {
		enum { SeparatorPosition = MultipleVariadicArgumentSeparatorHandler_Impl<Tail...>::SeparatorPosition + 1 };
		typedef typename MultipleVariadicArgumentSeparatorHandler_Impl<Tail...>::TailType TailType;
	};

	// Unpack tuple implementation
	// Heavily based on: http://stackoverflow.com/questions/10766112/c11-i-can-go-from-multiple-args-to-tuple-but-can-i-go-from-tuple-to-multiple
	template <bool done, typename F, typename Tuple, int nbOfArgumentsToTake, int... N>
	struct MultipleVariadicArgumentCall_Impl {
		static void call (F& f, Tuple&& t) {
			MultipleVariadicArgumentCall_Impl<nbOfArgumentsToTake == 1+sizeof...(N), F, Tuple, nbOfArgumentsToTake, N..., sizeof...(N)>::call(f,t);	
		}
	};

    template <typename F, typename Tuple, int nbOfArgumentsToTake, int... N>
    struct MultipleVariadicArgumentCall_Impl<true, F, Tuple, nbOfArgumentsToTake, N...> {
        static void call(F& f, Tuple&& t) {
			f(std::get<N>(std::forward<Tuple>(t))...);
        }
    };

	template <bool offsetStarted, typename F, typename Tuple, int nbOfArgumentsToTake, int offset, int... N>
	struct MultipleVariadicArgumentCallWithOffset {
		static void call(F& f, Tuple&& t) {
			MultipleVariadicArgumentCallWithOffset<offsetStarted == sizeof...(N), F, Tuple, nbOfArgumentsToTake, offset, N..., sizeof...(N)>::call(f,t);	
		}
	};

	template <typename F, typename Tuple, int nbOfArgumentsToTake, int offset, int... N>
	struct MultipleVariadicArgumentCallWithOffset<true, F, Tuple, nbOfArgumentsToTake, offset, N...> {
		static void call(F& f, Tuple&& t) {
			MultipleVariadicArgumentCall_Impl<0 == nbOfArgumentsToTake, F, Tuple, nbOfArgumentsToTake, sizeof...(N)>::call(f,t);
		}
	};
}

#endif

#endif
