#include "u3tp-simple.hpp"

#include "Engine.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <chrono>
#include <cctype>

#define ISDIGIT(x) ((x) >= '0' && (x) <= '9')

namespace
{
    // Local variables
    Engine engine;
    const std::regex ws_re("\\s+"); // whitespace
    bool beforeHandshake = true;
    std::chrono::duration<double> moveTime = std::chrono::seconds(5);

    int getMultiple(const std::string& str, std::size_t pos)
    {
        if (str.size() == pos + 1)
        {
            if (str[pos] == 's')
                return 1000;
            if (str[pos] == 'm')
                return 60000;
            if (str[pos] == 'h')
                return 3600000;
            if (str[pos] == 'd')
                return 86400000;
        }

        if (str.size() == pos + 2 && str[pos] == 'm' && str[pos + 1] == 's')
            return 1;

        return 0;
    }
    
    void setconstraint(const std::string& param, const std::string& pvalue, const std::string& format)
    {
        if (param != "-time")
            return;

        if (pvalue == "move")
        {
            std::size_t pos;
            const int i = std::stoi(format, &pos);
            const int m = getMultiple(format, pos);

            if (m == 0)
                return;

            moveTime = std::chrono::milliseconds(i * m);
        }
        else if (pvalue == "total")
        {
            std::size_t pos;
            std::vector<std::string> tokens;
            int oldPos = 0, num, mult;

            for (int i = 0; i < format.size(); ++i)
            {
                if (format[i] == '+' || format[i] == '-')
                {
                    tokens.push_back(std::string(format, oldPos, i - oldPos));
                    oldPos = i;
                }
            }
            tokens.push_back(std::string(format, oldPos, format.size() - oldPos));

            moveTime = std::chrono::milliseconds(0);

            for (int i = 0; i < tokens.size(); ++i)
            {
                bool incdel = false;
                if (tokens[i][0] == '+' || tokens[i][0] == '-')
                {
                    tokens[i].erase(0, 1);
                    incdel = true;
                }
                   
                num = std::stoi(tokens[i], &pos);
                mult = getMultiple(tokens[i], pos);

                if (mult == 0)
                    return;

                if (incdel)
                    moveTime += std::chrono::milliseconds(num * mult);
                else
                    moveTime += std::chrono::milliseconds((int) num * mult / 41);
            }

            std::cout << moveTime << std::endl;
        }
    }

    void position(const std::string& position)
    {
        Board board;
        if (board.set(position))
            engine.setBoard(board);
    }

    void makemove(const std::string& move)
    {
        // If right size try to convert
        if (move.size() == 2)
        {
            // Convert char to int and sub 1 to convert from human to machine
            int bc = ((int)move[0]) - 48 - 1;
            int sc = ((int)move[1]) - 48 - 1;

            // If index if in range [0, 8] return move
            if (bc >= 0 && bc <= 8 && sc >= 0 && sc <= 8)
                engine.makeMove(Vec2(bc, sc));
        }
    }

    void go()
    {
        MoveScore best = engine.analyzePosition(moveTime);
        std::cout << "Best move: " << best << std::endl;
    }
}

// Process input, call action, print output
void sh::U3tp::loop()
{
    std::string input;

    while(true)
    {
        // Get line from stdin
        std::getline(std::cin, input);

        // Tokenize
        std::vector<std::string> tokens(std::sregex_token_iterator(input.begin(), input.end(), ws_re, -1), std::sregex_token_iterator());
        
        if(tokens[0] == "quit")
            return;  // Always quit

        // Before the handshake every command is ignored
        if(beforeHandshake)
        {
            if(tokens[0] == "u3tp-simple")
            {
                std::cout << "u3tp-simpleok" << std::endl;
                beforeHandshake = false;
            }
        }
        else
        {
            if(tokens[0] == "setconstraint") setconstraint(tokens[1], tokens[2], tokens[3]);
            else if(tokens[0] == "position") position(tokens[1]);
            else if(tokens[0] == "move") makemove(tokens[1]);
            else if(tokens[0] == "go") go();
        }
    }

    return;
}
