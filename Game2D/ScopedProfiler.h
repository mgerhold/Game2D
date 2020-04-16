#pragma once

#include "Clock.h"
#include <string>
#include <unordered_map>

class ScopedProfiler {
public:
	struct FunctionInfo {
		std::string file = "";
		std::string function = "";
		unsigned int line = 0;

		FunctionInfo(const std::string& file, const std::string& function, unsigned int line);

		bool operator==(const FunctionInfo& other) const {
			return (file == other.file && function == other.function && line == other.line);
		}
	};

public:
	ScopedProfiler(const std::string& file, const std::string& function, unsigned int line);
	~ScopedProfiler();

	static void	printMeasurements();

private:
	static void	profile(const FunctionInfo& info, float duration);

private:
	Clock											mClock;
	FunctionInfo									mFunctionInfo;
	static std::unordered_map<FunctionInfo, float>	sMeasurements;
	static Clock									sGlobalClock;
};

namespace std {
	template<>
	struct hash<ScopedProfiler::FunctionInfo> {
		size_t operator()(const ScopedProfiler::FunctionInfo& info) const noexcept {
			return std::hash<std::string>{}(info.file + std::to_string(info.line) + info.function);
		}
	};
}