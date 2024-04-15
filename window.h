//
// Created by tenni on 4/8/2024.
//
#ifndef MINESWEEPER_WINDOW_H
#define MINESWEEPER_WINDOW_H
#endif //MINESWEEPER_WINDOW_H
#include <iostream>
using namespace std;
#include <SFML/Graphics.hpp>


class Button{
public:

    sf::Event event;//for functions when there are keyboard events
    string inputText;//welcome screen input
    static void keyboardInputName(sf::Event event, sf::Text &text2);

   // void generateMines(int minesRemaining, std::vector<sf::Vector2i> &minePositions);
};
enum class TileState { Hidden, Revealed, Flagged,Number };
class Tile{
public:
    bool isMine;
    int adjacentMines;
    TileState state;
    Tile(bool isMine = false) : isMine(isMine), adjacentMines(0), state(TileState::Hidden) {}

    bool isMineTile() const {
        return isMine;
    }
    int getAdjacentMines() const {
        return adjacentMines;
    }
    TileState getState() const {
        return state;
    }
    void setAdjacentMines(int count) {
        adjacentMines = count;
    }
    void reveal() {
        state = TileState::Revealed;
    }
    void toggleFlag() {
        state = (state == TileState::Flagged) ? TileState::Hidden : TileState::Flagged;
    }



};