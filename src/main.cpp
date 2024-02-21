/*
* SoftHeart, an ultimate-tic-tac-toe engine.
* Copyright (C) 2023-2024 Davide Fassio
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "u3tp-simple.hpp"

int main(int argc, char* argv[])
{
    sh::U3tp::loop();
    
    return 0;
}

/*
# BUILD OPTIONS #
Create a new folder called Debug or Release, go into it, 
and copy-paste the relevant command.

 - GCC
Debug: TODO
Release: TODO

 - CLANG
Debug:
clang++ -std=c++2a -g -o SoftHeart_debug ../src/main.cpp ../src/u3tp-simple.cpp ../src/Board.cpp ../src/Engine.cpp ../src/Tree.cpp
Release:
clang++ -std=c++2a -O2 -DNDEBUG -flto -march=native -o SoftHeart ../src/main.cpp ../src/u3tp-simple.cpp ../src/Board.cpp ../src/Engine.cpp ../src/Tree.cpp

 - MSVC
Debug: 
cl.exe /DWIN32 /D_WINDOWS /W2 /GR /EHsc /std:c++20 /MDd /Zi /Ob0 /Od /RTC1 ..\src\main.cpp ..\src\u3tp-simple.cpp ..\src\Board.cpp ..\src\Engine.cpp ..\src\Tree.cpp /Fe:SoftHeart_debug.exe
Release:
cl.exe /DWIN32 /D_WINDOWS /W2 /GR /EHsc /std:c++20 /MD /O2 /Ob2 /DNDEBUG /GL ..\src\main.cpp ..\src\u3tp-simple.cpp ..\src\Board.cpp ..\src\Engine.cpp ..\src\Tree.cpp /Fe:SoftHeart.exe
To release with instruction set extensions add: /arch:<IA32|SSE|SSE2|AVX|AVX2|AVX512>
*/ 