#pragma once

#include <string>

#include "../src/Engine.hpp"

class GameApp
{
public:
	GameApp() = default;

	void run();
	void run(bool, bool, int);

private:
	void game1v1();
	void game1vAI(bool, int);
	void gameAIvAI(int);

	Vec2 moveFromUser(const std::string);
};