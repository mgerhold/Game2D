#include "ScopedProfiler.h"
#include <iostream>
#include <vector>
#include <algorithm>

std::unordered_map<ScopedProfiler::FunctionInfo, float>	ScopedProfiler::sMeasurements = std::unordered_map<ScopedProfiler::FunctionInfo, float>();
Clock ScopedProfiler::sGlobalClock = Clock();

ScopedProfiler::ScopedProfiler(const std::string& file, const std::string& function, unsigned int line)
	: mClock()
	, mFunctionInfo(file, function, line)
{}

ScopedProfiler::~ScopedProfiler() {
	profile(mFunctionInfo, mClock.getElapsedTime().asSeconds());
}

void ScopedProfiler::printMeasurements() {
#ifdef _DEBUG
	std::cout << "---------------\nProfiler Measurements:\n---------------\n";
	std::vector<std::pair<FunctionInfo, float>> measurementsVector(sMeasurements.begin(), sMeasurements.end());
	std::sort(measurementsVector.begin(), measurementsVector.end(), [](const auto& lhs, const auto& rhs) -> bool {
		return lhs.second > rhs.second;
	});
	float sum = 0.f;
	float totalExecutionTime = sGlobalClock.getElapsedTime().asSeconds();
	for (const auto& measurement : measurementsVector) {
		sum += measurement.second;
	}
	for (const auto& measurement : measurementsVector) {
		std::cout << measurement.first.file << ", " << measurement.first.function 
			<< " (" << measurement.first.line << "): " << measurement.second 
			<< " s = " << (measurement.second * 100.f / sum) <<  "% ("
			<< (measurement.second * 100.f / totalExecutionTime) << "%)\n";
	}
	std::cout << "---------------\n\tTotal profiled time: " << sum << " s\n";
	std::cout << "\tTotal execution time: " << totalExecutionTime << " s\n\nPress enter to continue...";
	std::cin.get();
#endif
}

void ScopedProfiler::profile(const FunctionInfo& info, float duration) {	
	//std::cout << "Profiling function " << info.function << "\n";
	auto find = sMeasurements.find(info);
	if (find == sMeasurements.end()) {
		// new entry
		sMeasurements[info] = duration;
	} else {
		// existing entry
		sMeasurements[info] += duration;
	}
}

ScopedProfiler::FunctionInfo::FunctionInfo(const std::string& file, const std::string& function, unsigned int line)
	: file(file)
	, function(function)
	, line(line)
{}
