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
#include <cassert>
#include <string>

#include "pipelinePattern.hpp"
#include "dummy.hpp"

using std::vector;
using std::srand;
using std::rand;
using std::numeric_limits;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

using namespace std::chrono;

const double minimumExecutionTime = 2.0;

enum PerformanceOptions {
	PerformanceOptions_Manual = 0,
	PerformanceOptions_Pipeline = 1,
	PerformanceOptions_Stages = 2,
	PerformanceOptions_StagesNoInvervention = 3,
} PerformanceOptions_t;

class Shift {
	public:
		Shift(const uint8_t shiftAmount) : _shiftAmount(shiftAmount) {
			;
		}

		inline uint32_t operator() (uint32_t x) const {
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

inline void showResult(std::string msg, duration<double> time, uint32_t nbOfIterations) {
	cout << msg << time.count() / double(nbOfIterations) << " seconds (" << nbOfIterations << " iterations)" << endl;
}

template<typename T>
vector<T> manual(uint32_t testSize, const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	vector<T> unrolledResult;
	unrolledResult.reserve(testSize);
	auto unrolledResultIt = unrolledResult.begin();
	for(auto testMatrixIt = testMatrix.begin(); testMatrixIt != testMatrix.end(); ++testMatrixIt, ++unrolledResultIt) {
		*unrolledResultIt = square(increase(shift(*testMatrixIt)));
	}
	return unrolledResult;
}

template<typename T>
vector<T> pipelineTest(uint32_t testSize, const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	vector<T> pipelineResult;
	pipelineResult.reserve(testSize);
	auto pipelineResultIt = pipelineResult.begin();
	for(auto testMatrixIt = testMatrix.begin(); testMatrixIt != testMatrix.end(); ++testMatrixIt, ++pipelineResultIt) {
		*pipelineResultIt = pipeline(*testMatrixIt, shift, increase, square);	
	}
	return pipelineResult;
}

template<typename T>
vector<T> stages(uint32_t testSize, const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	vector<uint32_t> directResult;
	directResult.reserve(testSize);
	auto directResultIt = directResult.begin();
	for(auto testMatrixIt = testMatrix.begin(); testMatrixIt != testMatrix.end(); ++testMatrixIt, ++directResultIt) {
		*directResultIt = shift(*testMatrixIt);
	}
	if(alwaysReturnFalse()) {
		dummy();
	}
	for(auto it = directResult.begin(); it != directResult.end(); ++it) {
		*it = increase(*it);
	}
	if(alwaysReturnFalse()) {
		dummy();
	}
	for(auto it = directResult.begin(); it != directResult.end(); ++it) {
		*it = square(*it);
	}
	return directResult;
}

template<typename T>
vector<T> stagesNoIntervention(uint32_t testSize, const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	vector<T> directResult;
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
	return directResult;
}

template<typename T>
bool checkResults(vector<T> foundResult, vector<T> correctResult) {
	if(! foundResult.size() == correctResult.size()) {
		return false;
	}

	auto foundResultIt = foundResult.begin();
	for(auto correctResultIt = correctResult.begin(); correctResultIt != correctResult.end(); ++correctResultIt, ++foundResultIt) {
		if(! *correctResultIt == *foundResultIt) {
			return false;
		}
	}	
	return true;
}

template<typename T>
void checkResults(string msg, vector<T> foundResult, vector<T> correctResult) {
	if(! checkResults(foundResult, correctResult)) {
		cerr << msg << " gave an error" << endl;
	}
}

int main(int argc, char** argv) {
	uint32_t testSize = 1024U;
	PerformanceOptions chosenOption = PerformanceOptions_Pipeline;

	if(argc > 1) {
		uint64_t inputSize = strtol(argv[1], 0, 10);
		if(inputSize < numeric_limits<uint32_t>::max()) {
			chosenOption = static_cast<PerformanceOptions>(inputSize);
		} else {
			cerr << "Error: input size does not fit in 32-bit variable" << endl;
			return 1;
		}
	}

	if(argc > 2) {
		uint64_t inputSize = strtol(argv[2], 0, 10);
		if(inputSize < numeric_limits<uint32_t>::max()) {
			testSize = inputSize;
		} else {
			cerr << "Error: input size does not fit in 32-bit variable" << endl;
			return 1;
		}
	}

	duration<double> time = duration<double>::zero();

	srand(0);
	vector<uint32_t> testMatrix;
	testMatrix.reserve(testSize);
	for(auto j = 0U; j < testSize; ++j) {
		testMatrix.push_back(rand());
	}	

	Shift shift(2);
	Increase increase(2);
	Square square;

	// Check whether results are the same. This also helps for warm boot
	vector<uint32_t> manualResult = manual(testSize, testMatrix, shift, increase, square);
	vector<uint32_t> pipelineResult = pipelineTest(testSize, testMatrix, shift, increase, square);
	vector<uint32_t> stagesResult = stages(testSize, testMatrix, shift, increase, square);
	vector<uint32_t> stagesNoInterventionResult = stagesNoIntervention(testSize, testMatrix, shift, increase, square);
	checkResults("pipeline", manualResult, pipelineResult);
	checkResults("stages", manualResult, stagesResult);
	checkResults("stages no intervention", manualResult, stagesNoInterventionResult);

	switch(chosenOption) {
		case PerformanceOptions_Manual: {
			auto nbOfIterations = 0U;
			auto t1 = high_resolution_clock::now();
			time = duration_cast<duration<double>>(high_resolution_clock::now()-t1);
			while(time.count() < minimumExecutionTime) {
				auto result = manual(testSize, testMatrix, shift, increase, square);	
				if(alwaysReturnFalse()) {
					dummy(result);	
				}
				++nbOfIterations;
				time = duration_cast<duration<double>>(high_resolution_clock::now() - t1);
			}
			showResult("Generated manual: average: ", time, nbOfIterations);
		}
			break;
		case PerformanceOptions_Pipeline: {
			auto nbOfIterations = 0U;
			auto t1 = high_resolution_clock::now();
			time = duration_cast<duration<double>>(high_resolution_clock::now()-t1);
			while(time.count() < minimumExecutionTime) {
				auto result = pipelineTest(testSize, testMatrix, shift, increase, square);	
				if(alwaysReturnFalse()) {
					dummy(result);	
				}
				++nbOfIterations;
				time = duration_cast<duration<double>>(high_resolution_clock::now() - t1);
			}
			showResult("Generated pipeline: average: ", time, nbOfIterations);
		}
			break;
		case PerformanceOptions_Stages: {
			auto nbOfIterations = 0U;
			auto t1 = high_resolution_clock::now();
			time = duration_cast<duration<double>>(high_resolution_clock::now()-t1);
			while(time.count() < minimumExecutionTime) {
				auto result = stages(testSize, testMatrix, shift, increase, square);	
				if(alwaysReturnFalse()) {
					dummy(result);	
				}
				++nbOfIterations;
				time = duration_cast<duration<double>>(high_resolution_clock::now() - t1);
			}
			showResult("Sequential: average: ", time, nbOfIterations);
		}
			break;
		case PerformanceOptions_StagesNoInvervention: {
			auto nbOfIterations = 0U;
			auto t1 = high_resolution_clock::now();
			time = duration_cast<duration<double>>(high_resolution_clock::now()-t1);
			while(time.count() < minimumExecutionTime) {
				auto result = stagesNoIntervention(testSize, testMatrix, shift, increase, square);	
				if(alwaysReturnFalse()) {
					dummy(result);	
				}
				++nbOfIterations;
				time = duration_cast<duration<double>>(high_resolution_clock::now() - t1);
			}
			showResult("Sequential no intervention: average: ", time, nbOfIterations);
		}
			break;
		default:
			cerr << "Performance option not supported" << endl;
			break;
	}
}
