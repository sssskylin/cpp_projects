add_library(battleship game.cpp game.h board.cpp board.h strategy.cpp strategy.h)
add_library(strategy strategy.cpp strategy.h)
add_library(board board.cpp board.h vertical_board.cpp hotizontal_board.cpp)
add_library(parser parser.cpp parser.h)