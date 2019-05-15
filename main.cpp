#include <iostream>
#include <SDL2/SDL.h>
#include <math.h>

using namespace std;

//Global variables
const double pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;

const int win_width = 256;
const int win_height = 256;

const int standard = 1;//Used for floating point normalization
const double dt = 0.01; //Value of time step
const double c = 0.5; //Value for wave propagation - must be 0.5 or highter, apparently

void dx(double f[][win_height], double storage[][win_height])
{
    for (int i = 0; i < win_width; i++)
    {
        int ia = i - 1;
        int ib = i + 1;
        if (i == 0) ia = win_width - 1;
        if (i == win_width-1) ib = 0;

        for (int j = 0; j < win_height; j++)
        {

            double current = f[i][j];
            double left = f[ia][j];
            double right = f[ib][j];

            storage[i][j] = (right - left) * 0.5;
        }
    }
}

void dy(double f[][win_height], double storage[][win_height])
{
    for (int i = 0; i < win_width; i++)
    {
        for (int j = 0; j < win_height; j++)
        {
            int ja = j - 1;
            int jb = j + 1;
            if (j == 0) ja = win_height - 1;
            if (j == win_width-1) jb = 0;

            double current = f[i][j];
            double up = f[i][ja];
            double down = f[i][jb];

            storage[i][j] = (up - down) * 0.5;
        }
    }
}

void divergence(double f[][win_height], double storage[][win_height])
{
    for (int i = 0; i < win_width; i++)
    {
        for (int j = 0; j < win_height; j++)
        {
            int ia = i - 1;
            int ib = i + 1;
            int ja = j - 1;
            int jb = j + 1;
            if (i == 0) ia = win_width - 1;
            if (i == win_width-1) ib = 0;
            if (j == 0) ja = win_height - 1;
            if (j == win_width-1) jb = 0;

            double current = f[i][j];
            double up = f[i][ja];
            double down = f[i][jb];
            double left = f[ia][j];
            double right = f[ib][j];

            storage[i][j] = (right - left + up - down) * 0.5;
        }
    }
}

void laplacian(double f[][win_height], double storage[][win_height])
{
    for (int i = 0; i < win_width; i++)
    {
        for (int j = 0; j < win_height; j++)
        {
            int ia = i - 1;
            int ib = i + 1;
            int ja = j - 1;
            int jb = j + 1;
            if (i == 0) ia = win_width - 1;
            if (i == win_width-1) ib = 0;
            if (j == 0) ja = win_height - 1;
            if (j == win_width-1) jb = 0;

            double current = f[i][j];
            double up = f[i][ja];
            double down = f[i][jb];
            double left = f[ia][j];
            double right = f[ib][j];
            //Add boundary conditions
            /*
            if (i == 0) left = current;
            if (i == win_width-1) right = current;
            if (j == 0) up = current;
            if (j == win_width-1) down = current;
            */
            storage[i][j] = up + down + left + right - (current * 4);
        }
    }
}

int main()
{
    SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    const int win_posX = 100, win_posY = 100;
    
    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("Wave2D", win_posX, win_posY, win_width, win_height, 0);
    
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    cout << "Wave2D window opened\n";

    //Initialize 2D array variables
    double f[win_width][win_height]; //value
    double v[win_width][win_height]; //velocity
    double tmpa[win_width][win_height]; //temporary storage
    double tmpb[win_width][win_height]; //temporary storage
    double d3x[win_width][win_height]; //temporary storage
    double d3y[win_width][win_height]; //temporary storage

    //Store initial values for f and v
    for (int i = 0; i < win_width; i++)
    {
        for (int j = 0; j < win_height; j++)
        {
            f[i][j] = 0.5 * sin(2.0 * pi * i / win_width) * sin(3.0 * pi * j / win_width) * standard;
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
        for (int i = 0; i < win_width; i++)
        {
            for (int j = 0; j < win_height; j++)
            {
                //set pixels
                double r = 0;
                double g = 0;
                double b = 0;
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
        for (int i = 0; i < win_width; i++)
        {
            for (int j = 0; j < win_height; j++)
            {
                f[i][j] += dt * v[i][j];
            }
        }


        //Update v
        /*
        dx(f, tmpa);        
        dx(tmpa, tmpb);
        dx(tmpb, d3x);

        dy(f, tmpa);        
        dy(tmpa, tmpb);
        dy(tmpb, d3y);

        dx(f, tmpa);
        dy(f, tmpb);
        */
        laplacian(f, tmpa);

        for (int i = 0; i < win_width; i++)
        {
            for (int j = 0; j < win_height; j++)
            {
                v[i][j] += c*c*tmpa[i][j] - c*sin(f[i][j]/standard);
            }
        }
    }
    cout << "Wave2D window closed" << endl;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
