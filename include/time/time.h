#pragma once
#include "stdafx.h"

namespace Ty
{
	namespace Time
	{
		struct Timer
		{
			uint64_t start_ticks = 0;
			uint64_t end_ticks = 0;

			void start();
			void end();

			uint64_t elapsed_ticks();
			double elapsed_ns();
			double elapsed_ms();
			double elapsed_secs();
		};

		void init();
		void update();

		uint64_t currentFrame();
		double elapsedTime();
		double deltaTime();
		double fps();

		namespace FrameTracking
		{
			void init();
			void cpu_frametracking_start();
			void cpu_frametracking_end();
			void gpu_frametracking_start();
			void gpu_frametracking_end();

			double* get_cpu_times();
			double* get_gpu_times();
		}
	}
}