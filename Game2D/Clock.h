#pragma once

#include <chrono>
#include "Time.h"

class Clock {
public:
											Clock();
	Time									restart();
	Time									getElapsedTime() const;
	void									setElapsedTime(Time time);
private:
	std::chrono::steady_clock::time_point	mStart;
};