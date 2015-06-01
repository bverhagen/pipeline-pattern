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
#include <iomanip>

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
using std::setfill;
using std::setw;

using namespace std::chrono;

const double minimumExecutionTime = 5.0;

enum PerformanceOptions {
	PerformanceOptions_Manual = 0,
	PerformanceOptions_Pipeline = 1,
	PerformanceOptions_Stages = 2,
	PerformanceOptions_StagesNoInvervention = 3,
};

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

		inline uint32_t operator() (uint32_t x) const {
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

		inline uint32_t operator() (uint32_t x) const {
			return x*x;
		}
};

inline void showResult(std::string msg, duration<double> time, uint32_t nbOfIterations) {
	cout << setfill(' ') << setw(28) << msg << setfill(' ') << setw(13) << time.count() / double(nbOfIterations) << " seconds (" << setfill(' ') << setw(15) << nbOfIterations << " iterations in " << time.count() << " seconds)" << endl;
}

template<typename T>
vector<T> manual(const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	vector<T> unrolledResult;
	unrolledResult.reserve(testMatrix.size());
	for(auto testMatrixIt = testMatrix.begin(); testMatrixIt != testMatrix.end(); ++testMatrixIt) {
		unrolledResult.push_back(square(increase(shift(*testMatrixIt))));
	}
	assert(unrolledResult.size() == testMatrix.size());
	return unrolledResult;
}

template<typename T>
vector<T> pipelineTest(const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	vector<T> pipelineResult;
	pipelineResult.reserve(testMatrix.size());
	for(auto testMatrixIt = testMatrix.begin(); testMatrixIt != testMatrix.end(); ++testMatrixIt) {
		pipelineResult.push_back(pipeline(*testMatrixIt, shift, increase, square));
	}
	assert(pipelineResult.size() == testMatrix.size());
	return pipelineResult;
}

template<typename T>
vector<T> stages(const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	vector<T> directResult;
	directResult.reserve(testMatrix.size());
	for(auto testMatrixIt = testMatrix.begin(); testMatrixIt != testMatrix.end(); ++testMatrixIt) {
		directResult.push_back(shift(*testMatrixIt));
	}
	if(alwaysReturnFalse()) {
		dummy(directResult);
	}
	for(auto it = directResult.begin(); it != directResult.end(); ++it) {
		*it = increase(*it);
	}
	if(alwaysReturnFalse()) {
		dummy(directResult);
	}
	for(auto it = directResult.begin(); it != directResult.end(); ++it) {
		*it = square(*it);
	}
	assert(directResult.size() == testMatrix.size());
	return directResult;
}

template<typename T>
vector<T> stagesNoIntervention(const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	vector<T> directResult;
	directResult.reserve(testMatrix.size());

	for(auto testMatrixIt = testMatrix.begin(); testMatrixIt != testMatrix.end(); ++testMatrixIt) {
		directResult.push_back(shift(*testMatrixIt));
	}
	for(auto it = directResult.begin(); it != directResult.end(); ++it) {
		*it = increase(*it);
	}
	for(auto it = directResult.begin(); it != directResult.end(); ++it) {
		*it = square(*it);
	}
	assert(directResult.size() == testMatrix.size());
	return directResult;
}

template<typename T>
bool checkResults(const vector<T>& foundResult, const vector<T>& correctResult) {
	if(foundResult.size() == 0) {
		cerr << "Size of the found result is 0" << endl;
		return false;
	}

	if(foundResult.size() != correctResult.size()) {
		cerr << "Size of found result is " << foundResult.size() << " while size of correct result is " << correctResult.size() << endl;
		return false;
	}

	auto foundResultIt = foundResult.begin();
	for(auto correctResultIt = correctResult.begin(); correctResultIt != correctResult.end(); ++correctResultIt, ++foundResultIt) {
		if(! *correctResultIt == *foundResultIt) {
			cerr << "Erroneous value found" << endl;
			return false;
		}
	}	
	return true;
}

template<typename T>
void checkResults(string msg, const vector<T>& foundResult, const vector<T>& correctResult) {
	if(! checkResults(foundResult, correctResult)) {
		cerr << msg << " gave an error" << endl;
		assert(true);
	}
}

template<typename T>
void executeManual(const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	auto nbOfIterations = 0U;
	auto t1 = high_resolution_clock::now();
	auto time = duration_cast<duration<double>>(high_resolution_clock::now()-t1);
	while(time.count() < minimumExecutionTime) {
		auto result = manual(testMatrix, shift, increase, square);
		if(alwaysReturnFalse()) {
			dummy(result);	
		}
		time = duration_cast<duration<double>>(high_resolution_clock::now() - t1);
		++nbOfIterations;
	}
	showResult("Generated manual: ", time, nbOfIterations);
}

template<typename T>
void executePipeline(const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	auto nbOfIterations = 0U;
	auto t1 = high_resolution_clock::now();
	auto time = duration_cast<duration<double>>(high_resolution_clock::now()-t1);
	while(time.count() < minimumExecutionTime) {
		auto result = pipelineTest(testMatrix, shift, increase, square);	
		if(alwaysReturnFalse()) {
			dummy(result);	
		}
		time = duration_cast<duration<double>>(high_resolution_clock::now() - t1);
		++nbOfIterations;
	}
	showResult("Generated pipeline: ", time, nbOfIterations);
}

template<typename T>
void executeStages(const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	auto nbOfIterations = 0U;
	auto t1 = high_resolution_clock::now();
	auto time = duration_cast<duration<double>>(high_resolution_clock::now()-t1);
	while(time.count() < minimumExecutionTime) {
		auto result = stages(testMatrix, shift, increase, square);	
		if(alwaysReturnFalse()) {
			dummy(result);	
		}
		time = duration_cast<duration<double>>(high_resolution_clock::now() - t1);
		++nbOfIterations;
	}
	showResult("Sequential: ", time, nbOfIterations);
}

template<typename T>
void executeStagesNoIntervention(const std::vector<T>& testMatrix, const Shift& shift, const Increase& increase, const Square& square) {
	auto nbOfIterations = 0U;
	auto t1 = high_resolution_clock::now();
	auto time = duration_cast<duration<double>>(high_resolution_clock::now()-t1);
	while(time.count() < minimumExecutionTime) {
		auto result = stagesNoIntervention(testMatrix, shift, increase, square);	
		if(alwaysReturnFalse()) {
			dummy(result);	
		}
		time = duration_cast<duration<double>>(high_resolution_clock::now() - t1);
		++nbOfIterations;
	}
	showResult("Sequential no intervention: ", time, nbOfIterations);
}

int main(int argc, char** argv) {
	uint32_t testSize = 1024U;

	if(argc > 1) {
		uint64_t inputSize = strtol(argv[1], 0, 10);
		if(inputSize < numeric_limits<uint32_t>::max()) {
			testSize = inputSize;
		} else {
			cerr << "Error: input size does not fit in 32-bit variable" << endl;
			return 1;
		}
	}

	uint32_t argIndex = 2U;
	uint32_t uargc = argc;
	vector<PerformanceOptions> options;
	while(uargc > argIndex) {
		uint64_t inputSize = strtol(argv[argIndex], 0, 10);
		if(inputSize < numeric_limits<uint8_t>::max()) {
			options.push_back(static_cast<PerformanceOptions>(inputSize));
		} else {
			cerr << "Error: input size does not fit in 32-bit variable" << endl;
			return 1;
		}
		++argIndex;
	}

	// Add default measurement option
	if(options.size() == 0) {
		options.push_back(PerformanceOptions_Pipeline);
	}

	srand(0);
	vector<uint32_t> testMatrix;
	testMatrix.reserve(testSize);
	for(auto j = 0U; j < testSize; ++j) {
		testMatrix.push_back(rand());
	}	
	assert(testMatrix.size() == testSize);

	Shift shift(2);
	Increase increase(2);
	Square square;

	// Check whether results are the same. This also helps for warm(er) calculations
	vector<uint32_t> manualResult = manual(testMatrix, shift, increase, square);
	vector<uint32_t> pipelineResult = pipelineTest(testMatrix, shift, increase, square);
	vector<uint32_t> stagesResult = stages(testMatrix, shift, increase, square);
	vector<uint32_t> stagesNoInterventionResult = stagesNoIntervention(testMatrix, shift, increase, square);
	checkResults("pipeline", manualResult, pipelineResult);
	checkResults("stages", manualResult, stagesResult);
	checkResults("stages no intervention", manualResult, stagesNoInterventionResult);

	for(auto option : options) {
		switch(option) {
			case PerformanceOptions_Manual:
				executeManual(testMatrix, shift, increase, square);
				break;
			case PerformanceOptions_Pipeline:
				executePipeline(testMatrix, shift, increase, square);
				break;
			case PerformanceOptions_Stages:
				executeStages(testMatrix, shift, increase, square);
				break;
			case PerformanceOptions_StagesNoInvervention:
				executeStagesNoIntervention(testMatrix, shift, increase, square);
				break;
			default:
				cerr << "Performance option not supported" << endl;
				break;
		}
	}
}
