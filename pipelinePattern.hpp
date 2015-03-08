#ifndef PIPELINEPATTERN_HPP
#define PIPELINEPATTERN_HPP

#if __cplusplus >= 201103L
#define USE_CPP11
#endif

#ifdef USE_CPP11

// Defines for C++98 compatibility -> we might want to automate this somehow
#define Action2 Action
#define Action3 Action

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
		
		void operator(uint32_t x) {
			_t1(x);
			Action<T2...>::operator()(x);
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

		void operator() (uint32_t x) {
			_t1(x);
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

		void operator() (uint32_t x) {
			_t1(x);
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

		void operator() (uint32_t x) {
			_t1(x);
			Action<T2>::operator()(x);
		}
	private:
		Action2();
		T& _t1;
};

template<typename T, typename T2, typename T3>
class Action3 : public Action2<T2, T3> {
	public:
		Action3(T& t1, T2& t2, T3& t3) : Action2<T2, T3>(t2, t3), _t1(t1) {
			;
		}

		void operator() (void) {
			_t1();
			Action2<T2, T3>::operator()();
		}

		void operator() (uint32_t x) {
			_t1(x);
			Action2<T2, T3>::operator()(x);
		}
	private:
		Action3();
		T& _t1;
};
#endif

template<typename Func>
void pipeline (Func t) {
    t();
}

template<typename Func>
void elementPipeline(Func t, uint32_t x) {
	t(x);
}

#endif
