#include <SDL3/SDL.h>
#include <math.h>
#include <stdint.h>

#define W 320
#define H 200
#define SCALE 3

static uint32_t basePixels[W * H];
static uint32_t outPixels[W * H];

uint32_t pack(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 16) | (g << 8) | b;
}

int clamp(int v, int a, int b) {
    if (v < a) return a;
    if (v > b) return b;
    return v;
}

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Heat Distortion",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);
    SDL_Texture* tex = SDL_CreateTexture(
        ren, SDL_PIXELFORMAT_XRGB8888,
        SDL_TEXTUREACCESS_STREAMING, W, H
    );

    // Cena base: um gradiente simples
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            uint8_t r = (uint8_t)(x * 255 / W);
            uint8_t g = (uint8_t)(y * 255 / H);
            uint8_t b = 80;
            basePixels[y * W + x] = pack(r, g, b);
        }
    }

    int running = 1;
    float t = 0.0f;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                running = 0;
        }

        t += 0.05f;

        // Heat distortion
        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                float ox = sinf(y * 0.05f + t) * 3.0f;
                float oy = cosf(x * 0.04f + t) * 2.0f;

                int sx = clamp((int)(x + ox), 0, W - 1);
                int sy = clamp((int)(y + oy), 0, H - 1);

                outPixels[y * W + x] = basePixels[sy * W + sx];
            }
        }

        SDL_UpdateTexture(tex, NULL, outPixels, W * sizeof(uint32_t));
        SDL_RenderClear(ren);

        SDL_FRect dst = { 0, 0, W * SCALE, H * SCALE };
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
