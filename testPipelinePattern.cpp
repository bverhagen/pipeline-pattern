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
#include <stdint.h>
#include <catch.hpp>
#include <iostream>


#include "pipelinePattern.hpp"

class Object1 {
    public:
		Object1(void) : _nbOfTimesCalled(0U) {
			;
		}

        void operator() (void) {
			++_nbOfTimesCalled;
        }

		uint32_t getNbOfTimesCalled(void) {
			return _nbOfTimesCalled;
		}

	private:
		uint32_t _nbOfTimesCalled; 
};

class Object2 {
    public: 
		Object2(void) : _nbOfTimesCalled(0U) {
			;
		}

        void operator() (void) {
			++_nbOfTimesCalled;
        }

		uint32_t getNbOfTimesCalled(void) {
			return _nbOfTimesCalled;
		}

	private:
		uint32_t _nbOfTimesCalled;
};

class Object3 {
    public: 
		Object3(void) : _nbOfTimesCalled(0U) {
			;
		}

        void operator() (void) {
			++_nbOfTimesCalled;
        }

		uint32_t getNbOfTimesCalled(void) {
			return _nbOfTimesCalled;
		}
	private:
		uint32_t _nbOfTimesCalled;
};

SCENARIO( "If we use the pipeline, objects are called the defined number of times", "[generalTest]" ) {
	GIVEN("Three initialized objects") {
	    Object1 o1;
		Object2 o2;
		Object3 o3;

		WHEN("We call the pipeline with one object as action") {
			Action<Object1> action(o1);
			pipeline(action);

			THEN("We should have called the object") {
				REQUIRE(o1.getNbOfTimesCalled() == 1);
			}
		}

		WHEN("We call the pipeline with one object as action ten times") {
			const uint8_t nbOfTimesToLoop = 10U;

			Action<Object1> action(o1);
			for(uint8_t i = 0; i < nbOfTimesToLoop; ++i) {
				pipeline(action);
			}

			THEN("We should have called the object") {
				REQUIRE(o1.getNbOfTimesCalled() == nbOfTimesToLoop);
			}
		}

		WHEN("We call the pipeline with these two objects as action") {
			Action2<Object1, Object2> action(o1, o2);
			pipeline(action);

			THEN("We should have called all objects once") {
				REQUIRE(o1.getNbOfTimesCalled() == 1);
				REQUIRE(o1.getNbOfTimesCalled() == 1);
			}
		}
	}
}
