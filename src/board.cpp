#include <board.h>
#include <constants.h>

// constructor
Board::Board()
:
board_colour(0),
shoulder_colour(0),
shoulder_width(50),
radius(15)
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
}

// destructor
Board::~Board()
{

}

void Board::draw_board(Uint32 *pixels, Uint32 board_colour, Uint32 shoulder_colour)
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
        }
    }

    for (unsigned int h = 0; h < 6; ++h)
    {
        for (unsigned int x = holes[h][0] - radius; x < holes[h][0] + radius; ++x)
        {
            for (unsigned int y = holes[h][1] - radius; y < holes[h][1] + radius; ++y)
            {
                Uint32 new_colour(0);
                double r_2 = (x-holes[h][0])*(x-holes[h][0]) + (y-holes[h][1])*(y-holes[h][1]);
                if (r_2 <= radius*radius)
                    pixels[x + y*width] = new_colour;
            }
        }
    }
}
