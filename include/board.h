#ifndef BOARD_H
#define BOARD_H

#include <SDL3/SDL.h>

class Board
{
public:
    Board();
    ~Board();
    void draw_board(Uint32 *pixels, Uint32 board_colour, Uint32 shoulder_colour);
private:
    Uint32 board_colour;
    Uint32 shoulder_colour;
    unsigned int shoulder_width;
    double holes[6][2];
    int radius;
};

#endif
