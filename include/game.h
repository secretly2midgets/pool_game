#ifndef GAME_H
#define GAME_H

// SDL stuff
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// std stuff
#include <string>
#include <iostream>
#include <cmath>

// game classes
#include <constants.h>
#include <ball.h>
#include <board.h>

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
    void process_physics();

    // Game objects
    Ball cue_ball;
    Ball balls[15];
    Board gameBoard;
    Uint32 board_colour;
    Uint32 shoulder_colour;

    // SDL objects

    //The window we'll be rendering to
    SDL_Window* window;

    //The surface contained by the window
    SDL_Surface* screenSurface;

    //The image we will load and show on the screen
    SDL_Surface* writeSurface;
};

#endif
