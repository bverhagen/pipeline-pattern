#include <iostream>

using std::cout;
using std::endl;

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

		// TODO: declare move constructor
        Action(T t1, T2... t2...) : _t1(t1), Action<T2...>(t2...) {
            cout << "Created action object" << endl;
        }

        void operator() (void) {
            cout << "In T2 Action operator()" << endl;
            _t1();
            Action<T2...>::operator()();
        }
    private:
	    T _t1;
};

template<typename T>
class Action<T> {
    public:
		// Explicitly deny access to default constructor
		Action() = delete;

		// TODO declare move constructor
		Action(T t1) : _t1(t1) {
			cout << "Created t1 with constructor" << endl;
		}

        void operator() (void) {
            cout << "In t1 action operator()" << endl;
            _t1();
        }

    private:
        T _t1;
};

#else

template<typename T>
class Action {
    public:
		// TODO declare move constructor
		Action(T t1) : _t1(t1) {
			cout << "Created t1 with constructor" << endl;
		}

        void operator() (void) {
            cout << "In t1 action operator()" << endl;
            _t1();
        }

    private:
		Action();
        T _t1;
};

template<typename T, typename T2>
class Action2 : public Action<T2> {
	public:
		Action2(T t1, T2 t2) : Action<T2>(t2), _t1(t1) {
			cout << "Created action object" << endl;
		}

		void operator() (void) {
            cout << "In T2 Action operator()" << endl;
            _t1();
            Action<T2>::operator()();
        }
	private:
		Action2();
		T _t1;
};
#endif

template<typename Func>
void pipeline (Func t) {
    cout << "In pipeline" << endl;
    t();
}

class Object1 {
    public:
        void operator() (void) {
            cout << "\tThis is object 1" << endl;
        }
};

class Object2 {
    public: 
        void operator() (void) {
            cout << "\tThis is object 2" << endl;
        }
};

class Object3 {
    public: 
        void operator() (void) {
            cout << "\tThis is object 3" << endl;
        }
};

int main(void) {
    Object1 o1;
    Object2 o2;
	Object3 o3;

    Action2<Object1, Object2> action(o1, o2);
    cout << "Made action"  << endl;
    pipeline(action);

    return 0;
}
