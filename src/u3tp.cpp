#include "u3tp.hpp"

#include <cstddef>
#include <iostream>
#include <vector>
#include <regex>

namespace
{

    enum State { BEGIN, HANDSHAKE, IDLE, GAME, ANALYZE, END };        

    std::string enumToString(State state)
    {
        switch(state)
        {
            case BEGIN:
                return "begin";
            case HANDSHAKE:
                return "handshake";
            case IDLE:
                return "idle";
            case GAME:
                return "game";
            case ANALYZE:
                return "analyze";
            case END:
                return "end";
            default:
                return "";
        }
    }
}

// Process input, call action, print output
void sh::U3tp::loop()
{   
    std::string input, data;
    std::unordered_map<std::string, std::string> params;
    std::size_t pos;

    const std::regex ws_re("\\s+"); // whitespace

    while(m_alive)
    {
        // Reset containers
        params.clear();
        data.clear();

        // Get line from stdin
        std::getline(std::cin, input);

        // Tokenize
        std::vector<std::string> tokens(std::sregex_token_iterator(input.begin(), input.end(), ws_re, -1), std::sregex_token_iterator());
        
        if(tokens.size() < 1)
        {
            continue;
        }
        else
        {  
            std::size_t i;
            for(i = 1; i < tokens.size(); i += 2)
            {
                if(tokens[i][0] == '-')
                    params[tokens[i]] = tokens[i+1];
                else
                    data = tokens[i];
            }
            stateMachine(tokens[0], params, data);
        }
    }

    return;
}

// Check if the command is compatible with the current state.
// If it is compatible then execute it.
bool sh::U3tp::stateMachine(const std::string& command, const std::unordered_map<std::string, std::string>& params, const std::string& data)
{
    if(command == "stop")
    {
        m_alive = false;
        return true;
    }

    if(command == "state")
    {
        std::cout << enumToString((State) m_currState) << std::endl;
        return true;
    }

    switch(m_currState)
    {
        case BEGIN:
            if(command == "u3tp")
            {
                std::cout << "u3tpok" << std::endl;
                m_currState = IDLE;
                return true;
            }
            break;

        case HANDSHAKE:
            break;

        case IDLE:
            if(command == "setoption")
            {
                
            }
            if(command == "help")
            {
                
            }
            if(command == "state_game")
            {
                m_currState = GAME;
                return true;
            }
            if(command == "state_analyze")
            {
                m_currState = ANALYZE;
                return true;
            }
            break;

        case GAME:
            if(command == "start")
            {
                
            }
            if(command == "move")
            {
                
            }
            if(command == "quit_state")
            {
                m_currState = IDLE;
                return true;
            }
            break;

        case ANALYZE:
            if(command == "position")
            {
                
            }
            if(command == "move")
            {
                
            }
            if(command == "quit_state")
            {
                m_currState = IDLE;
                return true;
            }
            break;

        case END:
            break;

        default:
            break;
    }

    return false;
}