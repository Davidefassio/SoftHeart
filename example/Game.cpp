#include "Game.hpp"

#include <iostream>

// Define a commonly used macro
#define END(str) \
	std::cout << (str) << "\n"; \
	b.printBoard(); \
	return; \

// Default run is 1v1
void GameApp::run()
{
	game1v1();
}

// Run the selected game
void GameApp::run(bool playerCross, bool playerCircle, int secs)
{
	if (playerCross && playerCircle)
		game1v1();
	else if (playerCross || playerCircle)
		game1vAI(playerCross, secs);
	else
		gameAIvAI(secs);
}

void GameApp::game1v1()
{
	Board b;
	std::string move;
	Vec2 cell;
	bool flag;

	while (true)
	{
		b.debugBoard();

		// Take move
		flag = true;
		while (flag)
		{
			flag = false;

			std::cout << "\n" << ((b.m_crossToMove) ? "X > " : "O > ");
			std::cin >> move;

			if (move[0] == 'q')
			{
				END("Aborted!")
			}

			cell = moveFromUser(move);

			// Try to apply move to the board
			if (cell[0] == -1 || !b.makeMove(cell))
			{
				flag = true;
				continue;
			}

			// Check end of the game conditions
			switch (b.checkEndGame())
			{
			case 0:
				continue;

			case 1:
				END("X wins!")

			case 2:
				END("O wins!")

			case 3:
				END("Draw!")
			}

			if (b.checkForcedDraw())
			{
				END("Draw!")
			}
		}
	}
}

void GameApp::game1vAI(bool playerCross, int secs)
{
	Engine e;
	Board b;
	std::string move;
	MoveScore ms;
	Vec2 cell;
	bool flag;

	while (true)
	{
		// Print the board
		b.debugBoard();
		std::cout << "\n";
		
		// Apply a move
		if (b.m_crossToMove ^ playerCross)  // AI
		{
			ms = e.analyzePosition(b, std::chrono::seconds(secs));
			std::cout << "AI > " << ms.m_move[0] + 1 << ms.m_move[1] + 1 << "\n";

			b.makeMove(ms.m_move);
		}
		else  // Player
		{
			// Take move
			flag = true;
			while (flag)
			{
				flag = false;
				std::cout << "Player > ";
				std::cin >> move;

				if (move[0] == 'q')
				{
					END("Aborted!")
				}

				cell = moveFromUser(move);

				if (cell[0] != -1 && b.makeMove(cell))
					break;  // Correctly made a move
			}
		}

		// Check end of the game conditions
		switch (b.checkEndGame())
		{
		case 0:
			continue;

		case 1:
			END("X wins!")

		case 2:
			END("O wins!")

		case 3:
			END("Draw!")
		}

		if (b.checkForcedDraw())
		{
			END("Draw!")
		}
	}
}

void GameApp::gameAIvAI(int secs)
{
	Engine e;
	Board b;
	MoveScore ms;

	while (true)
	{
		b.debugBoard();

		// ms = e.analyzePosition(b, std::chrono::seconds(secs));
		ms = e.analyzePosition(b, std::chrono::milliseconds(secs*10));

		std::cout << "\n" << ((b.m_crossToMove) ? "X > " : "O > ");
		std::cout << ms.m_move[0] + 1 << ms.m_move[1] + 1 << ": " << ms.m_score << "\n";

		b.makeMove(ms.m_move);

		switch (b.checkEndGame())
		{
		case 0:
			continue;

		case 1:
			END("X wins!")

		case 2:
			END("O wins!")

		case 3:
			END("Draw!")
		}

		if (b.checkForcedDraw())
		{
			END("Draw!")
		}
	}
}

Vec2 GameApp::moveFromUser(const std::string move)
{
	// If right size try to convert
	if (move.size() == 2)
	{
		// Convert char to int and sub 1 to convert from human to machine
		int bc = ((int)move[0]) - 48 - 1;
		int sc = ((int)move[1]) - 48 - 1;

		// If index if in range [0, 8] return move
		if (bc >= 0 && bc <= 8 && sc >= 0 && sc <= 8)
			return Vec2(bc, sc);
	}
	// Else return -1
	return Vec2(-1);
}