#ifndef TESTPIPELINEOBJECTS_HPP
#define TESTPIPELINEOBJECTS_HPP

#include <vector>

	template<typename T>
	class Object1 {
		public:
			Object1(void) : _nbOfTimesCalled(0U), _receivedContent() {
				;
			}

			void operator() (void) {
				++_nbOfTimesCalled;
			}

			T operator() (T x) {
				++_nbOfTimesCalled;
				_receivedContent.push_back(x);
				return x;
			}

			uint32_t getNbOfTimesCalled(void) {
				return _nbOfTimesCalled;
			}

			std::vector<T> getReceivedOrderedContent(void) {
				return _receivedContent;
			}

		private:
			uint32_t _nbOfTimesCalled; 
			std::vector<T> _receivedContent; 
	};

	template <typename T>
	class Object2 {
		public: 
			Object2(void) : _nbOfTimesCalled(0U) {
				;
			}

			void operator() (void) {
				++_nbOfTimesCalled;
			}

			T operator() (T x) {
				++_nbOfTimesCalled;
				_receivedContent.push_back(x);
				return x;
			}

			uint32_t getNbOfTimesCalled(void) {
				return _nbOfTimesCalled;
			}

			std::vector<T> getReceivedOrderedContent(void) {
				return _receivedContent;
			}
		private:
			uint32_t _nbOfTimesCalled;
			std::vector<T> _receivedContent; 
	};

	template <typename T>
	class Object3 {
		public: 
			Object3(void) : _nbOfTimesCalled(0U) {
				;
			}

			void operator() (void) {
				++_nbOfTimesCalled;
			}

			T operator() (T x) {
				++_nbOfTimesCalled;
				_receivedContent.push_back(x);
				return x;
			}

			uint32_t getNbOfTimesCalled(void) {
				return _nbOfTimesCalled;
			}

			std::vector<T> getReceivedOrderedContent(void) {
				return _receivedContent;
			}
		private:
			uint32_t _nbOfTimesCalled;
			std::vector<T> _receivedContent; 
	};

	template <typename T1, typename T2>
	class HeterogeneousObject1 {
		public: 
			HeterogeneousObject1(T2 returnValue) : _nbOfTimesCalled(0U), _returnValue(returnValue) {
				;
			}

			void operator() (void) {
				++_nbOfTimesCalled;
			}

			T2 operator() (T1 x) {
				++_nbOfTimesCalled;
				_receivedContent.push_back(x);
				return _returnValue;
			}

			uint32_t getNbOfTimesCalled(void) {
				return _nbOfTimesCalled;
			}

			std::vector<T1> getReceivedOrderedContent(void) {
				return _receivedContent;
			}
		private:
			uint32_t _nbOfTimesCalled;
			std::vector<T1> _receivedContent; 
			const T2 _returnValue;
	};
#endif
