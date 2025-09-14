#ifndef BALL_H
#define BALL_H

#include <SDL3/SDL.h>
#include <constants.h>
#include <cmath>
#include <iostream>

enum SinkState
{
    NOT_SUNK,
    SINKING,
    SUNK
};

class Ball
{
public:
    Ball() {}
    Ball(Uint32 colour_in, int r, double pos_x, double pos_y);
    inline void set_colour(Uint32 colour_in) { colour = colour_in; }
    inline void set_pos(double pos_x, double pos_y) {pos[0] = pos_x; pos[1] = pos_y;}
    inline void set_rad(int r) { radius = r; }
    inline void set_stripe() { stripes=true; }

    void draw_ball(Uint32 *pixels);
    void move_ball(double dt);
    void apply_force(double Fx, double Fy);
    double check_collision(Ball other_ball, double (&u_vec)[2]);
    void sinking_towards(double hx, double hy);
    ~Ball();

    double pos[2];
    double vel[2];
    int radius;
    double mass;
    SinkState sunk;

private:
    Uint32 colour;

    double F_net[2];
    double sinkHole[2];

    bool stripes;
};

#endif
