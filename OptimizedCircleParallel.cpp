#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <omp.h>

// Screen dimensions and circle parameters
const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 600;
const int CENTER_X      = SCREEN_WIDTH / 2;
const int CENTER_Y      = SCREEN_HEIGHT / 2;
const int RADIUS        = 200;
const int TERMS         = 10;         // Taylor series terms
const float STEP        = 0.0004;    // Step size for angle increment
const int NUM_THREADS   = 8;          // Number of OpenMP threads

// Convert degrees to radians
inline double toRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

// Compute sin(x) using Taylor series with OpenMP SIMD
double taylorSin(double x, int terms = TERMS) {
    double term = x;  // First term: x
    double sum  = term;

    #pragma omp simd reduction(+:sum)
    for (int n = 1; n < terms; n++) {
        term = -term * (x * x) / ((2 * n) * (2 * n + 1));
        sum += term;
    }
    return sum;
}

// Compute cos(x) using Taylor series with OpenMP SIMD
double taylorCos(double x, int terms = TERMS) {
    double term = 1.0;  // First term: 1
    double sum  = term;

    #pragma omp simd reduction(+:sum)
    for (int n = 1; n < terms; n++) {
        term = -term * (x * x) / ((2 * n - 1) * (2 * n));
        sum += term;
    }
    return sum;
}

struct Point {
    int x, y;
};

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Circle Drawing", 100, 100,
                                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!win) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1,
                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        SDL_DestroyWindow(win);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    int num_steps = static_cast<int>(360.0f / STEP);
    std::vector<Point> circlePoints(num_steps);

    auto start_time = std::chrono::high_resolution_clock::now();

    omp_set_num_threads(NUM_THREADS);

    // **Level 1: Parallelize the loop generating circle points**
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < num_steps; i++) {
        float angle_deg = i * STEP;
        double rad = toRadians(angle_deg);

        // **Level 2: Parallelized Taylor series calculations**
        double cos_val, sin_val;
        #pragma omp parallel sections
        {
            #pragma omp section
            cos_val = taylorCos(rad);

            #pragma omp section
            sin_val = taylorSin(rad);
        }

        circlePoints[i].x = static_cast<int>(RADIUS * cos_val + CENTER_X);
        circlePoints[i].y = static_cast<int>(RADIUS * sin_val + CENTER_Y);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> computationTime = end_time - start_time;
    std::cout << "Circle computation time (two-level parallelism): "
              << computationTime.count() << " seconds" << std::endl;

    // SDL rendering loop
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
        }
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        for (const auto& pt : circlePoints) {
            SDL_RenderDrawPoint(ren, pt.x, pt.y);
        }
        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
