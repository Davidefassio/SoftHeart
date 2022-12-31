#include "u3tp.hpp"

#include <cstddef>
#include <iostream>
#include <vector>
#include <regex>

namespace
{
    const std::regex ws_re("\\s+"); // whitespace

    enum State { BEGIN = 0, HANDSHAKE, IDLE, GAME, ANALYZE, END };        

    std::string enumToString(int state)
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

    void help()
    {
        std::cout << "Print helpful message" << std::endl;
    }
}

// Process input, call action, print output
void sh::U3tp::loop()
{   
    std::string input, data;
    std::unordered_map<std::string, std::string> params;
    std::size_t pos;

    while(m_alive)
    {
        // Reset containers
        params.clear();
        data.clear();

        // Get line from stdin
        std::getline(std::cin, input);

        // Tokenize
        std::vector<std::string> tokens(std::sregex_token_iterator(input.begin(), input.end(), ws_re, -1), std::sregex_token_iterator());
        
        // Fill containers
        for(std::size_t i = 1; i < tokens.size();)
        {
            if(tokens[i][0] == '-' && data.size() == 0)
            {
                params[tokens[i].substr(1)] = tokens[i+1];
                i += 2;
            }
            else
            {
                if(data.size() != 0)
                    data.push_back(' ');
                data.append(tokens[i++]);
            }
        }

        /*
        // DEBUG ///
        // tokens
        for(auto s : tokens)
            std::cout << "'" << s << "'" << std::endl;
        // comm
        std::cout << "comm: '" << tokens[0] << "'" << std::endl;
        // param
        std::cout << "param:" << std::endl;
        for(auto [key, value]: params)
            std::cout << "'" << key << "': '" << value << "'" << std::endl;
        // data
        std::cout << "data: '" << data << "'" << std::endl;
        ////////////
        */

        stateMachine(tokens[0], params, data);
    }

    return;
}

// Check if the command is compatible with the current state.
// If it is compatible then execute it.
void sh::U3tp::stateMachine(const std::string& command, const std::unordered_map<std::string, std::string>& params, const std::string& data)
{
    if(command == "stop")
    {
        m_alive = false;
        return;
    }

    if(command == "state")
    {
        std::cout << enumToString(m_currState) << std::endl;
        return;
    }

    switch(m_currState)
    {
        case BEGIN:
            if(command == "u3tp")
            {
                std::cout << "u3tpok" << std::endl;
                m_currState = IDLE;
                return;
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
                help();
                return;
            }
            if(command == "state_game")
            {
                m_currState = GAME;
                return;
            }
            if(command == "state_analyze")
            {
                m_currState = ANALYZE;
                return;
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
                return;
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
                return;
            }
            break;

        case END:
            break;

        default:
            break;
    }

    return;
}