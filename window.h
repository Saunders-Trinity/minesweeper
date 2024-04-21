//
// Created by tenni on 4/8/2024.
//
#ifndef MINESWEEPER_WINDOW_H
#define MINESWEEPER_WINDOW_H
#endif //MINESWEEPER_WINDOW_H
#include <iostream>
using namespace std;
#include <cstdlib>
#include <ctime>
#include <random>

#include <SFML/Graphics.hpp>


class Button{
public:

    sf::Event event;//for functions when there are keyboard events
    string inputText;//welcome screen input
    static void keyboardInputName(sf::Event event, sf::Text &text2);


};



class Tile{
public://note: tile sprite is all sprites
    bool isMine = false;
    bool isFlag = false;
    bool isRevealed = false;
    int adjacentMines;
    int count;
    sf::Sprite tileSprite;
    sf::Sprite tile2sprite;
    sf::Texture texturehiddentile;
    sf::Texture revealedTile;
    sf::Texture flag2;
    sf::Texture mine2;


    Tile(int x, int y, bool isMine = false){
        texturehiddentile.loadFromFile("files/images/tile_hidden.png");
        tileSprite.setTexture(texturehiddentile);
        tileSprite.setPosition(x,y);

        flag2.loadFromFile("files/images/flag.png");
        tile2sprite.setTexture(flag2);
        tile2sprite.setPosition(x,y);

        mine2.loadFromFile("files/images/mine.png");
    }


   void drawTile( sf::RenderWindow &gameWindow) {
       gameWindow.draw(tileSprite);
       if (isFlag) {
           gameWindow.draw(tile2sprite);
       }
       if (isRevealed and isMine) {
           tile2sprite.setTexture(mine2);
           gameWindow.draw(tile2sprite);
       }
   }

    void reveal() {
        revealedTile.loadFromFile("files/images/tile_revealed.png");
        tileSprite.setTexture(revealedTile);
        isRevealed = true;
    }
    void generateMines(sf::RenderWindow& gameWindow,  vector<vector<Tile>>& tileVector,int mineCount) {
        random_device rd;
        mt19937 gen(rd());
        int minesPlaced = 0;
        int totalTiles = tileVector.size() * tileVector[0].size();

        while (minesPlaced < mineCount) {
            int randomIndex = rand() % totalTiles; // Generate random index for the tileVector
            int row = randomIndex / tileVector[0].size();
            int col = randomIndex % tileVector[0].size();

            Tile& tile = tileVector[row][col];
            if (!tile.isMineTile()) {
                tile.isMine = true;
                minesPlaced++;
            }
        }
    }




    bool isMineTile() const {

        return isMine;
    }
    int getAdjacentMines(sf::RenderWindow& gameWindow, const vector<vector<Tile>>& tileVector, int row, int col) {
        //loops through  adjacent mines
        //counts them
        //returns number
        //if that number is 1-8
        //then...
        int count = 0;
        int numRows = tileVector.size();
        int numCols = tileVector[0].size();
        for (int i = row - 1; i <= row + 1; ++i) {
            for (int j = col - 1; j <= col + 1; ++j) {
                // Check if neighboring tile is within bounds and not the current tile
                if (i >= 0 && i < numRows && j >= 0 && j < numCols && !(i == row && j == col)) {
                    // Check if neighboring tile contains a mine
                    if (tileVector[i][j].isMineTile()) {
                        count++;
                    }
                }
            }
         }
        adjacentMines = count;
       // cout << count << endl;
        return count;
     }










    void revealallMines(sf::RenderWindow& gameWindow,  vector<vector<Tile>>& tileVector) {
        //user clicks on tile
        //all other tiles reveal
        int numRows = tileVector.size();
        int numCols = tileVector[0].size();
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                // Check if the tile is a mine
                if (tileVector[i][j].isMineTile()) {
                    // Reveal the mine tile
                    tile2sprite.setTexture(mine2);
                    tileVector[i][j].isRevealed = true;
                }
            }
        }
    }
    void revealallTiles(sf::RenderWindow& gameWindow,  vector<vector<Tile>>& tileVector){
        int numRows = tileVector.size();
        int numCols = tileVector[0].size();
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                // Check if the tile is a mine
                if (!tileVector[i][j].isMineTile() and !isRevealed) {
                    // flip all of the hidden tiles to reveal tiles?
                    tileSprite.setTexture(revealedTile);
                    tileVector[i][j].isRevealed = true;
                }
            }
        }
    }



    void revealNumbers(sf::RenderWindow& gameWindow, int count, vector<vector<Tile>>& tileVector, int x, int y){
        sf::Texture* numberTexture = new sf::Texture();
        Tile& tile = tileVector[x][y];
        if(count >= 1 && count <= 8 && !tile.isMineTile()){//a number and not a mine tile
            numberTexture->loadFromFile("files/images/number_" + std::to_string(count) + ".png");

        }
        sf::Sprite* numberSprite = new sf::Sprite();
        numberSprite->setTexture(*numberTexture);
        numberSprite->setPosition(x * 32, y * 32);

        // Draw sprite
        gameWindow.draw(*numberSprite);

        // Clean up memory
        delete numberSprite;
        delete numberTexture;
    }

    void openLeaderboard(sf::RenderWindow& gameWindow , sf::RenderWindow& leaderboardWindow){
        gameWindow.clear(sf::Color(255, 255, 255, 128));
        leaderboardWindow.clear(sf::Color::Blue);
        leaderboardWindow.display();

    }
};


class Player{//for leaderboard window
public:
    string name;
    string time;

    static vector<Player> readLeaderboardFromFile(const std::string& filename){
        ifstream file("leaderboard.txt");
        vector<Player> leaderboard;
        if (file.is_open()){
            string line;
            while (getline(file, line)){
                Player player;
                string rank, timeAndName;
                size_t pos = line.find(',');
                if (pos != string::npos) {
                    rank = line.substr(0, pos);
                    timeAndName = line.substr(pos + 1); // Extract time and name part
                    size_t pos2 = timeAndName.find(',');

                    if (pos2 != string::npos) {
                        player.time = timeAndName.substr(0, pos2);
                        player.name = timeAndName.substr(pos2 + 1);
                        leaderboard.push_back(player);
                    } else {
                        cout << "Error: Second comma not found in line: " << line << endl;
                    }
                }
                else{
                    cout << "Error: Comma not found in line: " << line << endl;
                }
            }
            file.close();
        }
        return leaderboard;
    }



    static void displayLeaderboard(const std::vector<Player>& leaderboard) {
        int rank = 1;
        for (const auto& player : leaderboard) {
            cout << rank << ". " << player.time << " " << player.name << endl;
            rank++;
        }
    }




};