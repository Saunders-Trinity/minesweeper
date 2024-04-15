#include <iostream>
#include <fstream>
using namespace std;
#include "window.h"
#include <SFML/Graphics.hpp>



//welcome screen user input
void Button::keyboardInputName(sf::Event event, sf::Text &text2){
    //take in keyboard letter press and backspace
    //text 2 is the user name on the screen
    //set the input to the string of text 2
    if (event.type == sf::Event::TextEntered){
        if (event.text.unicode < 128){
            if (event.text.unicode == 8){//for backspace
                string inputText = text2.getString();
                if (!inputText.empty()) {
                    inputText.pop_back();
                    text2.setString(inputText);
                }
            }
            else{
                char typedChar = static_cast<char>(event.text.unicode);
                string inputText = text2.getString();
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
    if(config.is_open()){//25,16,50
        //cout << "config open successful" << endl;
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



    while (window.isOpen())//leaderboard goes in while loop
    {
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            if(event.type == sf::Event::TextEntered){
                if (event.key.code == sf::Keyboard::N or event.key.code == sf::Keyboard::Enter) {
                    if (!(text2.getString().isEmpty())) {
                        window.close();
                    }
                }
                else {
                    Button::keyboardInputName(event, text2);//
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

    sf::RenderWindow gameWindow(sf::VideoMode(colCount*32, rowCount*32 + 100), "MINESWEEPER",sf::Style::Close);
    int windowWidth = colCount * 32;
    int windowHeight = rowCount * 32 + 100;


    sf::Texture texture1;//this is the tile hidden texture
    texture1.loadFromFile("files/images/tile_hidden.png");
    sf::Sprite spriteHidden;//this is the sprite for tile hidden
    spriteHidden.setTexture(texture1);

    sf::Texture texture2;//texture for flag
    texture2.loadFromFile("files/images/flag.png");
    sf::Sprite spriteFlag;
    spriteFlag.setTexture(texture2);

    sf::Vector2f rightclickPosition;//these two vectors work together to store flag positions
    std::vector<sf::Vector2f> rightclickPositions;

//for tile revealed
    sf::Texture texture3;
    texture3.loadFromFile("files/images/tile_revealed.png");
    sf::Sprite spriteRevealed;
    spriteRevealed.setTexture(texture3);

    std::vector<sf::Vector2f> leftClickPositions; //this is for tile revealed positions to be stored

//for mine to loaded on screen
    sf::Texture texture4;
    texture4.loadFromFile("files/images/mine.png");
    sf::Sprite spriteMine;
    spriteMine.setTexture(texture4);

//starting implementation of mines
    bool minesGenerated = false;



    while (gameWindow.isOpen())
    {
        sf::Event event;
        int repetitionsX = windowWidth / texture1.getSize().x + 1;
        int repetitionsY = windowHeight / texture1.getSize().y + 1;
        while (gameWindow.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                gameWindow.close();
            }
        }
        gameWindow.clear(sf::Color(128, 128, 128, 128));
        gameWindow.draw(spriteHidden);//draws one sprite
        for (int x = 0; x < repetitionsX; ++x) {//draws screen full hidden tiles
            for (int y = 0; y < repetitionsY; ++y) {
                spriteHidden.setPosition(x * texture1.getSize().x, y * texture1.getSize().y);
                gameWindow.draw(spriteHidden);
            }
        }
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
        {
            //place a flag on top of tile hidden
            sf::Vector2i mousePosition = sf::Mouse::getPosition(gameWindow);
            rightclickPosition = gameWindow.mapPixelToCoords(mousePosition);
            rightclickPositions.push_back(rightclickPosition);
        }


        for (const auto& pos : rightclickPositions) {//this draws multiple flags in the spots where the user clicks
            spriteFlag.setPosition(pos);
            gameWindow.draw(spriteFlag);
        }

        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
            //left click does a tile revealed action
            sf::Vector2i mousePosition = sf::Mouse::getPosition(gameWindow);
            sf::Vector2f clickPosition = gameWindow.mapPixelToCoords(mousePosition);
            leftClickPositions.push_back(clickPosition);//saves where the user clicks
            spriteRevealed.setPosition(clickPosition);




        }

        /*Tile tile1(true);
        Tile tile2(false);
        tile2.setAdjacentMines(2);//this puts 2 mines adjacent of this tile
        tile2.reveal();
        tile2.toggleFlag();*/


        gameWindow.display();

    }

    return 0;
}


