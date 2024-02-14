#pragma once

#include <cstdint>
#include <random>

namespace sh
{
	template<std::uint8_t _Ax = 13, std::uint8_t _Bx = 7, std::uint8_t _Cx = 17>
	class XorShift
	{
		static_assert(_Ax < 64 && _Bx < 64 && _Cx < 64, "template arguments for XorShift must be less than 64");

	public:
		XorShift() : state((((std::uint64_t) std::random_device{}()) << 32) | (std::random_device{}()))
		{
		}

		XorShift(std::uint64_t seed) : state(seed)
		{
		}

		inline std::uint64_t operator()()
		{
			state ^= state << _Ax;
			state ^= state >> _Bx;
			state ^= state << _Cx;
			return state;
		}

		// This function is slightly skewed towards smaller numbers,
		// but for max <<< uint64_t::max() is negligible.
		inline std::int32_t generateInt(const std::int32_t max)
		{
			return (*this)() % max;
		}

	private:
		std::uint64_t state;
	};
}