#include "Timer.hpp"

#include <iostream>

namespace
{
	double durationInSeconds(std::chrono::duration<double> dur)
	{
		return decltype(dur)::period::num * dur.count() / (double) decltype(dur)::period::den;
	}
}

Timer::Timer()
{
	m_start = std::chrono::steady_clock::now();
	m_lastSplit = m_start;
}

std::chrono::duration<double> Timer::split(bool print)
{
	auto split = std::chrono::steady_clock::now();
	auto dur = split - m_lastSplit;
	m_lastSplit = split;

	if (print)
		std::cout << durationInSeconds(dur) << "s" << std::endl;

	return dur;
}

std::chrono::duration<double> Timer::total(bool print)
{
	auto split = std::chrono::steady_clock::now();
	auto dur = split - m_start;

	if (print) 
		std::cout << durationInSeconds(dur) << "s" << std::endl;

	return dur;
}