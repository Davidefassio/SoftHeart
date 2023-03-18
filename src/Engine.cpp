#include "Engine.hpp"

#include "Timer.hpp"

#include <limits>
#include <cstdint>
#include <iostream>
#include <vector>
#include <cmath>

// Construct the engine with 1GiB of memory and generate a random seed
Engine::Engine() : m_mcTree(1073741824)
{
	m_gen.seed(std::random_device{}());
}

// Set the new board and clear the tree
void Engine::setBoard(const Board& newBoard)
{
	m_currPosition = newBoard;
	m_mcTree.clear();
}

bool Engine::makeMove(const Vec2 move)
{
	// If the move is legal make it
	if (!m_currPosition.makeMove(move))
		return false;

	// If the move is a child of the root make it the new root, otherwise clear all
	if (!newRoot(move))
		m_mcTree.clear();

	return true;
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

// Generate and append to the tree all the possible child of the node
// Return:
//   true  = all child were created
//   false = no room for all childs, # child created : [0, maxchilds)
bool Engine::create_childs(const Board& board, Node* currNode)
{
	if (board.m_lastMoveSC != -1)
	{
		for (int i = 0; i < 9; ++i)
			if (board.m_smallBoards[board.m_lastMoveSC][i] == 0)
				if(currNode->m_child = m_mcTree.fillFirstEmpty(Node(Vec2(board.m_lastMoveSC, i), currNode, currNode->m_child)))
					return false;
	}
	else
	{
		for (int i = 0; i < 9; ++i)
			if (board.m_bigBoard[i] == 0)
				for (int j = 0; j < 9; ++j)
					if (board.m_smallBoards[i][j] == 0)
						if (currNode->m_child = m_mcTree.fillFirstEmpty(Node(Vec2(i, j), currNode, currNode->m_child)))
							return false;
	}
	return true;
}

Node* Engine::bestChildByScore(const Node* father)
{
	if (!father->m_child)
		return nullptr;

	std::vector<Node*> bests;
	/*
	* Score:
	*	-2 = no prev child analyzed
	*	-1 = the best has m_total = 0
	*	otherwise score = m_wins / m_total + sqrt(2) * sqrt(ln(father->m_total) / m_total)
	*/
	double bestScore = -2.0, score;

	Node* currNode = father->m_child;
	while (currNode)
	{
		if (currNode->m_total == 0)
		{
			if (bestScore != -1.0)
			{
				bestScore = -1.0;
				bests.clear();
			}
			bests.push_back(currNode);
		}
		else
		{
			if (bestScore != -1.0)
			{
				score = (currNode->m_wins / (double) currNode->m_total) +
					explorationCoeff * std::sqrt(std::log(father->m_total) / currNode->m_total);

				if (score >= bestScore)
				{
					if (score > bestScore)
					{
						bestScore = score;
						bests.clear();
					}
					bests.push_back(currNode);
				}
			}
		}

		currNode = currNode->m_sibling;
	}

	if (bests.size() == 1)
		return bests[0];

	return bests[randInt(bests.size())];
}

Node* Engine::bestChildByPlays()
{
	if (!m_mcTree.m_root->m_child)
		return nullptr;

	Node* best = nullptr;
	Node* currNode = m_mcTree.m_root->m_child;
	std::uint64_t bestScore = -1;

	while (currNode)
	{
		if (currNode->m_total > bestScore)
		{
			best = currNode;
			bestScore = currNode->m_total;
		}
		currNode = currNode->m_sibling;
	}
	
	return best;
}

bool Engine::newRoot(const Vec2 move)
{
	if (!m_mcTree.m_root->m_child)
		return false;

	Node* currNode = m_mcTree.m_root->m_child;

	// Find new root
	while (currNode)
	{
		if (currNode->m_move == move)
		{
			// Found the correct node, set new root
			Node* newRoot = currNode, *tmp;

			// Delete other childs
			currNode = m_mcTree.m_root->m_child;
			while (currNode)
			{
				if (currNode->m_move != move)
				{
					tmp = currNode;
					currNode = currNode->m_sibling;
					m_mcTree.eraseTree(tmp);
				}
			}

			// Delete old root
			m_mcTree.eraseNode(m_mcTree.m_root);

			// Assign new one
			m_mcTree.m_root = newRoot;
			m_mcTree.m_root->m_father = nullptr;
			m_mcTree.m_root->m_sibling = nullptr;

			return true;
		}
	}

	return false;
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
