#pragma once
#include <chrono>

class Time
{
public:
	static std::chrono::high_resolution_clock::time_point lastTimePoint;

	// Times in seconds
	static double time;
	static double deltaTime;

	static void Init();
	static void UpdateTime();
};