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

#include <array>
#include <string>

/*
* Board cell state:
*	0 = empty
*	1 = X
*	2 = O
*	3 = draw
 */
class Board
{
public:
	// Constructors
	Board();
	Board(const Board&) = default;

	bool set(const std::string&);
	void reset();               // Reset to empty board
	std::string toString() const;

	bool makeMove(const Vec2);  // Apply a move to the board
	void makeMoveUnsafe(const Vec2);
	int checkEndGame() const;      // Check if a game is ended
	bool checkForcedDraw() const;  // Check if the only outcome possible is a draw

	// Printing functions
	void printBoard() const;
	void debugBoard() const;

	// Members
	Vec9 m_bigBoard;
	std::array<Vec9, 9> m_smallBoards;
	int m_lastMoveSC;
	bool m_crossToMove;
};