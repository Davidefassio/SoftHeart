#include "Engine.hpp"
#include "Timer.hpp"

#include <limits>
#include <cstdint>
#include <iostream>

// Construct the engine and generate a random seed
Engine::Engine()
{
	m_gen.seed(std::random_device{}());
}

void Engine::setBoard(const Board& newBoard)
{
	m_currPosition = newBoard;
}

bool Engine::makeMove(const Vec2 move)
{
	return m_currPosition.makeMove(move);
}

const Board& Engine::getBoard() const
{
	return m_currPosition;
}

// Analyze a position and return the best move.
// It tries to return in totTime.
// If totTime is <1s it is suggested to reduce sampleRuns to 200 or less.
MoveScore Engine::analyzePosition(std::chrono::duration<double> totTime, int sampleRuns)
{
	std::int64_t count = 0;
	float score;
	int mask = (m_currPosition.m_crossToMove) ? 1 : -1;

	Vec2 moves[81], buffer[81];
	int moves_size;
	generateMoves(m_currPosition, moves, &moves_size);

	MoveScore bestMS(Vec2(), std::numeric_limits<float>::lowest());

	// moves[0] + testTime
	Board moved(m_currPosition);
	moved.makeMoveUnsafe(moves[0]);

	Timer timer;

	for (int i = 0; i < sampleRuns; ++i)
	{
		Board copy(moved);
		count += playRandom(copy, buffer);
	}

	auto diff = timer.total();

	std::uint64_t playsPerMove = (sampleRuns * totTime) / (diff * moves_size);

	for (std::uint64_t i = sampleRuns; i < playsPerMove; ++i)
	{
		Board copy(moved);
		count += playRandom(copy, buffer);
	}

	score = (((float) count) / playsPerMove) * mask;

	if (score > bestMS.m_score)
	{
		bestMS.m_score = score;
		bestMS.m_move = moves[0];
	}

	// All other moves
	for (int m = 1; m < moves_size; ++m)
	{
		count = 0;

		Board moved(m_currPosition);
		moved.makeMoveUnsafe(moves[m]);

		for (std::uint64_t i = 0; i < playsPerMove; ++i)
		{
			Board copy(moved);
			count += playRandom(copy, buffer);
		}

		score = (((float)count) / playsPerMove) * mask;

		if (score > bestMS.m_score)
		{
			bestMS.m_score = score;
			bestMS.m_move = moves[m];
		}
	}

	bestMS.m_score *= mask;
	return bestMS;
}

// Generate all possible moves from a position.
// The moves are stored in Vec2* moves.
// The number of moves generated is stored in int* cnt.
void Engine::generateMoves(const Board& board, Vec2* moves, int* cnt)
{
	*cnt = 0;

	if (board.m_lastMoveSC != -1)
	{
		for (int i = 0; i < 9; ++i)
			if (board.m_smallBoards[board.m_lastMoveSC][i] == 0)
				moves[(*cnt)++] = Vec2(board.m_lastMoveSC, i);
	}
	else
	{
		for (int i = 0; i < 9; ++i)
			if (board.m_bigBoard[i] == 0)
				for (int j = 0; j < 9; ++j)
					if (board.m_smallBoards[i][j] == 0)
						moves[(*cnt)++] = Vec2(i, j);
	}
}

// Play a position randomly till the end and return the outcome.
// For efficiency sake the buffer for the moves is already created.
int Engine::playRandom(Board& b, Vec2* moves)
{
	int r, moves_size;
	while (true)
	{
		generateMoves(b, moves, &moves_size);

		b.makeMoveUnsafe(moves[randInt(moves_size)]);

		r = b.checkEndGame();

		if (r == 0)
		{
			if (b.checkForcedDraw())
				return 0;
		}
		else if (r == 1)
			return 1;
		else if (r == 2)
			return -1;
		else
			return 0;
	}
}

// MoveScore
// Override stream extraction operator
std::ostream& operator<<(std::ostream& os, const MoveScore& ms)
{
	os << ms.m_move.data[0] + 1 << ms.m_move.data[1] + 1 << " " << ms.m_score;
	return os;
}
