#ifndef GAME_H
#define GAME_H

// SDL stuff
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// std stuff
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>

// game classes
#include <constants.h>
#include <ball.h>
#include <board.h>

enum GameState
{
    PLAYER_ONE,
    PLAYER_TWO,
    PHYSICS_PROCESS
};

class Game
{
public:
    Game();
    ~Game();
    bool init();
    void run();
    void close();
private:
    // member functions
    void render();
    void make_balls();
    void process_physics(double dt);

    // Game objects
    Ball balls[16];
    Board gameBoard;
    Uint32 board_colour;
    Uint32 shoulder_colour;
    double angle;
    GameState currentState;

    // SDL objects

    //The window we'll be rendering to
    SDL_Window* window;

    //The surface contained by the window
    SDL_Surface* screenSurface;

    //The image we will load and show on the screen
    SDL_Surface* writeSurface;

    // Debug stuff
    std::stringstream timeText;
};

#endif
