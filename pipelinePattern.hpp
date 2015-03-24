#ifndef PIPELINEPATTERN_HPP
#define PIPELINEPATTERN_HPP

#if __cplusplus >= 201103L
#define USE_CPP11
#endif

#ifdef USE_CPP11

template<typename FunctorElementType, typename T, typename... T2>
FunctorElementType pipeline(const FunctorElementType x, T& t1, T2&... t2) {
	return pipeline<FunctorElementType, T2...>(t1(x), t2...);
}

template<typename FunctorElementType, typename T>
FunctorElementType pipeline(const FunctorElementType x, T& t1) {
	return t1(x);
}

#else

template<typename FunctorElementType, typename T>
FunctorElementType pipeline(const FunctorElementType x, T& t1) {
	return t1(x);
}

template<typename FunctorElementType, typename T, typename T2>
FunctorElementType pipeline(const FunctorElementType x, T& t1, T2& t2) {
	return pipeline(t1(x), t2);
}

template<typename FunctorElementType, typename T, typename T2, typename T3>
FunctorElementType pipeline(const FunctorElementType x, T& t1, T2& t2, T3& t3) {
	return pipeline(t1(x), t2, t3);
}

#endif

#endif
