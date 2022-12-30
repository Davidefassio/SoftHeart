#pragma once

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
        bool stateMachine(const std::string&, const std::unordered_map<std::string, std::string>&, const std::string&);

        int m_currState;
        bool m_alive = true;
    };
}