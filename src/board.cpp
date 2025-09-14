#include <board.h>
#include <constants.h>

// constructor
Board::Board()
:
board_colour(0),
shoulder_colour(0),
shoulder_width(50),
radius(25), //radius(15),
board_init(false)
{
    holes[0][0] = shoulder_width;
    holes[0][1] = shoulder_width;

    holes[1][0] = SCREEN_WIDTH/2.0;
    holes[1][1] = shoulder_width;

    holes[2][0] = SCREEN_WIDTH - shoulder_width;
    holes[2][1] = shoulder_width;

    holes[3][0] = shoulder_width;
    holes[3][1] = SCREEN_HEIGHT - shoulder_width;

    holes[4][0] = SCREEN_WIDTH/2.0;
    holes[4][1] = SCREEN_HEIGHT - shoulder_width;

    holes[5][0] = SCREEN_WIDTH  - shoulder_width;
    holes[5][1] = SCREEN_HEIGHT - shoulder_width;

    for (unsigned int i = 0; i < SCREEN_WIDTH; ++i)
        for (unsigned int j = 0; j < SCREEN_HEIGHT; ++j)
            naked_board[i][j] = 0x00000000u;
}

// destructor
Board::~Board()
{

}

void Board::draw_board(Uint32 *pixels, Uint32 board_colour, Uint32 shoulder_colour)
{
    if (!board_init)
    {
        int width  = SCREEN_WIDTH;
        int height = SCREEN_HEIGHT;
        int shoulder_width = 50;
        for (unsigned int x = 0; x < width; ++x)
        {
            bool xShoulder = (x<shoulder_width) || (x>width-shoulder_width);
            for (unsigned int y = 0; y < height; ++y)
            {
                bool yShoulder = (y<shoulder_width) || (y>height-shoulder_width);
                Uint32 new_colour(0);
                if (xShoulder || yShoulder)
                    new_colour = shoulder_colour;
                else
                    new_colour = board_colour;
                pixels[x + y*width] = new_colour;
                naked_board[x][y]   = new_colour;
            }
        }

        for (unsigned int h = 0; h < 6; ++h)
        {
            for (unsigned int x = holes[h][0] - radius; x < holes[h][0] + radius; ++x)
            {
                for (unsigned int y = holes[h][1] - radius; y < holes[h][1] + radius; ++y)
                {
                    double r_2 = (x-holes[h][0])*(x-holes[h][0]) + (y-holes[h][1])*(y-holes[h][1]);
                    if (r_2 <= radius*radius)
                    {
                        pixels[x + y*width] = BLACK;
                        naked_board[x][y]   = BLACK;
                    }
                }
            }
        }
        board_init = true;
    }
    else
    {
        for (unsigned int x = 0; x < SCREEN_WIDTH; ++x)
        {
            for (unsigned int y = 0; y < SCREEN_HEIGHT; ++y)
            {
                pixels[x + y*SCREEN_WIDTH] = naked_board[x][y];
            }
        }
    }
}

void Board::draw_line(Uint32 *pixels, Uint32 line_colour, double line_width, double px, double py, double theta)
{
    bool on_board = true;
    double current_point[2] = {px, py};
    while (on_board)
    {
        if (((current_point[0] < shoulder_width) || (current_point[0] > (SCREEN_WIDTH  - shoulder_width))) ||
            ((current_point[1] < shoulder_width) || (current_point[1] > (SCREEN_HEIGHT - shoulder_width))))
        {
            on_board = false;
        }
        pixels[int(current_point[0]) + int(current_point[1])*SCREEN_WIDTH] = line_colour;
        for (unsigned int i=0; i<line_width; ++i)
        {
            pixels[int(current_point[0])+i + int(current_point[1])*SCREEN_WIDTH ] = line_colour;
            pixels[int(current_point[0])-i + int(current_point[1])*SCREEN_WIDTH ] = line_colour;
            pixels[int(current_point[0]) + (int(current_point[1])+i)*SCREEN_WIDTH ] = line_colour;
            pixels[int(current_point[0]) + (int(current_point[1])-i)*SCREEN_WIDTH ] = line_colour;
//             pixels[int(current_point[0]+double(i)*std::sin(theta)) + int(current_point[1]+double(i)*std::cos(theta))*SCREEN_WIDTH ] = line_colour;
//             pixels[int(current_point[0]-double(i)*std::sin(theta)) + int(current_point[1]-double(i)*std::cos(theta))*SCREEN_WIDTH ] = line_colour;
        }
        current_point[0] += 0.1*std::cos(theta);
        current_point[1] += 0.1*std::sin(theta);
    }
}

void Board::draw_line_segment(Uint32 *pixels, Uint32 line_colour, double line_width, double px, double py, double theta, double length, double start_offset)
{
    bool on_board = true;
    double current_point[2] = {px + start_offset*std::cos(theta), py + start_offset*std::sin(theta)};
    double current_length = 0.0;
    while (on_board)
    {
        pixels[int(current_point[0]) + int(current_point[1])*SCREEN_WIDTH] = line_colour;
        for (unsigned int i=0; i<line_width; ++i)
        {
            pixels[int(current_point[0])+i + int(current_point[1])*SCREEN_WIDTH ] = line_colour;
            pixels[int(current_point[0])-i + int(current_point[1])*SCREEN_WIDTH ] = line_colour;
            pixels[int(current_point[0]) + (int(current_point[1])+i)*SCREEN_WIDTH ] = line_colour;
            pixels[int(current_point[0]) + (int(current_point[1])-i)*SCREEN_WIDTH ] = line_colour;
        }
        current_point[0] += 0.1*std::cos(theta);
        current_point[1] += 0.1*std::sin(theta);
        current_length += 0.1;

        if (((current_point[0] < line_width) || (current_point[0] > SCREEN_WIDTH -  line_width)) ||
            ((current_point[1] < line_width) || (current_point[1] > SCREEN_HEIGHT - line_width)) ||
            (current_length > length))
        {
            on_board = false;
        }
    }
}

int Board::ball_in_hole(double px, double py)
{
    int sink_hole = -1;
    for (unsigned int h = 0; h < 6; ++h)
    {
        double dx = px - holes[h][0];
        double dy = py - holes[h][1];
        // if the position is closer to any hole than the radius then it's in the hole
        if ((dx*dx + dy*dy) < radius*radius)
            sink_hole = h;
    }
    return sink_hole;
}
