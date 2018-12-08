#pragma once
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <chrono>

typedef uint64_t MicroSeconds;

// вернет промежуток времени в микросекундах
// с момента последнего запуска этого же метода
MicroSeconds QuantMS()
{
	typedef std::chrono::microseconds
		MSec;
	typedef std::chrono::high_resolution_clock
		Clock;

	static Clock::time_point last;

	const Clock::time_point now
		= Clock::now();

	const auto result
		= std::chrono::duration_cast<MSec>(now - last);

	last = now;

	return static_cast<MicroSeconds>(result.count());
}
