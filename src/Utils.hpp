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

#include "Vec.hpp"

namespace Utils
{
	// Convert the internal representation to the human one
	constexpr char _cellIntToChar[5] = { '.', 'X', 'O', '#', '?' };

	inline char cellIntToChar(const int n)
	{
		return (n >= 0 && n < 4) ? _cellIntToChar[n] : _cellIntToChar[4];
	}

	// Conversion utilities between boards
	constexpr int _mailboxGridToArray[81] = {
		 0,  1,  2,	 9, 10, 11,	18, 19, 20,
		 3,  4,  5,	12, 13, 14,	21, 22, 23,
		 6,  7,  8,	15, 16, 17,	24, 25, 26,
		27, 28, 29,	36, 37, 38,	45, 46, 47,
		30, 31, 32,	39, 40, 41,	48, 49, 50,
		33, 34, 35,	42, 43, 44,	51, 52, 53,
		54, 55, 56,	63, 64, 65,	71, 73, 74,
		57, 58, 59,	66, 67, 68,	75, 76, 77,
		60, 61, 62,	69, 70, 71,	78, 79, 80
	};

	constexpr int _mailboxArrayToArray[162] = {
		0, 0, 0, 1, 0, 2, 1, 0, 1, 1, 1, 2,	2, 0, 2, 1, 2, 2,
		0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5,	2, 3, 2, 4, 2, 5,
		0, 6, 0, 7, 0, 8, 1, 6, 1, 7, 1, 8,	2, 6, 2, 7, 2, 8,
		3, 0, 3, 1, 3, 2, 4, 0, 4, 1, 4, 2,	5, 0, 5, 1, 5, 2,
		3, 3, 3, 4, 3, 5, 4, 3, 4, 4, 4, 5,	5, 3, 5, 4, 5, 5,
		3, 6, 3, 7, 3, 8, 4, 6, 4, 7, 4, 8,	5, 6, 5, 7, 5, 8,
		6, 0, 6, 1, 6, 2, 7, 0, 7, 1, 7, 2,	8, 0, 8, 1, 8, 2,
		6, 3, 6, 4, 6, 5, 7, 3, 7, 4, 7, 5,	8, 3, 8, 4, 8, 5,
		6, 6, 6, 7, 6, 8, 7, 6, 7, 7, 7, 8,	8, 6, 8, 7, 8, 8
	};

	inline int nestedGridToArray(Vec2 coords)
	{
		return _mailboxGridToArray[9 * coords[0] + coords[1]];
	}

	inline Vec2 arrayToNestedGrid(const int i)
	{
		return Vec2(_mailboxArrayToArray[2 * i], _mailboxArrayToArray[2 * i + 1]);
	}
}