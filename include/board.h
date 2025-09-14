#ifndef BOARD_H
#define BOARD_H

#include <SDL3/SDL.h>
#include <cmath>
#include <constants.h>

class Board
{
public:
    Board();
    ~Board();
    void draw_board(Uint32 *pixels, Uint32 board_colour, Uint32 shoulder_colour);
    void draw_line(Uint32 *pixels, Uint32 line_colour, double line_width, double px, double py, double theta);
    void draw_line_segment(Uint32 *pixels, Uint32 line_colour, double line_width, double px, double py, double theta, double length, double start_offset);
private:
    Uint32 naked_board[SCREEN_WIDTH][SCREEN_HEIGHT];
    bool board_init;

    Uint32 board_colour;
    Uint32 shoulder_colour;
    unsigned int shoulder_width;
    double holes[6][2];
    int radius;
};

#endif
