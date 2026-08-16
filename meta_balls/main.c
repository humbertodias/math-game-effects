#include <SDL3/SDL.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define W 320
#define H 200
#define SCALE 3
#define BALLS 4

typedef struct {
    float x, y;
    float vx, vy;
    float r;
} Ball;

static uint32_t pixels[W * H];

uint32_t pack(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 16) | (g << 8) | b;
}

int main(void) {
    srand((unsigned)time(NULL));
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Metaballs",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);
    SDL_Texture* tex = SDL_CreateTexture(
        ren, SDL_PIXELFORMAT_XRGB8888,
        SDL_TEXTUREACCESS_STREAMING, W, H
    );

    Ball b[BALLS];
    for (int i = 0; i < BALLS; ++i) {
        b[i].x = rand() % W;
        b[i].y = rand() % H;
        b[i].vx = (rand() % 200 - 100) / 100.0f;
        b[i].vy = (rand() % 200 - 100) / 100.0f;
        b[i].r  = 30 + rand() % 30;
    }

    int running = 1;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                running = 0;
        }

        // Atualiza bolas
        for (int i = 0; i < BALLS; ++i) {
            b[i].x += b[i].vx;
            b[i].y += b[i].vy;

            if (b[i].x < 0 || b[i].x >= W) b[i].vx = -b[i].vx;
            if (b[i].y < 0 || b[i].y >= H) b[i].vy = -b[i].vy;
        }

        // Campo metaball
        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                float v = 0.0f;
                for (int i = 0; i < BALLS; ++i) {
                    float dx = x - b[i].x;
                    float dy = y - b[i].y;
                    float d2 = dx * dx + dy * dy + 1.0f;
                    v += (b[i].r * b[i].r) / d2;
                }

                if (v > 1.0f) {
                    int c = (int)(v * 40);
                    if (c > 255) c = 255;
                    pixels[y * W + x] = pack(c, 60, 180);
                } else {
                    pixels[y * W + x] = pack(10, 10, 20);
                }
            }
        }

        SDL_UpdateTexture(tex, NULL, pixels, W * sizeof(uint32_t));
        SDL_RenderClear(ren);

        SDL_FRect dst = {0, 0, W * SCALE, H * SCALE};
        SDL_RenderTexture(ren, tex, NULL, &dst);

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
