#pragma once

#include <chrono>

class Timer
{
public:
	Timer();
	std::chrono::duration<double> split(bool = false);
	std::chrono::duration<double> total(bool = false);

private:
	std::chrono::time_point<std::chrono::steady_clock> m_start;
	std::chrono::time_point<std::chrono::steady_clock> m_lastSplit;
};