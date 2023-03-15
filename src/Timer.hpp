#pragma once

#include <chrono>

class Timer
{
public:
	// Constructor
	Timer() : m_start(std::chrono::steady_clock::now()), m_lastSplit(m_start)
	{
	}

	// Return the time elpsed from the previous call (or the start if it's the first call)
	inline std::chrono::duration<double> split()
	{
		auto split = std::chrono::steady_clock::now();
		auto dur = split - m_lastSplit;
		m_lastSplit = split;

		return dur;
	}

	// Return the time elpsed from the start
	inline std::chrono::duration<double> total() const
	{
		return std::chrono::steady_clock::now() - m_start;
	}

	// Utility: convert the duration count in seconds
	static inline double durationInSeconds(std::chrono::duration<double> dur)
	{
		return decltype(dur)::period::num * dur.count() / (double) decltype(dur)::period::den;
	}

private:
	std::chrono::time_point<std::chrono::steady_clock> m_start;
	std::chrono::time_point<std::chrono::steady_clock> m_lastSplit;
};