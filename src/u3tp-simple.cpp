#include "u3tp-simple.hpp"

#include "Engine.hpp"
#include "Timer.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <chrono>

namespace
{
    // Local variables
    Engine engine;
    const std::regex ws_re("\\s+"); // whitespace
    bool beforeHandshake = true;
    std::chrono::duration<double> moveTime = std::chrono::seconds(5);

    double getMultiple(const std::string& str, std::size_t pos)
    {
        if (str.size() == pos + 1)
        {
            if (str[pos] == 's')
                return 1.0;
            if (str[pos] == 'm')
                return 60.0;
            if (str[pos] == 'h')
                return 3600.0;
            if (str[pos] == 'd')
                return 86400.0;
        }

        if (str.size() == pos + 2 && str[pos] == 'm' && str[pos + 1] == 's')
            return 0.001;

        return 0.0;
    }
    
    void setconstraint(const std::string& param, const std::string& pvalue, const std::string& format)
    {
        if (param != "-time")
            return;

        std::size_t pos;
        int num;
        double mult;

        if (pvalue == "move")
        {
            num = std::stoi(format, &pos);
            
            if (!(mult = getMultiple(format, pos)))
                return;

            moveTime = std::chrono::duration<double>(num * mult);
        }
        else if (pvalue == "total")
        {
            std::vector<std::string> tokens;
            int oldPos = 0;

            for (int i = 0; i < format.size(); ++i)
            {
                if (format[i] == '+' || format[i] == '-')
                {
                    tokens.push_back(std::string(format, oldPos, i - oldPos));
                    oldPos = i;
                }
            }
            tokens.push_back(std::string(format, oldPos, format.size() - oldPos));

            auto newMoveTime = std::chrono::duration<double>(0);

            for (int i = 0; i < tokens.size(); ++i)
            {
                bool total = true;
                if (tokens[i][0] == '+' || tokens[i][0] == '-')
                {
                    tokens[i].erase(0, 1);
                    total = false;
                }
                   
                num = std::stoi(tokens[i], &pos);

                if (!(mult = getMultiple(tokens[i], pos)))
                    return;

                mult *= num;
                if (total)
                    mult /= 41;

                newMoveTime += std::chrono::duration<double>(mult);
            }
            
            moveTime = newMoveTime;
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

            // If index if in range [0, 8] apply move
            if (bc >= 0 && bc <= 8 && sc >= 0 && sc <= 8)
                engine.makeMove(Vec2(bc, sc));
        }
    }

    void go()
    {
        std::cout << engine.analyzePosition(moveTime) << std::endl;
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
                if (tokens.size() == 1)
                {
                    std::cout << "u3tp-simpleok" << std::endl;
                    beforeHandshake = false;
                }
            }
        }
        else
        {
            if (tokens[0] == "setconstraint")
            {
                if(tokens.size() == 4)
                    setconstraint(tokens[1], tokens[2], tokens[3]);
            }
            else if (tokens[0] == "position")
            {
                if (tokens.size() == 2)
                    position(tokens[1]);
            }
            else if (tokens[0] == "move")
            {
                if (tokens.size() == 2)
                    makemove(tokens[1]);
            }
            else if (tokens[0] == "go")
            {
                if (tokens.size() == 1)
                    go();
            }
        }
    }

    return;  // Never used
}
