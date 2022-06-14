#include "stdafx.h"
#include "time/time.h"

#define MAX_FRAMES_TO_TRACK 10

std::chrono::time_point<std::chrono::high_resolution_clock> Time::lastTimePoint;
double Time::time;
double Time::deltaTime;
uint64_t Time::frameCount;
double Time::fps;

double lastFrameTimes[MAX_FRAMES_TO_TRACK];
int lastFrameTimePointer = 0;

double GetFPSFromLastFrameTimes()
{
	double sum = 0.;
	for (int i = 0; i < MAX_FRAMES_TO_TRACK; i++)
	{
		sum += lastFrameTimes[i];
	}
	sum /= MAX_FRAMES_TO_TRACK;
	return 1. / sum;
}

void Time::Init()
{
	lastTimePoint = std::chrono::high_resolution_clock::now();
	time = 0.;
	deltaTime = 0.;
	frameCount = 0;
	fps = 0.;
	for (int i = 0; i < MAX_FRAMES_TO_TRACK; i++)
	{
		lastFrameTimes[i] = 0.;
	}
}

void Time::UpdateTime()
{
	auto currentTimePoint = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> timePointDelta = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTimePoint - lastTimePoint);

	deltaTime = timePointDelta.count();
	time += deltaTime;
	lastTimePoint = currentTimePoint;

	frameCount++;
	lastFrameTimes[lastFrameTimePointer] = deltaTime;
	lastFrameTimePointer++;
	if (lastFrameTimePointer == MAX_FRAMES_TO_TRACK) lastFrameTimePointer = 0;
	fps = GetFPSFromLastFrameTimes();
}