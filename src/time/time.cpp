#include "stdafx.h"
#include "time/time.h"

std::chrono::time_point<std::chrono::high_resolution_clock> Time::lastTimePoint;
double Time::time;
double Time::deltaTime;

void Time::Init()
{
	lastTimePoint = std::chrono::high_resolution_clock::now();
	time = 0.;
	deltaTime = 0.;
}

void Time::UpdateTime()
{
	auto currentTimePoint = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> timePointDelta = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTimePoint - lastTimePoint);

	deltaTime = timePointDelta.count();
	time += deltaTime;
	lastTimePoint = currentTimePoint;
}