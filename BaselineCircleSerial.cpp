#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
using namespace std;

// Screen dimensions and circle parameters
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int CENTER_X = SCREEN_WIDTH / 2;
const int CENTER_Y = SCREEN_HEIGHT / 2;
const int RADIUS = 200;
const int TERMS = 100;
const double STEPS=0.0006;

int calFact(int num)
{
    if(num<2)
    return 1;
    else
    return num* calFact(num-1);
}
struct Point
{

    int xAxis;
    int yAxis;
};


double conToRadian(double degPoint)
{
    return degPoint * (M_PI) / 180.0;
}

double sinTaylorSeries(double x)
{
    double term = x;
    double sum = x;
    for (int i = 1; i < TERMS; i++) {
        term *= -x * x / ((2 * i) * (2 * i + 1));
        sum += term;
    }
    return sum;

}

double cosTaylorSeries(double x)
{
    double term = 1.0;
    double sum = 1.0;
    for (int i = 1; i < TERMS; i++) {
        term *= -x * x / ((2 * i - 1) * (2 * i));
        sum += term;
    }
    return sum;
}

int main(int argc, char* argv[]) {
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "SDL_Init Error: " << SDL_GetError() <<endl;
        return 1;
    }
    
    SDL_Window* win = SDL_CreateWindow("Circle Drawing", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!win) {
        cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        SDL_DestroyWindow(win);
       cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }
    
    // Compute circle points using the Taylor series for sine and cosine.
         vector<Point> circlePoints;
    auto start = chrono::high_resolution_clock::now();
    for (float t = 0; t <= 360; t+=STEPS) {
        double rad = conToRadian(t);
        double reducedRad = fmod(rad + M_PI, 2 * M_PI) - M_PI;
        double cos_val = cosTaylorSeries(reducedRad);
        double sin_val = sinTaylorSeries(reducedRad);
        int x = static_cast<int>(RADIUS * cos_val + CENTER_X);
        int y = static_cast<int>(RADIUS * sin_val + CENTER_Y);
        circlePoints.push_back({x, y});
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> computationTime = end - start;
    cout << "Circle computation time (serial): " << computationTime.count() << " seconds" << endl;
    
    // Main loop: draw the circle until the window is closed.
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        
        // Clear the screen with a black background.
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        
        // Draw circle points in white.
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        for (const auto &pt : circlePoints) {
            SDL_RenderDrawPoint(ren, pt.xAxis, pt.yAxis);
        }
        
        SDL_RenderPresent(ren);
    }
  
    
    // Cleanup SDL resources.
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    
    return 0;
}
