#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <thread>
#include <chrono>

#include "../include/pente.h"
#include "../include/algorithm.h"
#include "../include/value.h"
#include "../include/timer.h"

#include <SFML/Graphics.hpp>

/* *********************************GAME CONTROLS************************q********* */
const float PLAYERLINEWEIGHT = 1.0f;
const float OPPONENTLINEWEIGHT = 1.0f;
const float PLAYERPAIRWEIGHT = 20.0f;
const float OPPONENTPAIRWEIGHT = 2.0f;

const double TIME_CONTROLLER = std::numeric_limits<double>::infinity();
int DEPTH_CONTROLLER = 1;
int NUMBER_OF_THREADS;

/* FOR SECOND IA */
const float PLAYERLINEWEIGHT_2 = 4.0f;
const float OPPONENTLINEWEIGHT_2 = 2.0f;
const float PLAYERPAIRWEIGHT_2 = 1.0f;
const float OPPONENTPAIRWEIGHT_2 = 1.0f;

const int AI_MOVE_DELAY = 1000; // miliseconds
const int WINDOW_HEIGHT = 750;
const int WINDOW_WIDTH = 700;
const int TOP_MARGIN = 70;
const int LEFT_MARGIN = 26;
const int BOARD_SPACING = 36;
const int PAWN_SIZE = 30;

std::ofstream file_stream;

void initializeTextures(sf::RenderWindow& window, sf::Texture& boardTexture, sf::Texture& whitePawnTexture, sf::Texture& blackPawnTexture) {
    if(!boardTexture.loadFromFile("gfx/board.png")) window.close();             // if texture loading failed -- exit
    if(!whitePawnTexture.loadFromFile("gfx/white-pawn.png")) window.close();    // if texture loading failed -- exit
    if(!blackPawnTexture.loadFromFile("gfx/black-pawn.png")) window.close();    // if texture loading failed -- exit
}

void initializeSprites(sf::Sprite& boardSprite, sf::Sprite& whitePawnSprite, sf::Sprite& blackPawnSprite,
              sf::Texture& boardTexture, sf::Texture& whitePawnTexture, sf::Texture& blackPawnTexture) {
    boardSprite.setTexture(boardTexture);
    whitePawnSprite.setTexture(whitePawnTexture);
    blackPawnSprite.setTexture(blackPawnTexture);
}

void initializeText(sf::RenderWindow& window, sf::Text& text, sf::Font& font) {
    if(!font.loadFromFile("fonts/arial/arial.ttf")) window.close(); // if font loading failed -- exit.
    text.setFont(font); // Add font
    text.setCharacterSize(14); // Set fontsize
    text.setColor(sf::Color(0, 0, 0)); // Set text color white
}

void drawPawns(Pente::board_type board, sf::RenderWindow& window,
               sf::Sprite& whitePawnSprite, sf::Sprite& blackPawnSprite) {
    int column = 0;
    int row = 0;
    int i = 0, j = 0;
    for(auto it = board.cbegin(); it != board.cend(); ++it, ++i){
        row = 0;
        for(auto in_it = it->cbegin(); in_it != it->cend(); ++in_it, ++j) {
            std::cout << std::setw(4);
            switch(*in_it) {
                case Player::PlayerColours::NONE:
                    break;
                case Player::PlayerColours::BLACK:
                    blackPawnSprite.setPosition(LEFT_MARGIN + row * BOARD_SPACING - PAWN_SIZE / 2, TOP_MARGIN + column * BOARD_SPACING - PAWN_SIZE / 2);
                    window.draw(blackPawnSprite);
                    break;
                case Player::PlayerColours::WHITE:
                    whitePawnSprite.setPosition(LEFT_MARGIN + row * BOARD_SPACING - PAWN_SIZE / 2, TOP_MARGIN + column * BOARD_SPACING - PAWN_SIZE / 2);
                    window.draw(whitePawnSprite);
                    break;
                default:
                    throw std::invalid_argument("This colour " + std::to_string((int)(*in_it)) + " does not exist!");
            }
            row++;
        }
        column++;
        j = 0;
    }
}

void handleMousePressed(sf::RenderWindow& window, sf::Event event, bool isGameWon, Pente& pente,
                  int& row, int& column, bool& playerOneTurn, bool& isGameStarted, bool isGameFinished, short& gameMode) {
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if(sf::Mouse::getPosition(window).x >= 0  && sf::Mouse::getPosition(window).y >= 0 &&
           sf::Mouse::getPosition(window).y <= WINDOW_HEIGHT && sf::Mouse::getPosition(window).x <= WINDOW_WIDTH) { // If click fits window boundaries
            if(!isGameFinished && isGameStarted && playerOneTurn && gameMode!= 3 && !isGameWon && sf::Mouse::getPosition(window).y >= TOP_MARGIN) { // If below top margin, so on board
                row        = int(round(round((sf::Mouse::getPosition(window).x) - LEFT_MARGIN) / BOARD_SPACING));
                column     = int(round(round((sf::Mouse::getPosition(window).y) - TOP_MARGIN) / BOARD_SPACING));
            } else if(sf::Mouse::getPosition(window).y < TOP_MARGIN) { // if over top margin, so on text field
                if(sf::Mouse::getPosition(window).x > 400 && sf::Mouse::getPosition(window).x < 612) { // "Start new game" buttons
                    if(sf::Mouse::getPosition(window).y >= 5 && sf::Mouse::getPosition(window).y <= 21) {
                        isGameStarted = true;
                        gameMode = 0;
                        playerOneTurn = true;
                    } else if(sf::Mouse::getPosition(window).y >= 25 && sf::Mouse::getPosition(window).y <= 42) {
                        isGameStarted = true;
                        gameMode = 1;
                        playerOneTurn = true;
                    } else if(sf::Mouse::getPosition(window).y >= 46) {
                        isGameStarted = true;
                        gameMode = 2;
                        playerOneTurn = true;
                    }
                    pente.initGame();
                } else if(sf::Mouse::getPosition(window).x >= 623 && sf::Mouse::getPosition(window).x <= 694) { // "Exit game" button
                    window.close();
                }
            }
        }
    }
    else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        pente.undoLastMove();
    }
}

void handleEvents(sf::RenderWindow& window, sf::Event event, bool isGameWon, Pente& pente,
                  int& row, int& column, bool& playerOneTurn, bool& isGameStarted, bool isGameFinished, short& gameMode) {
    while(window.pollEvent(event)){
        switch(event.type) {
            case sf::Event::Closed:
                window.close();
                break;
        }
    }
}

void displayText(Pente& pente, sf::RenderWindow& window, sf::Text& text, bool isGameFinished) {
    if(!isGameFinished) {
        text.setString("This is turn of player: " + pente.getStringCurrentPlayer());
        text.setPosition(10, 26);
        window.draw(text);
    } else {
        text.setString(pente.getStringCurrentPlayer() + " player won the game!!!");
        text.setPosition(10, 26);
        window.draw(text);
    }
    text.setString("White player beat " + std::to_string(pente.getCapturedPairsOfPlayer(Player::PlayerColours::WHITE)) + " pairs.");
    text.setPosition(220, 10);
    window.draw(text);
    text.setString("Black player beat " + std::to_string(pente.getCapturedPairsOfPlayer(Player::PlayerColours::BLACK)) + " pairs.");
    text.setPosition(220, 45);
    window.draw(text);
    text.setString("Start new game : player vs player");
    text.setPosition(400, 5);
    window.draw(text);
    text.setString("Start new game : player vs AI");
    text.setPosition(400, 26);
    window.draw(text);
    text.setString("Start new game : AI vs AI");
    text.setPosition(400, 47);
    window.draw(text);
    text.setString("Exit game");
    text.setPosition(630, 26);
    window.draw(text);
}

void AImove(Pente& pente, PenteEvaluation& eval, Algorithm& algo1,
            Move& bestMove, bool& playerOneTurn) {
    std::cout << "RUNNING WITH: " << NUMBER_OF_THREADS << " THREADS" << std::endl;
    std::cout << "ANALYZING FOR DEPTH: " << DEPTH_CONTROLLER << std::endl;
    file_stream << NUMBER_OF_THREADS << ";" << DEPTH_CONTROLLER << ";";
    if(NUMBER_OF_THREADS < 3)
    {
        std::cout << "RUNNING MIN MAX" << std::endl;
        Move bestMove1 = algo1.findBestMove(pente, eval, DEPTH_CONTROLLER, TIME_CONTROLLER, Algorithm::SearchType::MINMAX);
        file_stream << algo1.getLastMoveTime() << ";" << algo1.getLastNodesExplored() << ";";
    }
    else
    {
        file_stream << 0.0 << ";" << 0 << ";";
    }
    int minmax_nodes = algo1.getLastNodesExplored();
    std::cout << "RUNNING PARALLEL MIN MAX" << std::endl;
    Move bestMove2 = algo1.findBestMove(pente, eval, DEPTH_CONTROLLER, TIME_CONTROLLER, Algorithm::SearchType::PARALLEL_MINMAX_LOCAL);
    file_stream << algo1.getLastMoveTime() << ";" << minmax_nodes << ";";
    if(NUMBER_OF_THREADS < 3)
    {
        std::cout << "RUNNING ALPHA BETA" << std::endl;
        Move bestMove3 = algo1.findBestMove(pente, eval, DEPTH_CONTROLLER, TIME_CONTROLLER, Algorithm::SearchType::ALPHABETA);
        file_stream << algo1.getLastMoveTime() << ";" << algo1.getLastNodesExplored() << ";" << std::endl;
    }
    else
    {
        file_stream << 0.0 << ";" << 0 << ";" << std::endl;
    }

    std::cout << std::endl;

    if(DEPTH_CONTROLLER >= 5)
    {
        DEPTH_CONTROLLER = 1;
        ++NUMBER_OF_THREADS;
    }
    else
    {
        ++DEPTH_CONTROLLER;
    }

//    if(bestMove1 != bestMove2 || bestMove2 != bestMove3 || bestMove1 != bestMove3)
//    {
//        std::cout << "Minmax: " << bestMove1 << std::endl;
//        std::cout << "Parallel minmax: " << bestMove2 << std::endl;
//        std::cout << "Alphabeta: " << bestMove3 << std::endl;
//        throw std::logic_error("Searches gave different results!");
//    }

    bestMove = bestMove2;
//    std::cout << "Best move: " << bestMove << std::endl;
//    pente.markCell(std::make_pair(bestMove.row, bestMove.col));
    playerOneTurn = true;
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(AI_MOVE_DELAY));
}

void handleGameEnd(bool& isGameStarted, short& gameMode) {
    isGameStarted = false;
    gameMode = -1;
}

void placePawn(Pente& pente, int &row, int &column) {
    if(row != -1 && column != -1) {
        pente.markCell(std::make_pair(column, row));
//        PenteEvaluation(pente.getCurrentPlayer(),
//                        1.0f, 1.5f,
//                        2.0f, 2.5f).evaluate(pente);
        row = -1;
        column = -1;
    }
}

Timer *Timer::s_instance = 0;

int main() {
    // ------------------------------
    //// Pente game init
    Pente pente;
    pente.initGame();

    std::chrono::time_point <std::chrono::system_clock> start, stop;

    PenteEvaluation white_eval(Player::PlayerColours::WHITE,
                               PLAYERLINEWEIGHT_2, OPPONENTLINEWEIGHT_2,
                               PLAYERPAIRWEIGHT_2, OPPONENTPAIRWEIGHT_2);

    PenteEvaluation black_eval(Player::PlayerColours::BLACK,
                               PLAYERLINEWEIGHT, OPPONENTLINEWEIGHT,
                               PLAYERPAIRWEIGHT, OPPONENTPAIRWEIGHT);

    NUMBER_OF_THREADS = 2;
    file_stream.open("Measurements2.csv");
    file_stream << "NumberOfThreads;Depth;MinMaxTime[s];MinMaxNodes;ParallelMinMaxTime[s];ParallelMinMaxNodes;AlphaBetaTime[s];AlphaBetaNodes" << std::endl;
    // ------------------------------
    Algorithm algo1;
    Move bestMove;
    bool isGameStarted = false;
    bool isGameFinished = false;
    short gameMode = -1; // -1: game not started ; 0: player vs player ; 1: player vs AI ; 2: AI vs AI
    bool playerOneTurn = true;
    //// Coordinates
    int row = -1;
    int column = -1;
    //// SFML init
    // Window init
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pente game");
    //window.setKeyRepeatEnabled(false);
    // Main global event, handling every event
    sf::Event event;
    // Text object, getting strings and passing them to display
    sf::Text text;
    // Font object, keeping pointer to font file
    sf::Font font;
    // Textures objects
    sf::Texture boardTexture;
    sf::Texture whitePawnTexture;
    sf::Texture blackPawnTexture;
    // Sprites objects
    sf::Sprite boardSprite;
    sf::Sprite whitePawnSprite;
    sf::Sprite blackPawnSprite;
    // ------------------------------
    //// Init functions
    // Loading textures
    initializeTextures(window, boardTexture, whitePawnTexture, blackPawnTexture);
    // Set textures to sprites
    initializeSprites(boardSprite, whitePawnSprite, blackPawnSprite,
                  boardTexture, whitePawnTexture, blackPawnTexture);
    // Configure text
    initializeText(window, text, font);
    // Game loop
    while(window.isOpen() || NUMBER_OF_THREADS < 10){
        // Handle events
        handleEvents(window, event, pente.getIsWon(), pente, row, column, playerOneTurn, isGameStarted, pente.getIsWon(), gameMode);
        // Handle mouse clicks
        handleMousePressed(window, event, pente.getIsWon(), pente, row,
                                column, playerOneTurn, isGameStarted, pente.getIsWon(), gameMode);

        if(isGameStarted) {
            if(!pente.getIsWon()) {
                switch(gameMode) {
                    case 0:
                        break;
                    case 1:
                        if(!playerOneTurn) {
                            AImove(pente, black_eval, algo1, bestMove, playerOneTurn);
                        } else {
                            if(pente.getCurrentPlayer() == Player::PlayerColours::WHITE && row != -1 && column != -1) {
                                playerOneTurn = false;
                            }
                        }
                        break;
                    case 2:
                        if(pente.getCurrentPlayer() == Player::PlayerColours::WHITE)
                        {
                            AImove(pente, white_eval, algo1, bestMove, playerOneTurn);
                            start = Timer::instance()->Measure_Start();
                        }
                        else
                        {
                            AImove(pente, black_eval, algo1, bestMove, playerOneTurn);
                            stop = Timer::instance()->Measure_Stop();
                            Timer::instance()->Measure_screen(start, stop);
                        }
                        break;
                }
                placePawn(pente, row, column);
            } else {
                handleGameEnd(isGameStarted, gameMode);
            }
        }
        // Clear window
        window.clear(sf::Color(255, 255, 255)); // Fill window with white color
        // Draw board
        window.draw(boardSprite);
        // Draw pawns
        drawPawns(pente.getBoard(), window, whitePawnSprite, blackPawnSprite);
        // Draw text
        displayText(pente, window, text, pente.getIsWon());
        // Display it
        window.display();
    }

    file_stream.close();
    return 0;
}
