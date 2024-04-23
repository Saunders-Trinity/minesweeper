#include <iostream>
#include <fstream>
using namespace std;
#include "window.h"
#include <SFML/Graphics.hpp>
#include "random"
#include <cstdlib>
#include <ctime>
#include <chrono>



//welcome screen user input

void Button::keyboardInputName(sf::Event event, sf::Text &text2){
    static const int max_length = 10; // Maximum length of the name
    static bool first_letter = true; // Flag to capitalize first letter

    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode < 128 && text2.getString().getSize() < max_length) {
            if (event.text.unicode == 8) { // Backspace
                // Remove last character if the string is not empty
                string inputText = text2.getString().toAnsiString();
                if (!inputText.empty()) {
                    inputText.pop_back();
                    text2.setString(inputText);
                }
            } else if (isalpha(event.text.unicode)) { // Alphabetic character
                char typedChar = static_cast<char>(event.text.unicode);
                if (first_letter) {
                    // Capitalize first letter
                    typedChar = toupper(typedChar);
                    first_letter = false;
                } else {
                    // Convert to lowercase
                    typedChar = tolower(typedChar);
                }
                // Append the character to the string
                string inputText = text2.getString().toAnsiString();
                inputText += typedChar;
                text2.setString(inputText);
            }
        }
    }
}




int main()
{

    int colCount = 0;
    int rowCount = 0;
    int mineCount = 0;
    ifstream config("files/config.cfg");
    if(config.is_open()){
        config >> colCount;
        config >> rowCount;
        config >> mineCount;

    }

//Welcome to minesweeper

    sf::RenderWindow window(sf::VideoMode(colCount*32, rowCount*32 + 100), "MINESWEEPER",sf::Style::Close);
    window.setFramerateLimit(60);



//welcome message
    sf::Font font;
    font.loadFromFile("files/font.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(235, 200);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    text.setString("WELCOME TO MINESWEEPER");

//name for welcome screen
    sf::Text text1;
    text1.setFont(font);
    text1.setCharacterSize(20);
    text1.setFillColor(sf::Color::White);
    text1.setStyle(sf::Text::Bold | sf::Text::Regular);
    text1.setPosition(240, 325);
    text1.setString("Enter Your Name:");

//where the user types in their name
    sf::Text text2;
    text2.setFont(font);
    text2.setCharacterSize(20);
    text2.setFillColor(sf::Color::White);
    text2.setStyle(sf::Text::Bold | sf::Text::Regular);
    text2.setPosition(450,325);

    while (window.isOpen())//this is the welcome window
    {
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            if(event.type == sf::Event::TextEntered) {
                Button::keyboardInputName(event, text2);
            }
            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Enter){
                    if (!(text2.getString().isEmpty())) {
                        window.close();
                         }
                     }
                }
            }


//otherwise

//start rendering welcome screen
        window.clear(sf::Color::Blue);//display screen to blue
//draws everything  for welcome screen text
        window.draw(text);
        window.draw(text1);
        window.draw(text2);
        window.display();//display for welcome screen text

//start of game screen drawing
    }

    sf::RenderWindow gameWindow(sf::VideoMode(colCount*32, rowCount*32 + 100), "MINESWEEPER!",sf::Style::Close);

    sf::Texture texture1;//this is the tile hidden texture
    texture1.loadFromFile("files/images/tile_hidden.png");
    sf::Sprite spriteHidden;//this is the sprite for tile hidden
    spriteHidden.setTexture(texture1);

    sf::Texture texture2;//texture for flag
    texture2.loadFromFile("files/images/flag.png");
    sf::Sprite spriteFlag;
    spriteFlag.setTexture(texture2);

    //this vector work together to store flag positions
    std::vector<sf::Vector2f> rightclickPositions;

//for tile revealed
    sf::Texture texture3;
    texture3.loadFromFile("files/images/tile_revealed.png");
    sf::Sprite spriteRevealed;
    spriteRevealed.setTexture(texture3);

    std::vector<sf::Vector2f> leftClickPositions; //this is for tile revealed positions to be stored

//loading mine sprites
    sf::Texture texture4;
    texture4.loadFromFile("files/images/mine.png");
    sf::Sprite spriteMine;
    spriteMine.setTexture(texture4);





    //for tile implementation, this helps with tile generation do not delete
    vector<vector<Tile >> TileVector;//vector to hold sprites
    for (int x = 0; x < colCount; ++x) {//draws screen full hidden tiles
        vector<Tile> innerTiles;
        for (int y = 0; y < rowCount; ++y) {
            Tile temp = Tile(x * 32, y * 32);

            innerTiles.push_back(temp);
//            cout << x << " "<<  y << endl;

        }
        TileVector.push_back(innerTiles);
    }


    sf::Texture debugTexture;
    debugTexture.loadFromFile("files/images/debug.png");
    sf::Sprite debugSprite;
    debugSprite.setTexture(debugTexture);

    sf::Texture happyTexture;
    happyTexture.loadFromFile("files/images/face_happy.png");
    sf::Sprite happySprite;
    happySprite.setTexture(happyTexture);

    sf::Texture playTexture;
    playTexture.loadFromFile("files/images/play.png");
    sf::Sprite playSprite;
    playSprite.setTexture(playTexture);

    sf::Texture leaderboardTexture;
    leaderboardTexture.loadFromFile("files/images/leaderboard.png");
    sf::Sprite leaderboardSprite;
    leaderboardSprite.setTexture(leaderboardTexture);

    sf::Texture pauseTexture;
    pauseTexture.loadFromFile("files/images/pause.png");
    //digits for clock
    sf::Texture clockNumbers;
    clockNumbers.loadFromFile("files/images/digits.png");


    //This  helps generates the mines/ DO NOT DELETE
    TileVector[0][0].generateMines(gameWindow, TileVector, mineCount);

    //working on clock management
    chrono::steady_clock::time_point startTime;
    chrono::steady_clock::time_point currentTime;
    //for minutes
    startTime = chrono::steady_clock::now();
    sf::Sprite minuteDigit(clockNumbers);
    minuteDigit.setPosition(((colCount * 32) - 97), ((rowCount + 0.5) * 32) + 16);
    sf::Sprite minuteDigit2(clockNumbers);
    minuteDigit2.setPosition(((colCount * 32) - 76), ((rowCount + 0.5) * 32) + 16);

    //for seconds
    sf::Sprite secondsDigit(clockNumbers);
    secondsDigit.setPosition(((colCount * 32) - 54), ((rowCount + 0.5) * 32) + 16);
    sf::Sprite secondsDigit2(clockNumbers);
    secondsDigit2.setPosition(((colCount * 32) - 32), ((rowCount + 0.5) * 32) + 16);

    currentTime = chrono::steady_clock::now();
    //time when player starts the game
    auto elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime);
    int seconds = elapsedTime.count();
    int minutes = seconds / 60;
    seconds %= 60;
    sf::IntRect minuteRect((minutes / 10) * 21, 0, 21, 32);
    sf::IntRect minuteRect2((minutes % 10) * 21, 0, 21, 32);
    sf::IntRect secondsRect((seconds / 10) * 21, 0, 21, 32);
    sf::IntRect secondsRect2((seconds % 10) * 21, 0, 21, 32);

    while (gameWindow.isOpen())
    {

        sf::Event event;
        while (gameWindow.pollEvent(event)) {

            //stores mouse click positions-DO NOT DELETE
            sf::Vector2i mousePosition = sf::Mouse::getPosition(gameWindow);

            if (event.type == sf::Event::Closed) {
                gameWindow.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                //places a flag on top of tile hidden - this works
                if (!TileVector[mousePosition.x / 32][mousePosition.y / 32].isRevealed) {
            TileVector[mousePosition.x / 32][mousePosition.y / 32].isFlag = !TileVector[mousePosition.x / 32][mousePosition.y / 32].isFlag;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left ) {
                //got rid of regular reveal because recursive reveal now works

                //for recursive reveal - This works
                int row = mousePosition.x / 32;
                int col = mousePosition.y / 32;
                TileVector[mousePosition.x / 32][mousePosition.y / 32].recursiveReveal(gameWindow, TileVector, row, col);

                //for button clicks - These work
                sf::FloatRect debugButtonBounds = debugSprite.getGlobalBounds();
                sf::FloatRect faceButtonBounds = happySprite.getGlobalBounds();
                sf::FloatRect pauseButtonBounds = playSprite.getGlobalBounds();
                sf::FloatRect leaderboardButtonBounds = leaderboardSprite.getGlobalBounds();
                if(debugButtonBounds.contains(sf::Vector2f(mousePosition))){
                    //all mines reveal
                        cout << "debug button clicked" << endl;
                        TileVector[0][0].revealallMines(gameWindow,TileVector);

                    }
                else if(faceButtonBounds.contains(sf::Vector2f(mousePosition))){
                        //this is for the happy face button
                        //suposed to start game from beginning
                        cout << "happy face clicked" << endl;
                        happyTexture.loadFromFile("files/images/face_lose.png");
                        happySprite.setTexture(happyTexture);

                    }
                else if(pauseButtonBounds.contains(sf::Vector2f(mousePosition))){
                        cout << "pause button clicked" << endl;
                        //this is for the pause/play sprite
                        //pauses the time
                        //flips the pause button to play button
                        playSprite.setTexture(pauseTexture);

                }
                else if(Tile::checkWinCondition(TileVector)  or  leaderboardButtonBounds.contains(sf::Vector2f(mousePosition))) {
                    //if user wins the game or they click the leaderboard button
                        cout << "leaderboard button clicked" << endl;
                        sf::RenderWindow leaderboardWindow(sf::VideoMode(colCount * 16, (rowCount * 16)+ 50), "Leaderboard");
                        sf::Text leaderboardText;
                        leaderboardText.setFont(font);
                        leaderboardText.setCharacterSize(30);
                        leaderboardText.setFillColor(sf::Color::White);
                        leaderboardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
                        leaderboardText.setPosition(100,50);
                        leaderboardText.setString("LEADERBOARD");
                        sf::Text leaderboardInfo;

                        while (leaderboardWindow.isOpen()) {
                            while (leaderboardWindow.pollEvent(event)) {
                                if (event.type == sf::Event::Closed) {
                                    leaderboardWindow.close();
                                }
                            }
                            leaderboardWindow.clear(sf::Color::Blue);
                            // Draw leaderboard here
                            vector<Player> leaderboard = Player::readLeaderboardFromFile("leaderboard.txt");
                            Player::displayLeaderboard(leaderboard);
                            leaderboardWindow.draw(leaderboardText);
                            leaderboardWindow.display();
                        }
                }
            }


            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left ){

                //if the user left clicks and it is a mine tile
                //reveals all other mine function
                int x = event.mouseButton.x / 32;
                int y = event.mouseButton.y / 32;

                if (TileVector[x][y].isMine) {//if user clicks and it is a mine
                    TileVector[x][y].getAdjacentMines(gameWindow,TileVector, rowCount, colCount);//this works
                    TileVector[x][y].revealallMines(gameWindow,TileVector);//this works
                    TileVector[x][y].revealallTiles(gameWindow,TileVector);//does not work
                    TileVector[0][0].YouLose(gameWindow,TileVector);
                    }

            }
        }






        //for buttons and their positions, do not delete
        debugSprite.setPosition(496,528);
        happySprite.setPosition(368,528);
        playSprite.setPosition(560,528);
        leaderboardSprite.setPosition(624,528);

        gameWindow.clear(sf::Color(255, 255, 255, 128));



        //this draws tiles, DO NOT DELETE
        //two for loops iterate through tile vectors and use draw function on both
        for(int i = 0; i < TileVector.size(); i++){
            for(int j = 0; j < TileVector[i].size(); j++){
                TileVector[i][j].drawTile(gameWindow,TileVector,i,j);
            }

        }
        //drawing the digits
        minuteDigit.setTextureRect(minuteRect);
        gameWindow.draw(minuteDigit);
        minuteDigit2.setTextureRect(minuteRect2);
        gameWindow.draw(minuteDigit2);
        //for drawing the seconds
        secondsDigit.setTextureRect(secondsRect);
        gameWindow.draw(secondsDigit);
        secondsDigit2.setTextureRect(secondsRect2);
        gameWindow.draw(secondsDigit2);


        gameWindow.draw(leaderboardSprite);
        gameWindow.draw(playSprite);
        gameWindow.draw(happySprite);
        gameWindow.draw(debugSprite);

        gameWindow.display();

    }


    return 0;
}


