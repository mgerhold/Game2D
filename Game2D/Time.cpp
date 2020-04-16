#include "Time.h"
#include <cmath>

Time Time::seconds(float seconds) {
	return Time(static_cast<long long>(seconds) * 1000000LL);
}

Time Time::milliseconds(long milliseconds) {
	return Time(static_cast<long long>(milliseconds) * 1000LL);
}

Time Time::microseconds(long long microseconds) {
	return Time(microseconds);
}

const Time Time::Zero = Time::microseconds(0);

Time::Time()
	: mMicroseconds(0)
{}

float Time::asSeconds() const {
	return static_cast<float>(mMicroseconds) / 1000000.f;
}

long Time::asMilliseconds() const {
	return static_cast<long>(std::round(mMicroseconds / 1000LL));
}

long long Time::asMicroseconds() const {
	return mMicroseconds;
}

Time::Time(long long microseconds)
	: mMicroseconds(microseconds)
{}

bool operator==(Time lhs, Time rhs) {
	return lhs.asMicroseconds() == rhs.asMicroseconds();
}

bool operator!=(Time lhs, Time rhs) {
	return !(lhs == rhs);
}

bool operator<(Time lhs, Time rhs) {
	return lhs.asMicroseconds() < rhs.asMicroseconds();
}

bool operator>(Time lhs, Time rhs) {
	return lhs.asMicroseconds() > rhs.asMicroseconds();
}

bool operator<=(Time lhs, Time rhs) {
	return (lhs < rhs || lhs == rhs);
}

bool operator>=(Time lhs, Time rhs) {
	return (lhs > rhs || lhs == rhs);
}

Time operator-(Time rhs) {
	return Time::microseconds(-rhs.asMicroseconds());
}

Time operator+(Time lhs, Time rhs) {
	return Time::microseconds(lhs.asMicroseconds() + rhs.asMicroseconds());
}

Time operator-(Time lhs, Time rhs) {
	return Time::microseconds(lhs.asMicroseconds() - rhs.asMicroseconds());
}

Time& operator+=(Time& lhs, Time rhs) {
	lhs = lhs + rhs;
	return lhs;
}

Time& operator-=(Time& lhs, Time rhs) {
	lhs = lhs - rhs;
	return lhs;
}

Time operator*(Time lhs, float rhs) {
	return Time::seconds(lhs.asSeconds() * rhs);
}

Time operator*(Time lhs, long long rhs) {
	return Time::microseconds(lhs.asMicroseconds() * rhs);
}

Time operator*(float lhs, Time rhs) {
	return rhs * lhs;
}

Time operator*(long long lhs, Time rhs) {
	return rhs * lhs;
}

Time& operator*=(Time& lhs, float rhs) {
	lhs = lhs * rhs;
	return lhs;
}

Time& operator*=(Time& lhs, long long rhs) {
	lhs = lhs * rhs;
	return lhs;
}

Time operator/(Time lhs, float rhs) {
	return Time::seconds(lhs.asSeconds() / rhs);
}

Time operator/(Time lhs, long long rhs) {
	return Time::microseconds(lhs.asMicroseconds() / rhs);
}

Time& operator/=(Time& lhs, float rhs) {
	lhs = lhs / rhs;
	return lhs;
}

Time& operator/=(Time& lhs, long long rhs) {
	lhs = lhs / rhs;
	return lhs;
}

float operator/(Time lhs, Time rhs) {
	return lhs.asSeconds() / rhs.asSeconds();
}
