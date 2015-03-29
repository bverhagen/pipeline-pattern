#ifndef PIPELINEPATTERN_HPP
#define PIPELINEPATTERN_HPP

#if __cplusplus >= 201103L
#define USE_CPP11
#endif

#ifdef USE_CPP11
template<typename FunctorElementType, typename T>
auto pipeline(const FunctorElementType x, T& t1) -> decltype(t1(x)) {
	return t1(x);
}

template<typename FunctorElementType, typename T, typename... T2>
auto pipeline(const FunctorElementType x, T& t1, T2&... t2) -> decltype(pipeline(t1(x), t2...)) {
	return pipeline(t1(x), t2...);
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

template<typename FunctorElementType, typename T, typename T2, typename T3, typename T4>
FunctorElementType pipeline(const FunctorElementType x, T& t1, T2& t2, T3& t3, T4& t4) {
	return pipeline(t1(x), t2, t3, t4);
}

template<typename FunctorElementType, typename T, typename T2, typename T3, typename T4, typename T5>
FunctorElementType pipeline(const FunctorElementType x, T& t1, T2& t2, T3& t3, T4& t4, T5& t5) {
	return pipeline(t1(x), t2, t3, t4, t5);
}

template<typename FunctorElementType, typename T, typename T2, typename T3, typename T4, typename T5, typename T6>
FunctorElementType pipeline(const FunctorElementType x, T& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6) {
	return pipeline(t1(x), t2, t3, t4, t5, t6);
}
#endif
#endif
