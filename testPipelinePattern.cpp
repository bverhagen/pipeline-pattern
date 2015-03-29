/*
 * =====================================================================================
 *
 *       Filename:  testPipelinePattern.cpp
 *
 *    Description:  
 *
 *
 *        Version:  1.0
 *        Created:  03/07/2015 10:17:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bart Verhagen (bv), bart.verhagen@tass.be
 *   Organization:  TASS
 *
 * =====================================================================================
 */

#include <cstdlib>	// Too bad we can not use the C++11 random functions due to C++98 backwards compatibility of the framework :-(
#include <stdint.h>
#include <vector>
#include <limits>
#include <iostream>
#include <string>

#include <catch.hpp>

#include "pipelinePattern.hpp"
#include "testPipelineObjects.hpp"

#if __cplusplus >= 201103L
#define USE_CPP11
#endif

using std::vector;
using std::numeric_limits;
using std::srand;
using std::rand;
using std::string;

SCENARIO( "If we use the pipeline, objects are called the defined number of times", "[generalTest]" ) {
	GIVEN("Three initialized objects") {
		Object1<uint32_t> o1;
		Object2<uint32_t> o2;
		Object3<uint32_t> o3;

		WHEN("We call the pipeline with one object as action") {
			pipeline(1U, o1);

			THEN("We should have called the object") {
				REQUIRE(o1.getNbOfTimesCalled() == 1);
			}
		}

		WHEN("We call the pipeline with one object as action ten times") {
			const uint8_t nbOfTimesToLoop = 10U;

			for(uint8_t i = 0; i < nbOfTimesToLoop; ++i) {
				pipeline(1U, o1);
			}

			THEN("We should have called the object the appropriate number of times") {
				REQUIRE(o1.getNbOfTimesCalled() == nbOfTimesToLoop);
			}
		}

		WHEN("We call the pipeline with these two objects as action") {
			pipeline(1U, o1, o2);

			THEN("We should have called all objects once") {
				REQUIRE(o1.getNbOfTimesCalled() == 1);
				REQUIRE(o1.getNbOfTimesCalled() == 1);
			}
		}
		
		WHEN("We call the pipeline with these three objects as action ten times") {
			const uint8_t nbOfTimesToLoop = 10U;
			for(uint8_t i = 0; i < nbOfTimesToLoop; ++i) {
				pipeline(1U, o1, o2, o3);
			}

			THEN("We should have called all objects the appropriate number of times") {
				REQUIRE(o1.getNbOfTimesCalled() == nbOfTimesToLoop);
				REQUIRE(o2.getNbOfTimesCalled() == nbOfTimesToLoop);
				REQUIRE(o3.getNbOfTimesCalled() == nbOfTimesToLoop);
			}
		}
	}
}

SCENARIO("Using the pipeline on loops in the elements", "[loopTests]") {
	GIVEN("A collection of elements in a certain order and a fully defined Pipeline") {
		const uint32_t nbOfElements = 10;
		const uint32_t lowerLimit = numeric_limits<uint32_t>::min();
		const uint32_t upperLimit = numeric_limits<uint32_t>::max();

		// Construct the collection of numbered elements
		vector<uint32_t> orderedElementsCollection;
		orderedElementsCollection.reserve(nbOfElements);

		srand(0);
		for(uint32_t i = 0; i < nbOfElements; ++i) {
			uint32_t randomInt = rand() % (upperLimit - lowerLimit) + lowerLimit;
			orderedElementsCollection.push_back(randomInt);
		}
		REQUIRE(nbOfElements == orderedElementsCollection.size());

		// Construct the Pipeline
		Object1<uint32_t> o1;
		Object2<uint32_t> o2;
		Object3<uint32_t> o3;

		WHEN("We loop over the elements, calling the pipeline for each element") {
			for(vector<uint32_t>::iterator it = orderedElementsCollection.begin(); it != orderedElementsCollection.end(); ++it) {
				uint32_t result = pipeline(*it, o1, o2, o3);
			}

			THEN("Each of the objects should return the same size for each call to the pipeline") {
				vector<uint32_t> o1Elements = o1.getReceivedOrderedContent();	
				vector<uint32_t> o2Elements = o1.getReceivedOrderedContent();	
				vector<uint32_t> o3Elements = o1.getReceivedOrderedContent();	

				REQUIRE(o1Elements.size() == orderedElementsCollection.size());
				REQUIRE(o2Elements.size() == orderedElementsCollection.size());
				REQUIRE(o3Elements.size() == orderedElementsCollection.size());
			}

			THEN("We should find the same ordered number of elements in each action of the pipeline") {
				vector<uint32_t> o1Elements = o1.getReceivedOrderedContent();	
				vector<uint32_t> o2Elements = o1.getReceivedOrderedContent();	
				vector<uint32_t> o3Elements = o1.getReceivedOrderedContent();	

				for(uint32_t i = 0; i < nbOfElements; ++i) {
					REQUIRE(o1Elements[i] == orderedElementsCollection[i]);
					REQUIRE(o2Elements[i] == orderedElementsCollection[i]);
					REQUIRE(o3Elements[i] == orderedElementsCollection[i]);
				}
			}
		}
	}
}

// Heterogeneous pipeline currently only supported for C++11 and later
#ifdef USE_CPP11
SCENARIO("Using the pipeline on heterogeneous elements", "[heterogeneousTests]") {
	GIVEN("Objects that form a heterogeneous pipeline") {
		typedef string o1Type1;
		typedef uint32_t o1Type2;

		typedef uint32_t o2Type1;	
		typedef string o2Type2;

		typedef string o3Type1;
		typedef uint32_t o3Type2;

		const o1Type1 startValue = "Hello";
		const o1Type2 o1ReturnValue = 1U;
		const o2Type2 o2ReturnValue = "world";
		const o3Type2 o3ReturnValue = 3U;

		HeterogeneousObject1<o1Type1, o1Type2> o1(o1ReturnValue);
		HeterogeneousObject1<o2Type1, o2Type2> o2(o2ReturnValue);
		HeterogeneousObject1<o3Type1, o3Type2> o3(o3ReturnValue);

		WHEN("We apply the pipeline multiple times") {
			const uint32_t nbOfIterations = 10U;

			vector<o3Type2> result;
			for(uint32_t i = 0; i < nbOfIterations; ++i) {
				result.push_back(pipeline(startValue, o1, o2, o3));
			}

			THEN("We should have called each object nbOfIterations number of times") {
				vector<o1Type1> o1Elements = o1.getReceivedOrderedContent();
				vector<o2Type1> o2Elements = o2.getReceivedOrderedContent();
				vector<o3Type1> o3Elements = o3.getReceivedOrderedContent();
				
				REQUIRE(o1Elements.size() == nbOfIterations);
				REQUIRE(o2Elements.size() == nbOfIterations);
				REQUIRE(o3Elements.size() == nbOfIterations);
				REQUIRE(result.size() == nbOfIterations);
			}

			THEN("The objects should have received the return value of the previous object") {
				vector<o1Type1> o1Elements = o1.getReceivedOrderedContent();
				vector<o2Type1> o2Elements = o2.getReceivedOrderedContent();
				vector<o3Type1> o3Elements = o3.getReceivedOrderedContent();

				for(uint32_t i = 0; i < nbOfIterations; ++i) {
					REQUIRE(o1Elements[i] == startValue);
					REQUIRE(o2Elements[i] == o1ReturnValue);
					REQUIRE(o3Elements[i] == o2ReturnValue);
					REQUIRE(result[i] == o3ReturnValue);
				}
			}
		}
	}
}
#endif

SCENARIO("An element occurs multiple times in the pipeline", "[generalTest]") {
	GIVEN("A few elements that we want to put in the pipeline") {
		typedef uint32_t o1Type;
		typedef uint32_t o2Type;
		typedef uint32_t o3Type;

		const o1Type startValue = 1U;
		const o1Type o1ReturnValue = 2U;
		const o2Type o2ReturnValue = 3U;
		const o3Type o3ReturnValue = 4U;

		HeterogeneousObject1<o1Type, o1Type> o1(o1ReturnValue);
		HeterogeneousObject1<o2Type, o2Type> o2(o2ReturnValue);
		HeterogeneousObject1<o3Type, o3Type> o3(o3ReturnValue);
	
		WHEN("We apply some elements multiple times to the pipeline") {
			const uint32_t nbOfIterations = 10U;

			vector<o1Type> result;
			for(uint32_t i = 0; i < nbOfIterations; ++i) {
				result.push_back(pipeline(startValue, o1, o2, o1, o3, o2, o1));
			}

			THEN("Each object should have been called the appropriate number of times") {
				vector<o1Type> o1Elements = o1.getReceivedOrderedContent();
				vector<o2Type> o2Elements = o2.getReceivedOrderedContent();
				vector<o3Type> o3Elements = o3.getReceivedOrderedContent();
				
				REQUIRE(o1Elements.size() == 3*nbOfIterations);
				REQUIRE(o2Elements.size() == 2*nbOfIterations);
				REQUIRE(o3Elements.size() == nbOfIterations);
				REQUIRE(result.size() == nbOfIterations);
			}
		}
	}
}
