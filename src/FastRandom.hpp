/*
* SoftHeart, an ultimate-tic-tac-toe engine.
* Copyright (C) 2023-2024 Davide Fassio
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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

		inline std::int32_t generateInt(const std::int32_t max)
		{
			return (*this)() % max;
		}

	private:
		std::uint64_t state;
	};
}