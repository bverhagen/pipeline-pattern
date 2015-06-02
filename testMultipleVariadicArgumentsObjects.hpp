#ifndef TEST_MULTIPLE_VARIADIC_ARGUMENTS_OBJECTS
#define TEST_MULTIPLE_VARIADIC_ARGUMENTS_OBJECTS

#include <cstdint>
#include <iostream>

class MultipleVariadicArgumentTestObject {
	public:
		MultipleVariadicArgumentTestObject() : 
			_nbOfTimesEmptyFunctorCalled(0U),
			_intValue1(getInitialIntValue()), 
			_intValue2(getInitialIntValue()), 
			_doubleValue1(getInitialDoubleValue()) {
			;
		}

		void operator()(void) {
			++_nbOfTimesEmptyFunctorCalled;
		}

		void operator()(uint32_t intValue) {
			_intValue1 = intValue;
		}

		void operator()(double doubleValue) {
			_doubleValue1 = doubleValue;
		}
		 
		void operator()(uint32_t intValue1, uint32_t intValue2) {
			_intValue1 = intValue1;
			_intValue2 = intValue2;
		}

		void operator()(uint32_t intValue, double doubleValue) {
			_intValue1 = intValue;
			_doubleValue1 = doubleValue;
		}

		void operator()(uint32_t intValue1, double doubleValue, uint32_t intValue2) {
			_intValue1 = intValue1;
			_doubleValue1 = doubleValue;
			_intValue2 = intValue2;
		}
		
		uint32_t getNbOfTimesEmptyFunctorCalled(void) {
			return _nbOfTimesEmptyFunctorCalled;
		}

		uint32_t getIntValue1(void) {
			return _intValue1;
		}
		
		uint32_t getIntValue2(void) {
			return _intValue2;
		}

		uint32_t getDoubleValue1(void) {
			return _doubleValue1;
		}

		static uint32_t getInitialIntValue(void) {
			return 0U;
		}

		static uint32_t getInitialDoubleValue(void) {
			return 0.0;
		}

	private:
		uint32_t _nbOfTimesEmptyFunctorCalled;
		uint32_t _intValue1;
		uint32_t _intValue2;
		double _doubleValue1;
};

#endif
