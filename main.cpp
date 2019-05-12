#include <iostream>
#include <SDL2/SDL.h>
#include <math.h>

using namespace std;

int main()
{
    SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    const int win_posX = 100, win_posY = 100, win_width = 256, win_height = 256;
    
    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("Wave2D", win_posX, win_posY, win_width, win_height, 0);
    
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    cout << "Wave2D window opened\n";

    //Initialize variables
    const double pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;

    const int standard = 1000000;
    const double dt = 1;
    const double c = 0.05;
    double r = 0, g = 0, b = 0;
    int i = 0, j = 0, ia = 0, ib = 0, ja = 0, jb = 0;
    long current = 0, up = 0, down = 0, left = 0, right = 0;
    long diff = 0;
    double f[256][256];
    double v[256][256];

    //Store initial values for f and v
    for (i = 0; i < win_width; i++)
    {
        for (j = 0; j < win_height; j++)
        {
            f[i][j] = 0.99 * sin(2.0 * 2.0 * pi * i / win_width) * sin(3.0 * 2.0 * pi * j / win_width) * standard;
            v[i][j] = 0;
        }
    }

    //Main loop
    while(1)
    {
        //Quit if the user exits the window
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                break;
            }
        }

        //Clear the frame
        SDL_RenderClear(renderer);
        //Update the frame
        for (i = 0; i < win_width; i++)
        {
            for (j = 0; j < win_height; j++)
            {
                //set pixels
                r = 0;
                b = 0;
                if (f[i][j] < 0)
                {
                    r = -255.0 * f[i][j] / standard;
                    b = 0;
                }
                if (f[i][j] > 0)
                {
                    r = 0;
                    b = 255.0 * f[i][j] / standard;
                }

                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }
        //Draw the frame
        SDL_RenderPresent(renderer);

        //memcpy(tmpa, f, sizeof(f));

        //Update f
        for (i = 0; i < win_width; i++)
        {
            for (j = 0; j < win_height; j++)
            {
                f[i][j] += dt * v[i][j];
            }
        }

        //Update v
        for (i = 0; i < win_width; i++)
        {
            for (j = 0; j < win_height; j++)
            {
                ia = i - 1;
                ib = i + 1;
                ja = j - 1;
                jb = j + 1;
                if (i == 0) ia = win_width - 1;
                if (i == win_width-1) ib = 0;
                if (j == 0) ja = win_height - 1;
                if (j == win_width-1) jb = 0;

                current = f[i][j];
                up = f[i][ja];
                down = f[i][jb];
                left = f[ia][j];
                right = f[ib][j];

                diff = up + down + left + right - (current * 4);

                v[i][j] += c * diff;

            }
        }
    }
    cout << "Wave2D window closed" << endl;


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
