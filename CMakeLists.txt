#cmake_minimum_required(VERSION 4.1)
#project(MazeProject)
#
#set(CMAKE_CXX_STANDARD 20)
#
#add_executable(MazeProject main.cpp
#        Maze.cpp
#        Maze.h)

cmake_minimum_required(VERSION 3.28)
project(MazeProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(MazeProject main.cpp
        Maze.cpp
        Maze.h
        MazeScreen.cpp
        MazeScreen.h
        Player.cpp
        Player.h
        HumanPlayer.cpp
        HumanPlayer.h
        ComputerBFTPlayer.cpp
        ComputerBFTPlayer.h
        Button.cpp
        Button.h
        ComputerDFSPlayer.cpp
        ComputerDFSPlayer.h
        ComputerRandomPlayer.cpp
        ComputerRandomPlayer.h)
target_compile_features(MazeProject PRIVATE cxx_std_17)
target_link_libraries(MazeProject PRIVATE SFML::Graphics)
