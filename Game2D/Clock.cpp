#include "Clock.h"

Clock::Clock()
	: mStart(std::chrono::steady_clock::now())
{}

Time Clock::restart() {
	auto result = getElapsedTime();
	mStart = std::chrono::steady_clock::now();
	return result;
}

Time Clock::getElapsedTime() const {
	auto now = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - mStart).count();
	return Time::microseconds(duration);
}

void Clock::setElapsedTime(Time time) {
	mStart = std::chrono::steady_clock::now() - std::chrono::microseconds(time.asMicroseconds());
}
