#include <chrono>
#include "timeutils.h"

// In order to be able to calculate when the getTimeDeltaSeconds() function was last called, we need to know the point in time when that happened. This requires us to keep hold of that point in time.
// We initialise this value to the time at the start of the program.
static std::chrono::steady_clock::time_point _previousTimePoint = std::chrono::steady_clock::now();

// Calculates the elapsed time since the previous time this function was called.
double getTimeDeltaSeconds() {
	// Determine the current time
	std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();

	// Calculate the number of nanoseconds that elapsed since the previous call to this function
	long long timeDelta = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - _previousTimePoint).count();
	// Convert the time delta in nanoseconds to seconds
	double timeDeltaSeconds = (double)timeDelta / 1000000000.0;

	// Store the previously measured current time
	_previousTimePoint = currentTime;

	// Return the calculated time delta in seconds
	return timeDeltaSeconds;
}