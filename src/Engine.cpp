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

MoveScore Engine::analyzePosition(std::chrono::duration<double> totTime)
{
	// Start timer
	Timer t;

	Board movingBoard(m_currPosition);
	Node* currNode;
	int res;

	// Cycle every round
	while (t.total() < totTime)
	{
		movingBoard = m_currPosition;
		currNode = m_mcTree.m_root;

		// Cycle every layer
		while (true)
		{
			// If the game is ended handle it
			if (res = movingBoard.checkEndGame())
			{
				updateTree(currNode, res, movingBoard.m_crossToMove);
				break;
			}

			// If the only outcome possible is a draw handle it
			if (movingBoard.checkForcedDraw())
			{
				updateTree(currNode, 3, movingBoard.m_crossToMove);
				break;
			}

			if (!currNode->m_child)
			{
				if (currNode->m_total == 0 || !create_childs(movingBoard, currNode))
				{
					// Play random and update all the nodes back the tree
					updateTree(currNode, playRandom(movingBoard), movingBoard.m_crossToMove);
					break;
				}
			}

			currNode = bestChildByScore(currNode);
			movingBoard.makeMoveUnsafe(currNode->m_move);
		}
	}

	Node* bestChild = bestChildByPlays(m_mcTree.m_root);
	if (!bestChild)
		return MoveScore(Vec2(-1, -1), 0);
	return MoveScore(bestChild->m_move, bestChild->m_wins / (float) bestChild->m_total);
}

// Generate and append to the tree all the possible child of the node
// Return:
//   true  = all child were created
//   false = no room for all childs, # child created : [0, maxchilds)
bool Engine::create_childs(const Board& board, Node* father)
{
	Node* prev = nullptr;
	Node* curr = nullptr;

	if (board.m_lastMoveSC != -1)
	{
		for (int i = 0; i < 9; ++i)
		{
			if (board.m_smallBoards[board.m_lastMoveSC][i] == 0)
			{
				if (!(curr = m_mcTree.fillFirstEmpty(Node(Vec2(board.m_lastMoveSC, i), father))))
					return false;

				if (!father->m_child)
					father->m_child = curr;
				else
					prev->m_sibling = curr;
				prev = curr;
			}
		}
	}
	else
	{
		for (int i = 0; i < 9; ++i)
		{
			if (board.m_bigBoard[i] == 0)
			{
				for (int j = 0; j < 9; ++j)
				{
					if (board.m_smallBoards[i][j] == 0)
					{
						if (!(curr = m_mcTree.fillFirstEmpty(Node(Vec2(i, j), father))))
							return false;

						if (!father->m_child)
							father->m_child = curr;
						else
							prev->m_sibling = curr;
						prev = curr;
					}
				}
			}
		}
	}
	return true;
}

Node* Engine::bestChildByScore(const Node* father)
{
	Node* currNode = father->m_child;
	if (!currNode)
		return nullptr;

	Node* buffer[81];

	// State:
	// 0 total = 0
	// 1 score > 0 : normal state
	int state = 1, size;
	double bestScore = -1.0, score;

	while (currNode)
	{
		if (currNode->m_total == 0)
		{
			if (state)
			{
				state = size = 0;
			}

			buffer[size++] = currNode;
		}
		else
		{
			if (state)
			{
				score = (currNode->m_wins / (double)currNode->m_total) +
					explorationCoeff * std::sqrt(std::log(father->m_total >> 1) / (currNode->m_total >> 1));

				if (score >= bestScore)
				{
					if (score > bestScore)
					{
						bestScore = score;
						size = 0;
					}
					buffer[size++] = currNode;
				}
			}
		}

		currNode = currNode->m_sibling;
	}

	if (size == 1)
		return buffer[0];

	return buffer[randInt(size)];
}

Node* Engine::bestChildByPlays(const Node* father)
{
	if (!father->m_child)
		return nullptr;

	Node* best = father->m_child;
	Node* currNode = best->m_sibling;
	std::int64_t bestScore = best->m_total;
	
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
	Node* currNode = m_mcTree.m_root->m_child;

	// Find new root
	while (currNode)
	{
		if (currNode->m_move == move)
		{
			// Found the correct node, set new root
			Node* newRoot = currNode;

			// Delete other childs
			currNode = m_mcTree.m_root->m_child;
			while (currNode)
			{
				if (currNode->m_move != move)
					m_mcTree.eraseTree(currNode);

				currNode = currNode->m_sibling;
			}

			// Delete old root
			m_mcTree.eraseNode(m_mcTree.m_root);

			// Assign new one
			m_mcTree.m_root = newRoot;
			m_mcTree.m_root->m_father = nullptr;
			m_mcTree.m_root->m_sibling = nullptr;

			return true;
		}

		currNode = currNode->m_sibling;
	}
	return false;
}

void Engine::updateTree(Node* currNode, const int res, bool crossToMove)
{
	while (currNode)
	{
		currNode->m_total += 2;

		if (res == 3)
			++(currNode->m_wins);
		else if ((res == 1 && !crossToMove) || (res == 2 && crossToMove))
			currNode->m_wins += 2;

		crossToMove = !crossToMove;
		currNode = currNode->m_father;
	}
}

// Generate all possible moves from a position.
// The moves are stored in this->moveBuffer.
// The number of moves generated is stored in int* cnt.
void Engine::generateMoves(const Board& board, Vec2* buffer, int& cnt)
{
	cnt = 0;

	if (board.m_lastMoveSC != -1)
	{
		for (int i = 0; i < 9; ++i)
			if (board.m_smallBoards[board.m_lastMoveSC][i] == 0)
				buffer[cnt++] = Vec2(board.m_lastMoveSC, i);
	}
	else
	{
		for (int i = 0; i < 9; ++i)
			if (board.m_bigBoard[i] == 0)
				for (int j = 0; j < 9; ++j)
					if (board.m_smallBoards[i][j] == 0)
						buffer[cnt++] = Vec2(i, j);
	}
}

// Play a position randomly till the end and return the outcome.
int Engine::playRandom(Board& b)
{
	Vec2 buffer[81];
	int r, moves_size;
	while (true)
	{
		generateMoves(b, buffer, moves_size);

		b.makeMoveUnsafe(buffer[randInt(moves_size)]);

		if (r = b.checkEndGame())
			return r;
		else if (b.checkForcedDraw())
				return 3;
	}
}

// MoveScore
// Override stream extraction operator
std::ostream& operator<<(std::ostream& os, const MoveScore& ms)
{
	os << ms.m_move.data[0] + 1 << ms.m_move.data[1] + 1 << " " << ms.m_score;
	return os;
}
