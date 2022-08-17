#include "stdafx.h"
#include "time/time.h"
#include <glad/glad.h>
#include "debugging/gl.h"

#define FRAMES_TO_TRACK 240
#define QUERY_INVALID UINT32_MAX

namespace Time
{
	uint64_t clock_frequency;
	uint64_t current_frame = 0;
	double elapsed_time = 0.;
	double delta_time = 0.;

	Timer timer_app;
	Timer timer_frame;

	void Timer::start()
	{
		BOOL ret;
		LARGE_INTEGER res;
		ret = QueryPerformanceCounter(&res);
		ASSERT(ret, "[ERROR:TIMER] QueryPerformanceCounter failed.");
		start_ticks = res.QuadPart;
	}

	void Timer::end()
	{
		ASSERT(start_ticks, "[ERROR:TIMER] Trying to end a timer that hasn't started yet.");
		BOOL ret;
		LARGE_INTEGER res;
		ret = QueryPerformanceCounter(&res);
		ASSERT(ret, "[ERROR:TIMER] QueryPerformanceCounter failed.");
		end_ticks = res.QuadPart;
	}

	uint64_t Timer::elapsed_ticks()
	{
		ASSERT(start_ticks, "[ERROR:TIMER] Trying to get time for timer that hasn't started yet.");
		ASSERT(end_ticks, "[ERROR:TIMER] Trying to get time for timer that hasn't ended yet.");

		return end_ticks - start_ticks;
	}

	double Timer::elapsed_ns()
	{
		uint64_t ticks = elapsed_ticks();
		ticks *= (uint64_t)1e9;
		return (double)ticks / (double)clock_frequency;
	}

	double Timer::elapsed_ms()
	{
		return elapsed_ns() / 1e6;
	}

	double Timer::elapsed_secs()
	{
		return elapsed_ns() / 1e9;
	}

	void init()
	{
#if !_WIN32
		ASSERT(0, "NOT SUPPORTED ON CURRENT PLATFORM.");
#endif
		BOOL ret;
		LARGE_INTEGER res;
		ret = QueryPerformanceFrequency(&res);
		ASSERT(ret, "Failed to initialize time system: QueryPerformanceFrequency failed.");
		clock_frequency = res.QuadPart;

		timer_app.start();
		timer_app.end();
		timer_frame.start();
		timer_frame.end();

		FrameTracking::init();
	}

	void update()
	{
		if (current_frame != 0)
		{
			timer_frame.end();
			delta_time = timer_frame.elapsed_secs();
		}
		elapsed_time = timer_app.elapsed_secs();

		timer_app.end();
		timer_frame.start();
		current_frame++;
	}

	uint64_t currentFrame()
	{
		return current_frame;
	}

	double elapsedTime()
	{
		return elapsed_time;
	}

	double deltaTime()
	{
		return delta_time;
	}

	double fps()
	{
		return 1. / delta_time;		// TODO_TIME: Stop using this for FPS metrics
	}

	namespace FrameTracking
	{
		double cpu_times[FRAMES_TO_TRACK];
		double gpu_times[FRAMES_TO_TRACK];
		Timer cpu_timer;
		uint32_t gpu_queries[2] = {QUERY_INVALID, QUERY_INVALID};
		int query_back_buffer = 0;
		uint64_t gpu_frametime;

		void init()
		{
			memset(cpu_times, 0, sizeof(cpu_times));
			memset(gpu_times, 0, sizeof(gpu_times));
		}

		void cpu_frametracking_start()
		{
			cpu_timer.start();
		}

		void cpu_frametracking_end()
		{
			if (currentFrame() == 0) return;
			cpu_timer.end();
			uint64_t i = currentFrame();
			cpu_times[i % FRAMES_TO_TRACK] = cpu_timer.elapsed_ms();
		}

		void gpu_frametracking_start()
		{
			if (gpu_queries[0] == QUERY_INVALID)
			{
				GL(glGenQueries(1, &gpu_queries[0]));
				GL(glGenQueries(1, &gpu_queries[1]));
			}

			GL(glBeginQuery(GL_TIME_ELAPSED, gpu_queries[query_back_buffer]));
		}

		void gpu_frametracking_end()
		{
			ASSERT(gpu_queries[0] != QUERY_INVALID, "Trying to track GPU frame time without initializing queries.");

			GL(glEndQuery(GL_TIME_ELAPSED));
			int query_front_buffer = (query_back_buffer + 1) % 2;
			GL(glGetQueryObjectui64v(gpu_queries[query_front_buffer], GL_QUERY_RESULT, &gpu_frametime));

			uint64_t i = currentFrame();
			if (i != 0)
			{
				gpu_times[(i - 1) % FRAMES_TO_TRACK] = (double)gpu_frametime / 1e6;		// Nano to milliseconds
			}

			query_back_buffer = query_front_buffer;
		}

		double* get_cpu_times()
		{
			return cpu_times;
		}

		double* get_gpu_times()
		{
			return gpu_times;
		}
	}
}