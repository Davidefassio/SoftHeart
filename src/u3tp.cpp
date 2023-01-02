#include "u3tp.hpp"

#include "Engine.hpp"

#include <cstddef>
#include <iostream>
#include <vector>
#include <string>
#include <regex>

namespace
{
    // Local variables
    Engine engine;
    const std::regex ws_re("\\s+"); // whitespace
    bool beforeHandshake = true;
    bool evaluating = false;

    void help()
    {
        std::cout << "Print helpful message" << std::endl;
    }

    void info()
    {
        std::cout << "Print info message" << std::endl;
    }

    void setoption()
    {
        std::cout << "Print helpful message" << std::endl;
    }

    void setconstraint()
    {
        std::cout << "Print helpful message" << std::endl;
    }

    void clear()
    {
        
    }

    void position()
    {
        std::cout << "Print helpful message" << std::endl;
    }

    void move()
    {
        std::cout << "Print helpful message" << std::endl;
    }

    void go()
    {
        MoveScore best = engine.analyzePosition();
        std::cout << "Best move: " << best << std::endl;
    }

    void ponder()
    {
        std::cout << "Print helpful message" << std::endl;
    }

    void stop()
    {
        std::cout << "Print helpful message" << std::endl;
    }

    void debug()
    {
        std::cout << "Print helpful message" << std::endl;
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
            if(tokens[0] == "u3tp")
            {
                std::cout << "u3tpok" << std::endl;
                beforeHandshake = false;
            }
        }
        else
        {
            // The help message is always printd if requested
            if(tokens[0] == "help") help();

            if(evaluating)
            {
                // Commands that can be used during a search
                if(tokens[0] == "stop") stop();
                else if(tokens[0] == "debug") debug();
            }
            else
            {   
                // Commands that can be used only when the engine is not searching
                if(tokens[0] == "setoption") setoption();
                else if(tokens[0] == "setconstraint") setconstraint();
                else if(tokens[0] == "info") info();
                else if(tokens[0] == "clear") clear();
                else if(tokens[0] == "position") position();
                else if(tokens[0] == "move") move();
                else if(tokens[0] == "go") go();
                else if(tokens[0] == "ponder") ponder();
            }
        }
    }

    return;
}