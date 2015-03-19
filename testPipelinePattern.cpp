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

#include <catch.hpp>

#include "pipelinePattern.hpp"

using std::vector;
using std::numeric_limits;
using std::srand;
using std::rand;

class Object1 {
    public:
		Object1(void) : _nbOfTimesCalled(0U), _receivedContent() {
			;
		}

        void operator() (void) {
			++_nbOfTimesCalled;
        }

		void operator() (uint32_t x) {
			++_nbOfTimesCalled;
			_receivedContent.push_back(x);
		}

		uint32_t getNbOfTimesCalled(void) {
			return _nbOfTimesCalled;
		}

		vector<uint32_t> getReceivedOrderedContent(void) {
			return _receivedContent;
		}

	private:
		uint32_t _nbOfTimesCalled; 
		vector<uint32_t> _receivedContent; 
};

class Object2 {
    public: 
		Object2(void) : _nbOfTimesCalled(0U) {
			;
		}

        void operator() (void) {
			++_nbOfTimesCalled;
        }

		void operator() (uint32_t x) {
			++_nbOfTimesCalled;
			_receivedContent.push_back(x);
		}

		uint32_t getNbOfTimesCalled(void) {
			return _nbOfTimesCalled;
		}

		vector<uint32_t> getReceivedOrderedContent(void) {
			return _receivedContent;
		}
	private:
		uint32_t _nbOfTimesCalled;
		vector<uint32_t> _receivedContent; 
};

class Object3 {
    public: 
		Object3(void) : _nbOfTimesCalled(0U) {
			;
		}

        void operator() (void) {
			++_nbOfTimesCalled;
        }

		void operator() (uint32_t x) {
			++_nbOfTimesCalled;
			_receivedContent.push_back(x);
		}

		uint32_t getNbOfTimesCalled(void) {
			return _nbOfTimesCalled;
		}

		vector<uint32_t> getReceivedOrderedContent(void) {
			return _receivedContent;
		}
	private:
		uint32_t _nbOfTimesCalled;
		vector<uint32_t> _receivedContent; 
};

SCENARIO( "If we use the pipeline, objects are called the defined number of times", "[generalTest]" ) {
	GIVEN("Three initialized objects") {
	    Object1 o1;
		Object2 o2;
		Object3 o3;

		WHEN("We call the pipeline with one object as action") {
			Pipeline<void, Object1> action(o1);
			action();

			THEN("We should have called the object") {
				REQUIRE(o1.getNbOfTimesCalled() == 1);
			}
		}

		WHEN("We call the pipeline with one object as action ten times") {
			const uint8_t nbOfTimesToLoop = 10U;

			Pipeline<void, Object1> action(o1);
			for(uint8_t i = 0; i < nbOfTimesToLoop; ++i) {
				action();
			}

			THEN("We should have called the object the appropriate number of times") {
				REQUIRE(o1.getNbOfTimesCalled() == nbOfTimesToLoop);
			}
		}

		WHEN("We call the pipeline with these two objects as action") {
			Pipeline2<void, Object1, Object2> action(o1, o2);
			action();

			THEN("We should have called all objects once") {
				REQUIRE(o1.getNbOfTimesCalled() == 1);
				REQUIRE(o1.getNbOfTimesCalled() == 1);
			}
		}
		
		WHEN("We call the pipeline with these three objects as action ten times") {
			const uint8_t nbOfTimesToLoop = 10U;
			Pipeline3<void, Object1, Object2, Object3> action(o1, o2, o3);
			for(uint8_t i = 0; i < nbOfTimesToLoop; ++i) {
				action();
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
		Object1 o1;
		Object2 o2;
		Object3 o3;
		Pipeline3<uint32_t, Object1, Object2, Object3> action(o1, o2, o3);

		WHEN("We loop over the elements, calling the pipeline for each element") {
			for(vector<uint32_t>::iterator it = orderedElementsCollection.begin(); it != orderedElementsCollection.end(); ++it) {
				action(*it);
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
