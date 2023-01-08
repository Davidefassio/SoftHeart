#include "Board.hpp"

#include <iostream>

#include "Utils.hpp"


Board::Board()
{
	reset();
}

bool Board::set(const std::string& state)
{
	// Check string lenght (81 cells + last move)
	if (state.size() != 82)
		return false;

	// Set the smallBoards
	Vec2 cell;
	int countX = 0, countO = 0;
	for (int i = 0; i < 81; ++i)
	{
		cell = Utils::arrayToNestedGrid(i);

		if (state[i] == '.')
		{
			m_smallBoards[cell[0]][cell[1]] = 0;
		}
		else if (state[i] == 'X')
		{
			m_smallBoards[cell[0]][cell[1]] = 1;
			++countX;
		}
		else if (state[i] == 'O')
		{
			m_smallBoards[cell[0]][cell[1]] = 2;
			++countO;
		}
		else
			return false;
	}

	// Set the turn (cross starts first)
	if (countX == countO)
		m_crossToMove = true;
	else if (countX == countO + 1)
		m_crossToMove = false;
	else
		return false;

	// Set last move
	if (state[81] == '-')
	{
		m_lastMoveSC = -1;
	}
	else
	{
		int sc = ((int)state[81]) - 48 - 1;

		if (sc < 0 && sc > 8)
			return false;

		m_lastMoveSC = sc;
	}

	// Compute the bigBoard from the smallBoards position
	for (int i = 0; i < 9; ++i)
		m_bigBoard = m_smallBoards[i].tris();

	return true;
}

void Board::reset()
{
	m_bigBoard.fill(0);
	for (int i = 0; i < m_smallBoards.size(); ++i)
		m_smallBoards[i].fill(0);
	m_crossToMove = true;
	m_lastMoveSC = -1;
}

std::string Board::toString() const
{
	std::string retval;
	Vec2 cell;

	for (int i = 0; i < 81; ++i)
	{
		cell = Utils::arrayToNestedGrid(i);
		retval += Utils::cellIntToChar(m_smallBoards[cell[0]][cell[1]]);
	}

	retval += " ";
	retval += (m_crossToMove) ? 'X' : 'O';
	retval += " ";
	retval += (m_lastMoveSC > 0) ? m_lastMoveSC + 48 + 1 : '-';

	return retval;
}

bool Board::makeMove(const Vec2 move)
{
	// If the big cell is already done return false
	if (m_bigBoard[move[0]] != 0)
		return false;

	// If the big cell doesn't match the last small cell return false 
	if (m_lastMoveSC != -1 && move[0] != m_lastMoveSC)
		return false;

	// If the small cell if already full return false
	if (m_smallBoards[move[0]][move[1]] != 0)
		return false;

	m_smallBoards[move[0]][move[1]] = (m_crossToMove) ? 1 : 2;

	m_bigBoard[move[0]] = m_smallBoards[move[0]].tris();

	if (m_bigBoard[move[1]] == 0)
		m_lastMoveSC = move[1];
	else
		m_lastMoveSC = -1;

	m_crossToMove = !m_crossToMove;

	return true;
}

void Board::makeMoveUnsafe(const Vec2 move)
{
	m_smallBoards[move[0]][move[1]] = 2 - ((int) m_crossToMove);
	m_bigBoard[move[0]] = m_smallBoards[move[0]].tris();
	m_lastMoveSC = (m_bigBoard[move[1]]) ? -1 : move[1];
	m_crossToMove = !m_crossToMove;
}

int Board::checkEndGame() const
{
	return m_bigBoard.tris();
}

bool Board::checkForcedDraw() const
{
	return m_bigBoard.forcedDraw();
}

void Board::printBoard() const
{
	Vec2 cells;

	std::cout << "Big board";
	for (int i = 0; i < 9; ++i) {
		if (i % 3 == 0)
			std::cout << "\n";

		std::cout << Utils::cellIntToChar(m_bigBoard[i]) << " ";
	}

	std::cout << "\nWhole board";
	for (int i = 0; i < 81; ++i)
	{
		if (i % 3 == 0 && i % 9 != 0)
			std::cout << "  ";

		if (i % 9 == 0)
			std::cout << "\n";

		if (i % 27 == 0 && i != 0)
			std::cout << "\n";

		cells = Utils::arrayToNestedGrid(i);

		std::cout << Utils::cellIntToChar(m_smallBoards[cells[0]][cells[1]]) << " ";
	}
	std::cout << "\n";
}

void Board::debugBoard() const
{
	printBoard();

	std::cout << "Cross to move: " << ((m_crossToMove) ? "True" : "False") << "\n";
	std::cout << "Last small cell move: " << m_lastMoveSC + 1 << "\n";
}
