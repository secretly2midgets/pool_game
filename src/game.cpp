#include <game.h>

// Constructor
Game::Game()
:
window(nullptr),
screenSurface(nullptr),
writeSurface(nullptr),
gameBoard()
{
    cue_ball = Ball(0, 10, 0, 0);
    for (unsigned int i = 0; i < 15; ++i)
    {
        balls[i] = Ball(0, 10, 0, 0);
    }
}

// Destructor
Game::~Game()
{

}

// Initialize SDL into video mode, then create a window and a screen surface
bool Game::init()
{
    bool success = true;

    if (SDL_Init( SDL_INIT_VIDEO ) == false)
    {
        SDL_Log("SDL initialize failed!  Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        window = SDL_CreateWindow( "SDL3 Tutorial: Hello SDL3", SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
        if ( window == nullptr )
        {
            SDL_Log( "Window could not be created! SDL error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            screenSurface = SDL_GetWindowSurface( window );
            writeSurface  = SDL_CreateSurface( SCREEN_WIDTH, SCREEN_HEIGHT, SDL_PIXELFORMAT_RGBA32 );
            board_colour    = SDL_MapSurfaceRGB(writeSurface, 0, 255, 0);
            shoulder_colour = SDL_MapSurfaceRGB(writeSurface, 0, 125, 0);

            make_balls();
        }
    }
    return success;
}

void Game::make_balls()
{
    // set ball colours
    Uint32 ball_colours[16];
    ball_colours[0]  = SDL_MapSurfaceRGB(writeSurface, 255, 255, 0);     // yellow
    ball_colours[0]  = SDL_MapSurfaceRGB(writeSurface, 255, 255, 0);     // yellow
    ball_colours[1]  = SDL_MapSurfaceRGB(writeSurface, 0,   0,   255);   // blue
    ball_colours[2]  = SDL_MapSurfaceRGB(writeSurface, 255, 0,   0);     // red
    ball_colours[3]  = SDL_MapSurfaceRGB(writeSurface, 255, 0,   255);   // purple
    ball_colours[4]  = SDL_MapSurfaceRGB(writeSurface, 255, 140, 0);     // orange
    ball_colours[5]  = SDL_MapSurfaceRGB(writeSurface, 0,   80,  0);     // green
    ball_colours[6]  = SDL_MapSurfaceRGB(writeSurface, 180, 50,  0);     // dark red
    ball_colours[7]  = SDL_MapSurfaceRGB(writeSurface, 0,   0,   0);     // black
    ball_colours[8]  = SDL_MapSurfaceRGB(writeSurface, 255, 255, 0);     // yellow
    ball_colours[9]  = SDL_MapSurfaceRGB(writeSurface, 0,   0,   255);   // blue
    ball_colours[10] = SDL_MapSurfaceRGB(writeSurface, 255, 0,   0);     // red
    ball_colours[11] = SDL_MapSurfaceRGB(writeSurface, 255, 0,   255);   // purple
    ball_colours[12] = SDL_MapSurfaceRGB(writeSurface, 255, 140, 0);     // orange
    ball_colours[13] = SDL_MapSurfaceRGB(writeSurface, 0,   80,  0);     // green
    ball_colours[14] = SDL_MapSurfaceRGB(writeSurface, 180, 50,  0);     // dark red

    cue_ball.set_colour( SDL_MapSurfaceRGB(writeSurface, 255, 255, 255) );
    cue_ball.set_rad(10);
    cue_ball.set_pos(SCREEN_WIDTH/3, SCREEN_HEIGHT/2);

    int triangle_row = 0;
    int row_count = 0;
    int ball_pos[15][2];
    for (unsigned int i = 0; i < 15; ++i)
    {
        ball_pos[i][0] = 3*SCREEN_WIDTH/5 + 30*triangle_row;
        ball_pos[i][1] = SCREEN_HEIGHT/2  + row_count*30 - (triangle_row)*15;

        row_count = row_count + 1;
        if (row_count > triangle_row)
        {
            row_count = 0;
            triangle_row = triangle_row + 1;
        }
    }

    int pos_arranged[15];
    pos_arranged[0]  = 0;
    pos_arranged[1]  = 13;
    pos_arranged[2]  = 2;
    pos_arranged[3]  = 11;
    pos_arranged[4]  = 7;
    pos_arranged[5]  = 9;
    pos_arranged[6]  = 6;
    pos_arranged[7]  = 4;
    pos_arranged[8]  = 8;
    pos_arranged[9]  = 5;
    pos_arranged[10] = 10;
    pos_arranged[11] = 3;
    pos_arranged[12] = 12;
    pos_arranged[13] = 1;
    pos_arranged[14] = 14;

    for (unsigned int i = 0; i < 15; ++i)
    {
        balls[i].set_colour( ball_colours[i] );
        balls[i].set_rad(10);
        balls[i].set_pos(ball_pos[pos_arranged[i]][0], ball_pos[pos_arranged[i]][1]);

        if (i > 7)
            balls[i].set_stripe();
    }
}

// main game loop
void Game::run()
{
    // Loop flags
    bool quit(false);
    SDL_Event e;
    SDL_zero(e);

    // main game loop
    while (!quit)
    {
        // events
        while (SDL_PollEvent( &e ) == true)
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
            else if( e.type == SDL_EVENT_KEY_DOWN )
            {
                if (e.key.key == SDLK_SPACE)
                {
                    cue_ball.apply_force(1.0, 0.0);
                }
            }
        }

        // physics
        process_physics();

        // render
        render();
    }
}

void Game::render()
{
    // do the CPU per pixel rendering
    Uint32 *pixels = (Uint32 *) writeSurface->pixels;
    gameBoard.draw_board(pixels, board_colour, shoulder_colour);
    cue_ball.draw_ball(pixels);
    for (unsigned int i = 0; i < 15; ++i)
    {
        balls[i].draw_ball(pixels);
    }

    // then put everything on the screen
    SDL_BlitSurface( writeSurface, nullptr, screenSurface, nullptr );
    SDL_UpdateWindowSurface( window );
}

void Game::process_physics()
{
    double dt = 1.0;
    double shoulder_width = 50.0;
    double e = 1.0;
    // move balls
    cue_ball.move_ball(dt);
    if (cue_ball.pos[0] - cue_ball.radius <= shoulder_width)
    {
        cue_ball.pos[0] = shoulder_width + cue_ball.radius;
        cue_ball.vel[0] *= -e;
    }
    else if (cue_ball.pos[0] + cue_ball.radius >= SCREEN_WIDTH - shoulder_width)
    {
        cue_ball.pos[0] = SCREEN_WIDTH - shoulder_width - cue_ball.radius;
        cue_ball.vel[0] *= -e;
    }
    if (cue_ball.pos[1] - cue_ball.radius <= shoulder_width)
    {
        cue_ball.pos[1] = shoulder_width + cue_ball.radius;
        cue_ball.vel[1] *= -e;
    }
    else if (cue_ball.pos[1] + cue_ball.radius >= SCREEN_HEIGHT - shoulder_width)
    {
        cue_ball.pos[1] = SCREEN_HEIGHT - shoulder_width - cue_ball.radius;
        cue_ball.vel[1] *= -e;
    }
    for (unsigned int i = 0; i < 15; ++i)
    {
        balls[i].move_ball(dt);
        if (balls[i].pos[0] - balls[i].radius <= shoulder_width)
        {
            balls[i].pos[0] = shoulder_width + balls[i].radius;
            balls[i].vel[0] *= -e;
        }
        else if (balls[i].pos[0] + balls[i].radius >= SCREEN_WIDTH - shoulder_width)
        {
            balls[i].pos[0] = SCREEN_WIDTH - shoulder_width - balls[i].radius;
            balls[i].vel[0] *= -e;
        }
        if (balls[i].pos[1] - balls[i].radius <= shoulder_width)
        {
            balls[i].pos[1] = shoulder_width + balls[i].radius;
            balls[i].vel[1] *= -e;
        }
        else if (balls[i].pos[1] + balls[i].radius >= SCREEN_HEIGHT - shoulder_width)
        {
            balls[i].pos[1] = SCREEN_HEIGHT - shoulder_width - balls[i].radius;
            balls[i].vel[1] *= -e;
        }
    }

    // detect collisions

    double q_collided = -1.0;
    double q_u[2] = {0.0, 0.0};
    double collided[15] = {-1.0};
    double unit_vec[15][2];
    for (unsigned int i = 0; i < 15; ++i)
    {
        double q_pen_dist = balls[i].check_collision(cue_ball, q_u);
        if (q_pen_dist > 0)
        {
            q_collided = q_pen_dist;

            double v1 = cue_ball.vel[0]*q_u[0] + cue_ball.vel[1]*q_u[1];
            double v2 = balls[i].vel[0]*q_u[0] + balls[i].vel[1]*q_u[1];

            double total_mass = cue_ball.mass + balls[i].mass;

            double delta_v1 = (balls[i].mass/total_mass)*(v2 - v1)*(1+e);
            double delta_v2 = (cue_ball.mass/total_mass)*(v1 - v2)*(1+e);

            double F1 = cue_ball.mass*delta_v1/dt;
            double F2 = balls[i].mass*delta_v2/dt;

            cue_ball.apply_force(F1*q_u[0], F1*q_u[1]);
            balls[i].apply_force(F2*q_u[0], F2*q_u[1]);

        }
        for (unsigned int j = i+1; j < 15; ++j)
        {
            double pen_dist = balls[i].check_collision(balls[j], unit_vec[i]);
            if (pen_dist > 0)
            {
                double v1 = balls[i].vel[0]*unit_vec[i][0] + balls[i].vel[1]*unit_vec[i][1];
                double v2 = balls[j].vel[0]*unit_vec[i][0] + balls[j].vel[1]*unit_vec[i][1];

                double total_mass = balls[i].mass + balls[j].mass;

                double delta_v1 = (balls[j].mass/total_mass)*(v2 - v1)*(1+e);
                double delta_v2 = (balls[i].mass/total_mass)*(v1 - v2)*(1+e);

                double F1 = balls[i].mass*delta_v1/dt;
                double F2 = balls[j].mass*delta_v2/dt;

                balls[i].apply_force(F1*unit_vec[i][0], F1*unit_vec[i][1]);
                balls[j].apply_force(F2*unit_vec[i][0], F2*unit_vec[i][1]);
            }

        }
    }



    // handle collisions
//     if (q_collided > 0)
//     {
//         double needed_force = q_collided/dt;
//         cue_ball.apply_force(needed_force*q_u[0], needed_force*q_u[1]);
//     }
//     for (unsigned int i = 0; i < 15; ++i)
//     {
//         if (collided[i] > 0)
//         {
//             SDL_Log("HERE");
//             // need to move collided[i] units in the next dt
//             // direction is the vector between the centres of i and j
//             // to move that many units, need to be going at least c[i]/dt velocity next sim step
//             // so need to add velocity in that direction to reach that
//             double needed_force = collided[i]/dt;
//             balls[i].apply_force(needed_force*q_u[0], needed_force*q_u[1]);
//         }
//     }
}

void Game::close()
{
    SDL_DestroyWindow( window );
    window = nullptr;
    SDL_DestroySurface(screenSurface);
    screenSurface = nullptr;
    SDL_DestroySurface(writeSurface);
    writeSurface = nullptr;

    SDL_Quit();
}
