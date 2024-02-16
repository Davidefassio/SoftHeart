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

#include "Board.hpp"
#include "Tree.hpp"
#include "FastRandom.hpp"

#include <chrono>

class MoveScore
{
public:
	MoveScore() : m_score(0.0f)
	{
	}

	MoveScore(Vec2 move, float score) : m_move(move), m_score(score)
	{
	}

	// Override stream extraction operator
	friend std::ostream& operator<<(std::ostream&, const MoveScore&);

	// Override comparison operators
	inline bool operator== (const MoveScore& other) const { return m_score == other.m_score; }
	inline bool operator!= (const MoveScore& other) const { return m_score != other.m_score; }
	inline bool operator<  (const MoveScore& other) const { return m_score <  other.m_score; }
	inline bool operator>  (const MoveScore& other) const { return m_score >  other.m_score; }
	inline bool operator<= (const MoveScore& other) const { return m_score <= other.m_score; }
	inline bool operator>= (const MoveScore& other) const { return m_score >= other.m_score; }

	Vec2 m_move;
	float m_score;
};

class Engine
{
public:
	// Construct the engine with 1GiB of memory
	Engine() : m_mcTree(1073741824)
	{
	}

	void setBoard(const Board&);
	bool makeMove(const Vec2);

	const Board& getBoard() const;

	MoveScore analyzePosition(std::chrono::duration<double> = std::chrono::seconds(5));

private:
	Tree m_mcTree;
	Board m_currPosition;
	sh::XorShift<> m_gen;

	// Exploration coefficient:
	//   0 = only exploitation
	//   sqrt2 (~ 1.41) = theoretical value
	//   inf = only exploration
	static constexpr double explorationCoeff = 1.41;

	bool create_childs(const Board&, Node*);

	Node* bestChildByScore(const Node*);
	Node* bestChildByPlays(const Node*) const;
	Node* bestChildByWins(const Node*) const;

	bool newRoot(const Vec2);
	void updateTree(Node*, const int, bool);

	void generateMoves(const Board&, Vec2*, int&) const;
	int playRandom(Board&);
};
