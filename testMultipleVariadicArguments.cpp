#include <stdint.h>
#include <iostream>
#include <string>

#include <catch.hpp>

#include "multipleVariadicArguments.hpp"
#include "testMultipleVariadicArgumentsObjects.hpp"

using std::string;

SCENARIO("Part of the solution to having multiple variadic arguments is knowing where the separator is located") {
	GIVEN("Function calls to the separator handler") {
		WHEN("We call the separator with only the separator object value") {
			THEN("The separator exists in the list") {
				REQUIRE(hasSeparator(multipleVariadicArgumentSeparator) == true);
			}
			THEN("We must find the corresponding indices") {
				REQUIRE(MultipleVariadicArgumentSeparatorHandler<MultipleVariadicArgumentSeparator>::SeparatorPosition == 0U);
				REQUIRE(findSeparatorPosition(multipleVariadicArgumentSeparator) == 0U);
			}
		}
		WHEN("We call the separator with no separator object values") {
			uint32_t testObject1 = 5U;
			string testObject2("test");
			double testObject3 = 0.9;
			THEN("The separator does not exist in the list") {
				REQUIRE(hasSeparator(testObject1) == false);
				REQUIRE(hasSeparator(testObject1, testObject2) == false);
				REQUIRE(hasSeparator(testObject1, testObject2, testObject3) == false);
			}
			THEN("We must find the size indices") {
				uint32_t sepPosition1 = MultipleVariadicArgumentSeparatorHandler<uint32_t>::SeparatorPosition;
				REQUIRE(sepPosition1 == 1U);
				REQUIRE(findSeparatorPosition(testObject1) == 1U);

				uint32_t sepPosition2 = MultipleVariadicArgumentSeparatorHandler<uint32_t, string>::SeparatorPosition;
				REQUIRE(sepPosition2 == 2U);
				REQUIRE(findSeparatorPosition(testObject1, testObject2) == 2U);

				uint32_t sepPosition3 = MultipleVariadicArgumentSeparatorHandler<uint32_t, string, double>::SeparatorPosition;
				REQUIRE(sepPosition3 == 3U);
				REQUIRE(findSeparatorPosition(testObject1, testObject2, testObject3) == 3U);
			}
		}
		WHEN("We call the separator with some objects before the separator object value") {
			uint32_t testObject1 = 5U;
			THEN("The separator exists in the list") {
				REQUIRE(hasSeparator(testObject1, multipleVariadicArgumentSeparator) == true);
			}
			THEN("We must find the corresponding indices") {
				uint32_t sepPosition = MultipleVariadicArgumentSeparatorHandler<uint32_t, MultipleVariadicArgumentSeparator>::SeparatorPosition;
				REQUIRE(sepPosition == 1U);
				REQUIRE(findSeparatorPosition(testObject1, multipleVariadicArgumentSeparator) == 1U);
			}
		}
		WHEN("We call the separator with some objects after the separator object value") {
			uint32_t testObject1 = 5U;
			THEN("The separator exists in the list") {
				REQUIRE(hasSeparator(multipleVariadicArgumentSeparator, testObject1) == true);
			}
			THEN("We must find the corresponding indices") {
				uint32_t sepPosition = MultipleVariadicArgumentSeparatorHandler<MultipleVariadicArgumentSeparator, uint32_t>::SeparatorPosition;
				REQUIRE(sepPosition == 0U);
				REQUIRE(findSeparatorPosition(multipleVariadicArgumentSeparator, testObject1) == 0U);
			}
		}
		WHEN("We call the separator with some objects before and after the separator object value") {
			uint32_t testObject1 = 5U;
			string testObject2("Test");
			THEN("The separator exists in the list") {
				REQUIRE(hasSeparator(testObject2, multipleVariadicArgumentSeparator, testObject1) == true);
			}
			THEN("We must find the corresponding indices") {
				uint32_t sepPosition = MultipleVariadicArgumentSeparatorHandler<string, MultipleVariadicArgumentSeparator, uint32_t>::SeparatorPosition;
				REQUIRE(sepPosition == 1U);
				REQUIRE(findSeparatorPosition(testObject2, multipleVariadicArgumentSeparator, testObject1) == 1U);
			}
		}
		WHEN("We invent some more difficult scenario's") {
			THEN("They must also effortlessly succeed") {
				REQUIRE(findSeparatorPosition(5U, 7U, "hello", multipleVariadicArgumentSeparator, 7.0, 5.0f) == 3U);
				REQUIRE(findSeparatorPosition(5U, 7U, "hello", 7.0, 5.0f, multipleVariadicArgumentSeparator, "World") == 5U);
			}
		}
		WHEN("We call the separator with no values at all") {
			THEN("The separator does not exist in the list") {
				REQUIRE(hasSeparator() == false);
			}
			THEN("We must find the corresponding indices") {
				REQUIRE(MultipleVariadicArgumentSeparatorHandler<>::SeparatorPosition == 0U);
				REQUIRE(findSeparatorPosition() == 0U);
			}
		}
	}
}

SCENARIO("Call a function using multiple variadic arguments") {
	GIVEN("A functor accepting the exact number of the argument pack") {
		const uint32_t initValue = MultipleVariadicArgumentTestObject::getInitialIntValue();
		const uint32_t newValue = initValue + 5U;
		const double initDoubleValue = MultipleVariadicArgumentTestObject::getInitialDoubleValue();
		const double newDoubleValue = initDoubleValue * 5;

		MultipleVariadicArgumentTestObject testObject;

		WHEN("We try to call this functor with no arguments") {
			callMultipleArgumentFunction(testObject);

			THEN("No values of the test object should have changed") {
				REQUIRE(testObject.getIntValue1() == initValue);	
				REQUIRE(testObject.getIntValue2() == initValue);	
				REQUIRE(testObject.getDoubleValue1() == initValue);	
			}
		}

		WHEN("We call the test object with a specified number of arguments") {
			callMultipleArgumentFunction(testObject, newValue);

			THEN("The corresponding values of the test object must have changed") {
				REQUIRE(testObject.getIntValue1() == newValue);	
				REQUIRE(testObject.getIntValue2() == initValue);	
				REQUIRE(testObject.getDoubleValue1() == initValue);	
			}
		}

		WHEN("We call the test object with a specified number of arguments") {
			callMultipleArgumentFunction(testObject, newValue, newValue);

			THEN("The corresponding values of the test object must have changed") {
				REQUIRE(testObject.getIntValue1() == newValue);	
				REQUIRE(testObject.getIntValue2() == newValue);	
				REQUIRE(testObject.getDoubleValue1() == initValue);	
			}
		}

		WHEN("We call the test object with a specified number of arguments") {
			callMultipleArgumentFunction(testObject, newValue, newDoubleValue);

			THEN("The corresponding values of the test object must have changed") {
				REQUIRE(testObject.getIntValue1() == newValue);	
				REQUIRE(testObject.getIntValue2() == initValue);	
				REQUIRE(testObject.getDoubleValue1() == newDoubleValue);	
			}
		}

		WHEN("We call the test object with a specified number of arguments") {
			callMultipleArgumentFunction(testObject, newValue, newDoubleValue, newValue);

			THEN("The corresponding values of the test object must have changed") {
				REQUIRE(testObject.getIntValue1() == newValue);	
				REQUIRE(testObject.getIntValue2() == newValue);	
				REQUIRE(testObject.getDoubleValue1() == newDoubleValue);	
			}
		}

		WHEN("We call the test object number of arguments before the separator") {
			callMultipleArgumentFunction(testObject, newValue, newDoubleValue, newValue, multipleVariadicArgumentSeparator);

			THEN("The corresponding values of the test object must have changed") {
				REQUIRE(testObject.getIntValue1() == newValue);	
				REQUIRE(testObject.getIntValue2() == newValue);	
				REQUIRE(testObject.getDoubleValue1() == newDoubleValue);	
			}
		}
	}
}
