#include <game.h>

// Constructor
Game::Game()
:
window(nullptr),
screenSurface(nullptr),
writeSurface(nullptr),
gameBoard()
{
    angle = 0.0;
    for (unsigned int i = 0; i < 16; ++i)
    {
        balls[i] = Ball(0, 10, 0, 0);
    }
    currentState = PLAYER_TURN;
    whoseTurn = PLAYER_ONE;
    timeText.str("");
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
    ball_colours[0]  = WHITE;
    ball_colours[1]  = YELLOW;
    ball_colours[2]  = BLUE;
    ball_colours[3]  = RED;
    ball_colours[4]  = PURPLE;
    ball_colours[5]  = ORANGE;
    ball_colours[6]  = GREEN;
    ball_colours[7]  = DARKRED;
    ball_colours[8]  = BLACK;
    ball_colours[9]  = YELLOW;
    ball_colours[10]  = BLUE;
    ball_colours[11] = RED;
    ball_colours[12] = PURPLE;
    ball_colours[13] = ORANGE;
    ball_colours[14] = GREEN;
    ball_colours[15] = DARKRED;


    int triangle_row = 0;
    int row_count = 0;
    int ball_pos[16][2];
    ball_pos[0][0] = SCREEN_WIDTH/3.0;
    ball_pos[0][1] = SCREEN_HEIGHT/2.0;
    for (unsigned int i = 1; i < 16; ++i)
    {
        ball_pos[i][0] = 3.0*SCREEN_WIDTH/5 + 30.0*triangle_row;
        ball_pos[i][1] = SCREEN_HEIGHT/2  + row_count*30.0 - (triangle_row)*15.0;

        row_count = row_count + 1;
        if (row_count > triangle_row)
        {
            row_count = 0;
            triangle_row = triangle_row + 1;
        }
    }

    int pos_arranged[16];
    pos_arranged[0]  = 0;
    pos_arranged[1]  = 1;
    pos_arranged[2]  = 14;
    pos_arranged[3]  = 3;
    pos_arranged[4]  = 12;
    pos_arranged[5]  = 8;
    pos_arranged[6]  = 10;
    pos_arranged[7]  = 7;
    pos_arranged[8]  = 5;
    pos_arranged[9]  = 9;
    pos_arranged[10] = 6;
    pos_arranged[11] = 11;
    pos_arranged[12] = 4;
    pos_arranged[13] = 13;
    pos_arranged[14] = 2;
    pos_arranged[15] = 15;

    for (unsigned int i = 0; i < 16; ++i)
    {
        balls[i].set_colour( ball_colours[i] );
        balls[i].set_rad(10);
        balls[i].set_pos(ball_pos[pos_arranged[i]][0], ball_pos[pos_arranged[i]][1]);

        if (i > 8)
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

    // time stuff
    double elapsed_time = 0.0;
    double last_time = SDL_GetTicksNS();
    // debug time stuff
    double eventTime   = 0.0;
    double physicsTime = 0.0;
    double renderTime  = 0.0;
    double waitTime    = 0.0;

    // main game loop
    while (!quit)
    {
        // check how long frame took
        elapsed_time =  SDL_GetTicksNS() - last_time;
        last_time = SDL_GetTicksNS();

        // events
        handle_events(e, quit);
        eventTime = SDL_GetTicksNS() - last_time;

        // physics
        process_physics(frametime);

        physicsTime = SDL_GetTicksNS() - last_time - eventTime;

        // render
        render();

        renderTime = SDL_GetTicksNS() - last_time - eventTime - physicsTime;

        waitTime = 0.0;
        if (1000.0*frametime > (1e-6)*elapsed_time)
        {
            waitTime = (1e9)*frametime - elapsed_time;
            SDL_Delay(1000.0*frametime - (1e-6)*elapsed_time);
        }

        timeText << "Events: " << (1e-6)*eventTime << "| Physics: " << (1e-6)*physicsTime << "| Render: " << (1e-6)*renderTime << "| Wait: " << (1e-6)*waitTime << "\r";
        std::cout << timeText.str();
        timeText.str("");
    }
}

void Game::handle_events(SDL_Event e, bool& quit)
{
    while (SDL_PollEvent( &e ) == true)
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            quit = true;
        }
        else if( e.type == SDL_EVENT_KEY_DOWN )
        {
            if (e.key.key == SDLK_D)
            {
                angle = 0.0;
            }
            else if (e.key.key == SDLK_A)
            {
                angle = M_PI; //180.0;
            }
            else if (e.key.key == SDLK_W)
            {
                angle = 3.0*M_PI/2.0; //90.0;
            }
            else if (e.key.key == SDLK_S)
            {
                angle = M_PI/2.0;  //270.0;
            }
            else if (e.key.key == SDLK_Q)
            {
                angle -= 0.1*M_PI; //10.0;
            }
            else if (e.key.key == SDLK_E)
            {
                angle += 0.1*M_PI; //10.0;
            }
            else if (e.key.key == SDLK_SPACE)
            {
                currentState = HITTING_BALL;
                //balls[0].apply_force(1000.0*std::cos(angle), 1000.0*std::sin(angle));
            }
        }
    }
}

void Game::render()
{
    // do the CPU per pixel rendering
    Uint32 *pixels = (Uint32 *) writeSurface->pixels;
    gameBoard.draw_board(pixels, board_colour, shoulder_colour);

    if (currentState == PLAYER_TURN)
    {
        switch (whoseTurn)
        {
            case PLAYER_ONE:
                // draw line showing where the shot will go
                gameBoard.draw_line(pixels, GREY, 1.0, balls[0].pos[0], balls[0].pos[1], angle);
                // draw pool cue
                gameBoard.draw_line_segment(pixels, BROWN,      3.0, balls[0].pos[0], balls[0].pos[1], std::fmod(angle + M_PI, 2.0*M_PI), 250.0, 50.0);
                gameBoard.draw_line_segment(pixels, RED,        3.0, balls[0].pos[0], balls[0].pos[1], std::fmod(angle + M_PI, 2.0*M_PI), 10.0,  100.0);
                gameBoard.draw_line_segment(pixels, LIGHTGREY,  3.0, balls[0].pos[0], balls[0].pos[1], std::fmod(angle + M_PI, 2.0*M_PI), 3.0,   50.0);
                break;
            case PLAYER_TWO:
                // draw line showing where the shot will go
                gameBoard.draw_line(pixels, GREY, 1.0, balls[0].pos[0], balls[0].pos[1], angle);
                // draw pool cue
                gameBoard.draw_line_segment(pixels, BROWN,      3.0, balls[0].pos[0], balls[0].pos[1], std::fmod(angle + M_PI, 2.0*M_PI), 250.0, 50.0);
                gameBoard.draw_line_segment(pixels, BLUE,       3.0, balls[0].pos[0], balls[0].pos[1], std::fmod(angle + M_PI, 2.0*M_PI), 10.0,  100.0);
                gameBoard.draw_line_segment(pixels, LIGHTGREY,  3.0, balls[0].pos[0], balls[0].pos[1], std::fmod(angle + M_PI, 2.0*M_PI), 3.0,   50.0);
                break;
        }
    }


    for (unsigned int i = 0; i < 16; ++i)
    {
        balls[i].draw_ball(pixels);
    }

    // then put everything on the screen
    SDL_BlitSurface( writeSurface, nullptr, screenSurface, nullptr );
    SDL_UpdateWindowSurface( window );
}

void Game::process_physics(double dt)
{
    double shoulder_width = 50.0;
    double e = 1.0;
    // move balls
    for (unsigned int i = 0; i < 16; ++i)
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
    double collided[16] = {-1.0};
    double unit_vec[16][2];
    for (unsigned int i = 0; i < 16; ++i)
    {
        for (unsigned int j = i+1; j < 16; ++j)
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
