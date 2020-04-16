#pragma once

class Time {
public:
	Time();
	float		asSeconds() const;
	long		asMilliseconds() const;
	long long	asMicroseconds() const;
public:
	static Time seconds(float seconds);
	static Time	milliseconds(long milliseconds);
	static Time	microseconds(long long microseconds);
	static const Time Zero;
private:
	Time(long long microseconds);
private:
	long long	mMicroseconds;
};

bool operator==(Time lhs, Time rhs);
bool operator!=(Time lhs, Time rhs);
bool operator<(Time lhs, Time rhs);
bool operator>(Time lhs, Time rhs);
bool operator<=(Time lhs, Time rhs);
bool operator>=(Time lhs, Time rhs);
Time operator-(Time rhs);
Time operator+(Time lhs, Time rhs);
Time operator-(Time lhs, Time rhs);
Time& operator+=(Time& lhs, Time rhs);
Time& operator-=(Time& lhs, Time rhs);
Time operator*(Time lhs, float rhs);
Time operator*(Time lhs, long long rhs);
Time operator*(float lhs, Time rhs);
Time operator*(long long lhs, Time rhs);
Time& operator*=(Time& lhs, float rhs);
Time& operator*=(Time& lhs, long long rhs);
Time operator/(Time lhs, float rhs);
Time operator/(Time lhs, long long rhs);
Time& operator/=(Time& lhs, float rhs);
Time& operator/=(Time& lhs, long long rhs);
float operator/(Time lhs, Time rhs);