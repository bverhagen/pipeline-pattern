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
#include <limits>

#include "pipelinePattern.hpp"

using std::vector;
using std::srand;
using std::rand;
using std::numeric_limits;
using std::cout;
using std::cerr;
using std::endl;

using namespace std::chrono;

class Shift {
	public:
		Shift(const uint8_t shiftAmount) : _shiftAmount(shiftAmount) {
			;
		}

		uint32_t operator() (uint32_t x) const {
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

		uint32_t operator() (uint32_t x) const {
			return x + _amount;
		}
	private:
		const uint32_t _amount;
};

class Square {
	public:
		Square(void) {
			;
		}

		uint32_t operator() (uint32_t x) const {
			return x*x;
		}
};
template<typename T>
void manual(uint32_t testSize, const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	vector<T> unrolledResult;
	unrolledResult.reserve(testSize);
	auto unrolledResultIt = unrolledResult.begin();
	for(auto testMatrixIt = testMatrix.begin(); testMatrixIt != testMatrix.end(); ++testMatrixIt, ++unrolledResultIt) {
		*unrolledResultIt = square(increase(shift(*testMatrixIt)));
	}
}

template<typename T>
void pipelineTest(uint32_t testSize, const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	vector<T> pipelineResult;
	pipelineResult.reserve(testSize);
	auto pipelineResultIt = pipelineResult.begin();
	for(auto testMatrixIt = testMatrix.begin(); testMatrixIt != testMatrix.end(); ++testMatrixIt, ++pipelineResultIt) {
		*pipelineResultIt = pipeline(*testMatrixIt, shift, increase, square);	
	}
}

template<typename T>
void stages(uint32_t testSize, const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	vector<uint32_t> directResult;
	directResult.reserve(testSize);
	auto directResultIt = directResult.begin();
	for(auto testMatrixIt = testMatrix.begin(); testMatrixIt != testMatrix.end(); ++testMatrixIt, ++directResultIt) {
		*directResultIt = shift(*testMatrixIt);
	}
	for(auto it = directResult.begin(); it != directResult.end(); ++it) {
		*it = increase(*it);
	}
	for(auto it = directResult.begin(); it != directResult.end(); ++it) {
		*it = square(*it);
	}
}

int main(int argc, char** argv) {
	uint32_t testSize = 1024U;
	uint32_t nbOfIterations = 1U;
	if(argc > 1) {
		uint64_t inputSize = strtol(argv[1], 0, 10);
		if(inputSize < numeric_limits<uint32_t>::max()) {
			testSize = inputSize;
		} else {
			cerr << "Error: input size does not fit in 32-bit variable" << endl;
			return 1;
		}
	}

	if(argc > 2) {
		nbOfIterations = strtol(argv[2], 0, 10);
	}
 	
	duration<double> time_span_manual = duration<double>::zero();
	duration<double> time_span_pipeline = duration<double>::zero();
	duration<double> time_span_stages = duration<double>::zero();

	for(auto i = 0; i < nbOfIterations; ++i) {
		srand(0);
		vector<uint32_t> testMatrix;
		testMatrix.reserve(testSize);
		for(int j = 0; j < testSize; ++j) {
			testMatrix.push_back(rand());
		}	

		Shift shift(2);
		Increase increase(2);
		Square square;

		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		manual(testSize, testMatrix, shift, increase, square);	
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		time_span_manual += duration_cast<duration<double>>(t2 - t1);

		t1 = high_resolution_clock::now();
		pipelineTest(testSize, testMatrix, shift, increase, square);	
		t2 = high_resolution_clock::now();
		time_span_pipeline += duration_cast<duration<double>>(t2 - t1);

		t1 = high_resolution_clock::now();
		stages(testSize, testMatrix, shift, increase, square);	
		t2 = high_resolution_clock::now();
		time_span_stages += duration_cast<duration<double>>(t2 - t1);
	}
	cout << "Generated manual: average: " << time_span_manual.count() / double(nbOfIterations) << " seconds." << endl;
	cout << "Generated Pipeline: average: " << time_span_pipeline.count() / double(nbOfIterations) << " seconds." << endl;
	cout << "Sequential: average: " << time_span_stages.count() / double(nbOfIterations) << " seconds." << endl;
}
