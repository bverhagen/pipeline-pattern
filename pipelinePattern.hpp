#ifndef PIPELINEPATTERN_HPP
#define PIPELINEPATTERN_HPP

#if __cplusplus >= 201103L
#define USE_CPP11
#endif

#ifdef USE_CPP11

#include <type_traits>

// Defines for C++98 compatibility -> we might want to automate this somehow
#define Action2 Action
#define Action3 Action

template<typename FunctorElementType, typename T, typename... T2>
class Action : public Action<FunctorElementType, T2...>  {
    public:
		Action() = delete;

        Action(T& t1, T2&... t2) : _t1(t1), Action<FunctorElementType, T2...>(t2...) {
			;
        }

        void operator() (FunctorElementType x) {
            _t1(x);
            Action<FunctorElementType, T2...>::operator()(x);
        }

    private:
	    T& _t1;
};

// Template specialisation for void type
template<typename T, typename... T2>
class Action<void, T, T2...> : public Action<void, T2...>  {
    public:
		Action() = delete;

        Action(T& t1, T2&... t2) : _t1(t1), Action<void, T2...>(t2...) {
			;
        }

        void operator() (void) {
            _t1();
            Action<void, T2...>::operator()();
        }

    private:
	    T& _t1;
};

template<typename FunctorElementType, typename T>
class Action<FunctorElementType, T> {
    public:
		Action() = delete;

		Action(T& t1) : _t1(t1) {
			;
		}

		void operator() (FunctorElementType x) {
			_t1(x);
		}

    private:
        T& _t1;
};

// Template specialisation for void functor type
template<typename T>
class Action<void, T> {
    public:
		Action() = delete;

		Action(T& t1) : _t1(t1) {
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
class Action {
    public:
		Action(T& t1) : _t1(t1) {
			;
		}

		void operator() (FunctorType x) {
			_t1(x);
		}

    private:
		Action();
        T& _t1;
};

template<typename T>
class Action<void, T> {
    public:
		Action(T& t1) : _t1(t1) {
			;
		}

		void operator() (void) {
			_t1();
		}

    private:
		Action();
        T& _t1;
};

template<typename FunctorType, typename T, typename T2>
class Action2 : public Action<FunctorType, T2> {
	public:
		Action2(T& t1, T2& t2) : Action<FunctorType, T2>(t2), _t1(t1) {
			;
		}

		void operator() (FunctorType x) {
			_t1(x);
			Action<FunctorType, T2>::operator()(x);
		}
	private:
		Action2();
		T& _t1;
};

template<typename T, typename T2>
class Action2<void, T, T2> : public Action<void, T2> {
	public:
		Action2(T& t1, T2& t2) : Action<void, T2>(t2), _t1(t1) {
			;
		}

		void operator() (void) {
			_t1();
			Action<void, T2>::operator()();
		}
	private:
		Action2();
		T& _t1;
};

template<typename FunctorType, typename T, typename T2, typename T3>
class Action3 : public Action2<FunctorType, T2, T3> {
	public:
		Action3(T& t1, T2& t2, T3& t3) : Action2<FunctorType, T2, T3>(t2, t3), _t1(t1) {
			;
		}

		void operator() (FunctorType x) {
			_t1(x);
			Action2<FunctorType, T2, T3>::operator()(x);
		}
	private:
		Action3();
		T& _t1;
};

template<typename T, typename T2, typename T3>
class Action3<void, T, T2, T3> : public Action2<void, T2, T3> {
	public:
		Action3(T& t1, T2& t2, T3& t3) : Action2<void, T2, T3>(t2, t3), _t1(t1) {
			;
		}

		void operator() (void) {
			_t1();
			Action2<void, T2, T3>::operator()();
		}
	private:
		Action3();
		T& _t1;
};
#endif

#endif
