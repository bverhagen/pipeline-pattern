#ifndef PIPELINEPATTERN_HPP
#define PIPELINEPATTERN_HPP

#if __cplusplus >= 201103L
#define USE_CPP11
#endif

#ifdef USE_CPP11

#define Action2 Action

template<typename T, typename... T2>
class Action : public Action<T2...> {
    public:
		// Explicitly deny access to default constructor
		Action() = delete;

        Action(T& t1, T2... t2...) : _t1(t1), Action<T2...>(t2...) {
			;
        }

        void operator() (void) {
            _t1();
            Action<T2...>::operator()();
        }
    private:
	    T& _t1;
};

template<typename T>
class Action<T> {
    public:
		// Explicitly deny access to default constructor
		Action() = delete;

		// TODO declare move constructor
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

template<typename T>
class Action {
    public:
		// TODO declare move constructor
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

template<typename T, typename T2>
class Action2 : public Action<T2> {
	public:
		Action2(T& t1, T2& t2) : Action<T2>(t2), _t1(t1) {
			;
		}

		void operator() (void) {
            _t1();
            Action<T2>::operator()();
        }
	private:
		Action2();
		T& _t1;
};
#endif

template<typename Func>
void pipeline (Func t) {
    t();
}

#endif
