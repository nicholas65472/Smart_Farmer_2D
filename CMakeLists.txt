cmake_minimum_required(VERSION 3.16)
project(SmartFarmer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "E:/Facultate/An 1/SFML-2.6.2/lib/cmake/SFML")

add_executable(SmartFarmer
        src/main.cpp
        src/Game.cpp
        src/GameMenu.cpp
        src/GamePlaying.cpp
        src/GameFences.cpp
        src/GameResult.cpp
        src/Board.cpp
        src/Resources.cpp
        src/FenceLogic.cpp
        src/UI.cpp
)

target_include_directories(SmartFarmer PRIVATE include)

find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

target_link_libraries(SmartFarmer
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)