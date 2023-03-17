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