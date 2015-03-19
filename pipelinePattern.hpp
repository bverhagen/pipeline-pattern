#ifndef PIPELINEPATTERN_HPP
#define PIPELINEPATTERN_HPP

#if __cplusplus >= 201103L
#define USE_CPP11
#endif

#ifdef USE_CPP11

// Defines for C++98 compatibility -> we might want to automate this somehow
#define Pipeline2 Pipeline
#define Pipeline3 Pipeline

template<typename FunctorElementType, typename T, typename... T2>
class Pipeline : public Pipeline<FunctorElementType, T2...>  {
    public:
		Pipeline() = delete;

        Pipeline(T& t1, T2&... t2) : _t1(t1), Pipeline<FunctorElementType, T2...>(t2...) {
			;
        }

        FunctorElementType operator() (const FunctorElementType x) {
            return Pipeline<FunctorElementType, T2...>::operator()(_t1(x));
        }

    private:
	    T& _t1;
};

// Template specialisation for void type
template<typename T, typename... T2>
class Pipeline<void, T, T2...> : public Pipeline<void, T2...>  {
    public:
		Pipeline() = delete;

        Pipeline(T& t1, T2&... t2) : _t1(t1), Pipeline<void, T2...>(t2...) {
			;
        }

        void operator() (void) {
            _t1();
            Pipeline<void, T2...>::operator()();
        }

    private:
	    T& _t1;
};

template<typename FunctorElementType, typename T>
class Pipeline<FunctorElementType, T> {
    public:
		Pipeline() = delete;

		Pipeline(T& t1) : _t1(t1) {
			;
		}

		FunctorElementType operator() (const FunctorElementType x) {
			return _t1(x);
		}

    private:
        T& _t1;
};

// Template specialisation for void functor type
template<typename T>
class Pipeline<void, T> {
    public:
		Pipeline() = delete;

		Pipeline(T& t1) : _t1(t1) {
			;
		}

		void operator() (void) {
			_t1();
		}

    private:
        T& _t1;
};

#else

template<typename FunctorType, typename T>
class Pipeline {
    public:
		Pipeline(T& t1) : _t1(t1) {
			;
		}

		FunctorType operator() (const FunctorType x) {
			_t1(x);
		}

    private:
		Pipeline();
        T& _t1;
};

template<typename T>
class Pipeline<void, T> {
    public:
		Pipeline(T& t1) : _t1(t1) {
			;
		}

		void operator() (void) {
			_t1();
		}

    private:
		Pipeline();
        T& _t1;
};

template<typename FunctorType, typename T, typename T2>
class Pipeline2 : public Pipeline<FunctorType, T2> {
	public:
		Pipeline2(T& t1, T2& t2) : Pipeline<FunctorType, T2>(t2), _t1(t1) {
			;
		}

		FunctorType operator() (const FunctorType x) {
			return Pipeline<FunctorType, T2>::operator()(_t1(x));
		}

	private:
		Pipeline2();
		T& _t1;
};

template<typename T, typename T2>
class Pipeline2<void, T, T2> : public Pipeline<void, T2> {
	public:
		Pipeline2(T& t1, T2& t2) : Pipeline<void, T2>(t2), _t1(t1) {
			;
		}

		void operator() (void) {
			_t1();
			Pipeline<void, T2>::operator()();
		}
	private:
		Pipeline2();
		T& _t1;
};

template<typename FunctorType, typename T, typename T2, typename T3>
class Pipeline3 : public Pipeline2<FunctorType, T2, T3> {
	public:
		Pipeline3(T& t1, T2& t2, T3& t3) : Pipeline2<FunctorType, T2, T3>(t2, t3), _t1(t1) {
			;
		}

		FunctorType operator() (const FunctorType x) {
			return Pipeline2<FunctorType, T2, T3>::operator()(_t1(x));
		}
	private:
		Pipeline3();
		T& _t1;
};

template<typename T, typename T2, typename T3>
class Pipeline3<void, T, T2, T3> : public Pipeline2<void, T2, T3> {
	public:
		Pipeline3(T& t1, T2& t2, T3& t3) : Pipeline2<void, T2, T3>(t2, t3), _t1(t1) {
			;
		}

		void operator() (void) {
			_t1();
			Pipeline2<void, T2, T3>::operator()();
		}
	private:
		Pipeline3();
		T& _t1;
};
#endif

#endif
