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
    PLAYER_TURN,
    PHYSICS_PROCESS,
    HITTING_BALL,
    PLACING_CUE_BALL
};

enum Player
{
    PLAYER_ONE,
    PLAYER_TWO
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
    void handle_events(SDL_Event e, bool& quit);
    inline double get_hitting_force(double time)
    {
        return std::min((time - spaceStartTime)*hitSpeedScale, maxHitStrength);
    }
    double hitSpeedScale;
    double maxHitStrength;

    // Game objects
    Ball balls[16];
    Board gameBoard;
    Uint32 board_colour;
    Uint32 shoulder_colour;

    // 1 if turning right, -1 if left, 0 if not at all
    double turning;
    // if shift is held this is 0.1 if not it is 1.0
    double shiftToSlow;
    double angle;
    // contains the amount of time that space has been held down which controls how hard the ball will be hit
    double spaceStartTime;


    GameState currentState;
    Player whoseTurn;

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
