#ifndef BALL_H
#define BALL_H

#include <SDL3/SDL.h>
#include <constants.h>
#include <cmath>

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
    ~Ball();

    double pos[2];
    int radius;
private:
    Uint32 colour;

    double vel[2];
    double F_net[2];
    double mass;

    bool stripes;
};

#endif
