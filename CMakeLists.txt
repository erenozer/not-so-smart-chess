cmake_minimum_required(VERSION 3.26)
project(Chess)

set(CMAKE_CXX_STANDARD 11)

add_executable(Chess
        main.cpp
        Piece.cpp
        Board.cpp
)
