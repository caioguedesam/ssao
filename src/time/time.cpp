#include "stdafx.h"
#include "time/time.h"

std::chrono::time_point<std::chrono::high_resolution_clock> Time::lastTimePoint;
double Time::time;
double Time::deltaTime;
uint64_t Time::frameCount;
double Time::fps;

void Time::Init()
{
	lastTimePoint = std::chrono::high_resolution_clock::now();
	time = 0.;
	deltaTime = 0.;
	frameCount = 0;
	fps = 0.;
}

void Time::UpdateTime()
{
	auto currentTimePoint = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> timePointDelta = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTimePoint - lastTimePoint);

	deltaTime = timePointDelta.count();
	time += deltaTime;
	lastTimePoint = currentTimePoint;

	frameCount++;
	if (frameCount % 100 == 0)
	{
		fps = 1. / deltaTime;	// Update fps every hundred frames
	}
}