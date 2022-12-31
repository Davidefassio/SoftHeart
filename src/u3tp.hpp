#pragma once

#include "Board.hpp"
#include "Engine.hpp"

#include <string>
#include <unordered_map>

namespace sh
{
    class U3tp
    {
    public:
        U3tp() = default;

        void loop();
    private:
        void stateMachine(const std::string&, const std::unordered_map<std::string, std::string>&, const std::string&);

        Engine engine;
        int m_currState = 0;
        bool m_alive = true;
    };
}