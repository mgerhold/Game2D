#pragma once

#include <chrono>
#include "Time.h"

class Clock {
public:
											Clock();
	Time									restart();
	Time									getElapsedTime() const;
private:
	std::chrono::steady_clock::time_point	mStart;
};