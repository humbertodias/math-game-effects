#include <SDL3/SDL.h>
#include <math.h>
#include <stdlib.h>

#define W 320
#define H 200
#define SCALE 3

float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

float fade(float t) {
    return t * t * (3.0f - 2.0f * t); // suavização
}

// ruído base em uma grade
float grid[64][64];

float valueNoise(float x, float y) {
    int x0 = (int)x;
    int y0 = (int)y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float sx = fade(x - x0);
    float sy = fade(y - y0);

    float n0 = lerp(grid[x0 & 63][y0 & 63], grid[x1 & 63][y0 & 63], sx);
    float n1 = lerp(grid[x0 & 63][y1 & 63], grid[x1 & 63][y1 & 63], sx);

    return lerp(n0, n1, sy);
}

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Value Noise ",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);

    // inicializa a grade com valores aleatórios
    for (int y = 0; y < 64; ++y)
        for (int x = 0; x < 64; ++x)
            grid[x][y] = (float)rand() / (float)RAND_MAX;

    int running = 1;
    float t = 0.0f;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                running = 0;
        }

        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                float nx = x * 0.05f;
                float ny = y * 0.05f;

                // soma de octaves (fractal noise)
                float v = 0.0f;
                float amp = 1.0f;
                float freq = 1.0f;

                for (int i = 0; i < 4; ++i) {
                    v += valueNoise(nx * freq + t, ny * freq) * amp;
                    amp *= 0.5f;
                    freq *= 2.0f;
                }

                int c = (int)(v * 255.0f);
                if (c > 255) c = 255;

                SDL_SetRenderDrawColor(ren, c, c, c + 40, 255);

                SDL_FRect r = {
                    x * SCALE,
                    y * SCALE,
                    SCALE,
                    SCALE
                };
                SDL_RenderFillRect(ren, &r);
            }
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
        t += 0.01f;
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
