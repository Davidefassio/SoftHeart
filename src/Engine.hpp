#pragma once

#include <random>
#include <chrono>

#include "Board.hpp"

class MoveScore
{
public:
	MoveScore()
	{
		m_score = 0.0f;
	}

	MoveScore(Vec2 move, float score)
	{
		m_move = move;
		m_score = score;
	}

	// Override stream extraction operator
	friend std::ostream& operator<<(std::ostream&, const MoveScore&);

	// Override comparison operators
	constexpr bool operator== (const MoveScore& other) const { return m_score == other.m_score; }
	constexpr bool operator!= (const MoveScore& other) const { return m_score != other.m_score; }
	constexpr bool operator<  (const MoveScore& other) const { return m_score <  other.m_score; }
	constexpr bool operator>  (const MoveScore& other) const { return m_score >  other.m_score; }
	constexpr bool operator<= (const MoveScore& other) const { return m_score <= other.m_score; }
	constexpr bool operator>= (const MoveScore& other) const { return m_score >= other.m_score; }

	Vec2 m_move;
	float m_score;
};

class Engine
{
public:
	Engine();

	MoveScore analyzePosition(
		const Board&, 
		std::chrono::duration<double> = std::chrono::seconds(5), 
		int = 500);

	void generateMoves(const Board&, Vec2*, int*);

private:
	std::ranlux48_base m_gen;
	std::uniform_real_distribution<> m_urd;  // Default values are 0.0, 1.0

	int playRandom(Board&, Vec2*);

	// Pick a random int in the range [0, size).
	inline int randInt(int size) { return m_urd(m_gen) * size; }
};