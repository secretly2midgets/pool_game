#include <ball.h>

Ball::Ball(Uint32 colour_in, int r, double pos_x, double pos_y)
:
colour(colour_in),
radius(r),
stripes(false),
sunk(NOT_SUNK)
{
    pos[0] = pos_x;
    pos[1] = pos_y;

    vel[0] = 0.0;
    vel[1] = 0.0;

    F_net[0] = 0.0;
    F_net[1] = 0.0;

    mass = 1.0;

    sinkHole[0] = 0.0;
    sinkHole[1] = 1.0;
}

Ball::~Ball()
{
    vel[0] = 0.0;
    vel[1] = 0.0;

    F_net[0] = 0.0;
    F_net[1] = 0.0;

    mass = 1.0;
}

void Ball::draw_ball(Uint32 *pixels)
{
    if (sunk != SUNK)
    {
        for (unsigned int x=pos[0]-radius; x<pos[0]+radius; ++x)
        {
            for (unsigned int y=pos[1]-radius; y<pos[1]+radius; ++y)
            {
                double r_2 = (x-pos[0])*(x-pos[0]) + (y-pos[1])*(y-pos[1]);
                if (r_2 < radius*radius)
                {
                    if ( !stripes || ( abs(y-pos[1]) < radius/2 ) )
                        pixels[x + y*SCREEN_WIDTH] = colour;
                    else
                        pixels[x + y*SCREEN_WIDTH] = WHITE;
                }

            }
        }
    }
}

void Ball::move_ball(double dt)
{
    switch(sunk)
    {
        case NOT_SUNK:
        {
            vel[0] += dt*F_net[0]/mass;
            vel[1] += dt*F_net[1]/mass;

            pos[0] += dt*vel[0];
            pos[1] += dt*vel[1];

            // drag
            vel[0] *= 0.99;
            vel[1] *= 0.99;

            if (std::abs(vel[0]) < vel_tol)
                vel[0] = 0.0;
            if (std::abs(vel[1]) < vel_tol)
                vel[1] = 0.0;

            F_net[0] = 0.0;
            F_net[1] = 0.0;
            break;
        }
        case SINKING:
        {
            vel[0] = sinkHole[0] - pos[0];
            vel[1] = sinkHole[1] - pos[1];

            pos[0] += dt*vel[0];
            pos[1] += dt*vel[1];

            if (radius > 0)
                radius -= 1;

            if ((std::abs(vel[0]) < 5.0) && (std::abs(vel[1]) < 5.0))
                sunk = SUNK;
            break;
        }
    }
}

void Ball::apply_force(double Fx, double Fy)
{
    F_net[0] += Fx;
    F_net[1] += Fy;
}

// check for collision, return penetration depth (negative if not collided)
// also set the unit vector for the collision
double Ball::check_collision(Ball other_ball, double (&u_vec)[2])
{
    double dx = other_ball.pos[0] - pos[0];
    double dy = other_ball.pos[1] - pos[1];
    double dist = std::sqrt(dx*dx + dy*dy);
    double rads = double(other_ball.radius + radius);

    u_vec[0] = dx/dist;
    u_vec[1] = dy/dist;

    return rads - dist;
}

void Ball::sinking_towards(double hx, double hy)
{
    sunk = SINKING;
    sinkHole[0] = hx;
    sinkHole[1] = hy;
}
