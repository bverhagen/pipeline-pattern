/*
 * =====================================================================================
 *
 *       Filename:  testPerformance.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/19/2015 07:51:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bart Verhagen (bv), bart.verhagen@tass.be
 *   Organization:  TASS
 *
 * =====================================================================================
 */
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <iostream>

#include "pipelinePattern.hpp"

using std::vector;
using std::srand;
using std::rand;
using std::cout;
using std::endl;

using namespace std::chrono;

class Shift {
	public:
		Shift(const uint8_t shiftAmount) : _shiftAmount(shiftAmount) {
			;
		}

		uint32_t operator() (uint32_t x) {
			return x << _shiftAmount;
		}
	
	private:
		const uint8_t _shiftAmount;
};

class Increase {
	public:
		Increase(const uint32_t amount) : _amount(amount) {
			;
		}

		uint32_t operator() (uint32_t x) {
			return x + _amount;
		}
	private:
		const uint32_t _amount;
};

int main(int argc, char** argv) {
	uint32_t testSize = 1024;
	if(argc > 1) {
		testSize = strtol(argv[1], 0, 10);
	}

	srand(0);
	vector<uint32_t> testMatrix;
	testMatrix.reserve(testSize);
	for(auto it = testMatrix.begin(); it != testMatrix.end(); ++it) {
		*it = rand();
	}	

	Shift shift(2);
	Increase increase(2);

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	vector<uint32_t> unrolledResult;
	unrolledResult.reserve(testSize);
	for(uint32_t i = 0; i < testSize; ++i) {
		unrolledResult[i] = increase(shift(testMatrix[i]));
	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
  	cout << "Pipeline: " << time_span.count() << " seconds." << endl;

	t1 = high_resolution_clock::now();
	vector<uint32_t> pipelineResult;
	pipelineResult.reserve(testSize);
	Pipeline<uint32_t, Shift, Increase> pipeline(shift, increase);
	for(uint32_t i = 0; i < testSize; ++i) {
		pipelineResult[i] = pipeline(testMatrix[i]);	
	}
	t2 = high_resolution_clock::now();

	time_span = duration_cast<duration<double>>(t2 - t1);
  	cout << "Generated Pipeline: " << time_span.count() << " seconds." << endl;

	t1 = high_resolution_clock::now();
	vector<uint32_t> directResult;
	directResult.reserve(testSize);
	for(uint32_t i = 0; i < testSize; ++i) {
		directResult[i] = shift(testMatrix[i]);
	}
	for(uint32_t i = 0; i < testSize; ++i) {
		directResult[i] = increase(directResult[i]);
	}
	t2 = high_resolution_clock::now();

	time_span = duration_cast<duration<double>>(t2 - t1);
  	cout << "Sequential (non-it): " << time_span.count() << " seconds." << endl;

	t1 = high_resolution_clock::now();
	vector<uint32_t> directResultIt;
	directResultIt.reserve(testSize);
	for(uint32_t i = 0; i < testSize; ++i) {
		directResultIt[i] = shift(testMatrix[i]);
	}
	for(auto it = directResultIt.begin(); it != directResultIt.end(); ++it) {
		*it = increase(*it);
	}
	t2 = high_resolution_clock::now();

	time_span = duration_cast<duration<double>>(t2 - t1);
  	cout << "Sequential (it): " << time_span.count() << " seconds." << endl;
}
