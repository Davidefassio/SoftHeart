#pragma once

#include "Board.hpp"
#include "Tree.hpp"

#include <random>
#include <chrono>
#include <numbers>

class MoveScore
{
public:
	MoveScore() : m_score(0.0f)
	{
	}

	MoveScore(Vec2 move, float score): m_move(move), m_score(score)
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
	Engine();

	void setBoard(const Board&);
	bool makeMove(const Vec2);

	const Board& getBoard() const;

	/*MoveScore analyzePosition(
		std::chrono::duration<double> = std::chrono::seconds(5), 
		int = 500);*/
	MoveScore analyzePosition(std::chrono::duration<double> = std::chrono::seconds(5));

private:
	Tree m_mcTree;
	Board m_currPosition;
	std::ranlux48_base m_gen;
	std::uniform_real_distribution<> m_urd;  // Default values are 0.0, 1.0

	// Exploration coefficient:
	// 0 = only exploitation
	// sqrt2 = theoretical value
	// inf = only exploration
	static constexpr double explorationCoeff = std::numbers::sqrt2;

	bool create_childs(const Board&, Node*);
	Node* bestChildByScore(const Node*);
	Node* bestChildByPlays(const Node*);
	bool newRoot(const Vec2);
	void updateTree(Node*, int, bool);
	void generateMoves(const Board&, Vec2*, int&);
	int playRandom(Board&);

	// Pick a random int in the range [0, size).
	inline int randInt(int size) 
	{ 
		return m_urd(m_gen) * size; 
	}
};
