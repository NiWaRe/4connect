#include "board.h"
#include <iostream>

Board::Board(quint8 width, quint8 height)
    : _width(width), _height(height), _stones_count(0),
      _Matrix(height, std::vector<int>(width, 0))
{
}


std::pair<qint16, qint16> Board::makeMove(quint8 x, Player player){
    if (x < _width){
        if (checkValidity(x)==true){
            quint8 y = getLowestRow(x);
            setStone(x, y, player);
            ++_stones_count;
            std::cout << "Setting Stone at ("
                      << static_cast<int>(x) << ", "
                      << static_cast<int>(y) << ")" << std::endl;
            return std::make_pair(static_cast<qint16>(x), static_cast<qint16>(y));
        }
        else{
            return std::make_pair(-2, -2);  // Column is full
        }
    }
    else{
        return std::make_pair(-1, -1);  // Column outside of the Board
    }
}


int Board::getStone(quint8 x, quint8 y) {
    return _Matrix.at(y).at(x) / 1000;
}


void Board::setStone(quint8 x, quint8 y, Player player) {
    switch (player){
        case Player::Player1:
            _Matrix.at(y).at(x) = 1000 + _stones_count;
            break;
        case Player::Player2:
            _Matrix.at(y).at(x) = 2000 + _stones_count;
            break;
    }
}

void Board::removeStone(quint8 x, quint8 y) {
    _Matrix.at(y).at(x) = 0;
}


quint8 Board::getLowestRow(quint8 x) {
    // should be called after Board::checkValidity()
    // + 1 because otherwise y changes to 255 after 0 and we get an infinite loop
    for (quint8 y = (_height - 1 + 1); y >= 0 + 1; y--) {
        if (_Matrix.at(y-1).at(x) == 0) {
            return y-1;
        }
    }
//    throw "Error in Board::processMove()";
}


bool Board::checkValidity(quint8 x){
    if (x < _width){

        if (_Matrix.at(0).at(x) == 0){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}


bool Board::checkAllColumnsFull(){
    for (quint8 x = 0; x < _width; x++){
        if (_Matrix.at(0).at(x) == 0) {
            return false;
        }
    }
    return true;
}


bool Board::checkWinCondition(Player player){
    int value = (player == Player::Player1) ? 1 : 2;

    // check horizontal
    for (quint8 y = 0; y < _height; y++){
        for (quint8 x = 0; x < _width-3; x++){
            if (getStone(x,y)==value){
                if (getStone(x+1,y) == value){
                    if (getStone(x+2,y) == value){
                        if (getStone(x+3,y) == value){
                            return true;
                        }
                    }
                }
            }
        }
    }


    // check vertical
    for (quint8 x = 0; x < _width; x++){
        for (quint8 y = 0; y < _height-3; y++){
            if (getStone(x,y) == value){
                if (getStone(x,y+1) == value){
                    if (getStone(x,y+2) == value){
                        if (getStone(x,y+3) == value){
                            return true;
                        }
                    }
                }
            }
        }
    }

    // check diagonal
    for (quint8 x = 0; x < _width-3; x++){
        for (quint8 y = 0; y < _height-3; y++){
            if (getStone(x,y) == value){
                if (getStone(x+1,y+1) == value){
                    if (getStone(x+2,y+2) == value){
                        if (getStone(x+3,y+3) == value){
                            return true;
                        }
                    }
                }
            }
        }
    }

    // check diagonal
    for (quint8 x = 3; x < _width; x++){
        for (quint8 y = 0; y < _height-3; y++){
            if (getStone(x,y) == value){
                if (getStone(x-1,y+1) == value){
                    if (getStone(x-2,y+2) == value){
                        if (getStone(x-3,y+3) == value){
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}


std::set<std::pair<quint8, quint8>> Board::check3Sets(Player player){
    std::set<std::pair<quint8, quint8>> pairs_set;
//    std::vector<Set3> sets_vector;

    int value = (player == Player::Player1) ? 1 : 2;

    // check horizontal
    for (quint8 y = 0; y < _height; y++){
        for (quint8 x = 0; x < _width-3; x++){
            // xxx0
            if (getStone(x,y)==value){
                if (getStone(x+1,y) == value){
                    if (getStone(x+2,y) == value){
                        if (getStone(x+3,y) == 0){
                            pairs_set.insert(std::make_pair(x,y));
                            pairs_set.insert(std::make_pair(x+1,y));
                            pairs_set.insert(std::make_pair(x+2,y));
//                            Set3 set;
//                            set.x1 = x;
//                            set.y1 = y;
//                            set.x2 = x+1;
//                            set.y2 = y;
//                            set.x3 = x+2;
//                            set.y3 = y;
//                            sets_vector.push_back(set);
                        }
                    }
                }
            }
            // 0xxx
            else if (getStone(x,y)==0){
                if (getStone(x+1,y) == value){
                    if (getStone(x+2,y) == value){
                        if (getStone(x+3,y) == value){
                            pairs_set.insert(std::make_pair(x+1,y));
                            pairs_set.insert(std::make_pair(x+2,y));
                            pairs_set.insert(std::make_pair(x+3,y));
//                            Set3 set;
//                            set.x1 = x+1;
//                            set.y1 = y;
//                            set.x2 = x+2;
//                            set.y2 = y;
//                            set.x3 = x+3;
//                            set.y3 = y;
//                            sets_vector.push_back(set);
                        }
                    }
                }
            }
        }
    }


    // check vertical
    for (quint8 x = 0; x < _width; x++){
        for (quint8 y = 0; y < _height-3; y++){
            //  --x--
            //  --x--
            //  --x--
            //  --0--
            if (getStone(x,y) == value){
                if (getStone(x,y+1) == value){
                    if (getStone(x,y+2) == value){
                        if (getStone(x,y+3) == 0){
                            pairs_set.insert(std::make_pair(x,y));
                            pairs_set.insert(std::make_pair(x,y+1));
                            pairs_set.insert(std::make_pair(x,y+2));
//                            Set3 set;
//                            set.x1 = x;
//                            set.y1 = y;
//                            set.x2 = x;
//                            set.y2 = y+1;
//                            set.x3 = x;
//                            set.y3 = y+2;
//                            sets_vector.push_back(set);
                        }
                    }
                }
            }
            //  --0--
            //  --x--
            //  --x--
            //  --x--
            else if (getStone(x,y) == 0){
                if (getStone(x,y+1) == value){
                    if (getStone(x,y+2) == value){
                        if (getStone(x,y+3) == value){
                            pairs_set.insert(std::make_pair(x,y+1));
                            pairs_set.insert(std::make_pair(x,y+2));
                            pairs_set.insert(std::make_pair(x,y+3));
//                            Set3 set;
//                            set.x1 = x;
//                            set.y1 = y+1;
//                            set.x2 = x;
//                            set.y2 = y+2;
//                            set.x3 = x;
//                            set.y3 = y+3;
//                            sets_vector.push_back(set);
                        }
                    }
                }
            }
        }
    }

    // check diagonal
    for (quint8 x = 0; x < _width-3; x++){
        for (quint8 y = 0; y < _height-3; y++){
            //  -x---
            //  --x--
            //  ---x-
            //  ----0
            if (getStone(x,y) == value){
                if (getStone(x+1,y+1) == value){
                    if (getStone(x+2,y+2) == value){
                        if (getStone(x+3,y+3) == 0){
                            pairs_set.insert(std::make_pair(x,y));
                            pairs_set.insert(std::make_pair(x+1,y+1));
                            pairs_set.insert(std::make_pair(x+2,y+2));
//                            Set3 set;
//                            set.x1 = x;
//                            set.y1 = y;
//                            set.x2 = x+1;
//                            set.y2 = y+1;
//                            set.x3 = x+2;
//                            set.y3 = y+2;
//                            sets_vector.push_back(set);
                        }
                    }
                }
            }
            //  -0---
            //  --x--
            //  ---x-
            //  ----x
            else if (getStone(x,y) == 0){
                if (getStone(x+1,y+1) == value){
                    if (getStone(x+2,y+2) == value){
                        if (getStone(x+3,y+3) == value){
                            pairs_set.insert(std::make_pair(x+1,y+1));
                            pairs_set.insert(std::make_pair(x+2,y+2));
                            pairs_set.insert(std::make_pair(x+3,y+3));
//                            Set3 set;
//                            set.x1 = x+1;
//                            set.y1 = y+1;
//                            set.x2 = x+2;
//                            set.y2 = y+2;
//                            set.x3 = x+3;
//                            set.y3 = y+3;
//                            sets_vector.push_back(set);
                        }
                    }
                }
            }
        }
    }

    // check diagonal
    for (quint8 x = 3; x < _width; x++){
        for (quint8 y = 0; y < _height-3; y++){
            //  ----x
            //  ---x-
            //  --x--
            //  -0---
            if (getStone(x,y) == value){
                if (getStone(x-1,y+1) == value){
                    if (getStone(x-2,y+2) == value){
                        if (getStone(x-3,y+3) == 0){
                            pairs_set.insert(std::make_pair(x,y));
                            pairs_set.insert(std::make_pair(x-1,y+1));
                            pairs_set.insert(std::make_pair(x-2,y+2));
//                            Set3 set;
//                            set.x1 = x;
//                            set.y1 = y;
//                            set.x2 = x-1;
//                            set.y2 = y+1;
//                            set.x3 = x-2;
//                            set.y3 = y+2;
//                            sets_vector.push_back(set);
                        }
                    }
                }
            }
            //  ----0
            //  ---x-
            //  --x--
            //  -x---
            else if (getStone(x,y) == 0){
                if (getStone(x-1,y+1) == value){
                    if (getStone(x-2,y+2) == value){
                        if (getStone(x-3,y+3) == value){
                            pairs_set.insert(std::make_pair(x-1,y+1));
                            pairs_set.insert(std::make_pair(x-2,y+2));
                            pairs_set.insert(std::make_pair(x-3,y+3));
//                            Set3 set;
//                            set.x2 = x-1;
//                            set.y2 = y+1;
//                            set.x3 = x-2;
//                            set.y3 = y+2;
//                            set.x1 = x-3;
//                            set.y1 = y+3;
//                            sets_vector.push_back(set);
                        }
                    }
                }
            }
        }
    }
    return pairs_set;
//    return sets_vector;
}


int Board::getHistory(quint8 x, quint8 y) {
    return _Matrix.at(y).at(x) % 1000;
}

//void addSetsToVector(std::vector<Set3> &sets_vector, Set3 set)
