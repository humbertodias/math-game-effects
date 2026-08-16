#include <SDL3/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define W 200
#define H 120
#define SCALE 4
#define PALETTE_SIZE 36

int fireIntensity = PALETTE_SIZE - 1; // 0..35

// Paleta de fogo (Doom Fire)
SDL_Color palette[PALETTE_SIZE] = {
    {0,0,0,255},{7,7,7,255},{31,7,7,255},{47,15,7,255},
    {71,15,7,255},{87,23,7,255},{103,31,7,255},{119,31,7,255},
    {143,39,7,255},{159,47,7,255},{175,63,7,255},{191,71,7,255},
    {199,71,7,255},{223,79,7,255},{223,87,7,255},{223,87,7,255},
    {215,95,7,255},{215,95,7,255},{215,103,15,255},{207,111,15,255},
    {207,119,15,255},{207,127,15,255},{207,135,23,255},{199,135,23,255},
    {199,143,23,255},{199,151,31,255},{191,159,31,255},{191,159,31,255},
    {191,167,39,255},{191,167,39,255},{191,175,47,255},{183,175,47,255},
    {183,183,47,255},{183,183,55,255},{207,207,111,255},{255,255,255,255}
};

void updateFire(uint8_t* fire) {
    for (int y = 1; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            int src = y * W + x;
            int decay = rand() & 3; // 0..3
            int dstX = x - decay + 1;

            if (dstX < 0) dstX = 0;
            if (dstX >= W) dstX = W - 1;

            int dst = (y - 1) * W + dstX;
            uint8_t v = fire[src];
            fire[dst] = (v > decay) ? (v - decay) : 0;
        }
    }
}

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    srand((unsigned)time(NULL));

    if (!SDL_Init(SDL_INIT_VIDEO))
        return 1;

    SDL_Window* win = SDL_CreateWindow(
        "Fire Math",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);

    static uint8_t fire[W * H]; // agora local e estático

    int running = 1;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                running = 0;

            if (e.type == SDL_EVENT_KEY_DOWN) {
                switch (e.key.key) {
                    case SDLK_ESCAPE:
                        running = 0;
                        break;

                    case SDLK_UP:
                        fireIntensity++;
                        if (fireIntensity >= PALETTE_SIZE)
                            fireIntensity = PALETTE_SIZE - 1;
                        break;

                    case SDLK_DOWN:
                        fireIntensity--;
                        if (fireIntensity < 0)
                            fireIntensity = 0;
                        break;

                    case SDLK_RIGHT:
                        fireIntensity += 3;
                        if (fireIntensity >= PALETTE_SIZE)
                            fireIntensity = PALETTE_SIZE - 1;
                        break;

                    case SDLK_LEFT:
                        fireIntensity -= 3;
                        if (fireIntensity < 0)
                            fireIntensity = 0;
                        break;
                }
            }
        }

        // Base do fogo controlada pela intensidade
        for (int x = 0; x < W; ++x)
            fire[(H - 1) * W + x] = (uint8_t)fireIntensity;

        updateFire(fire);

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                SDL_Color c = palette[fire[y * W + x]];
                SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, 255);
                SDL_FRect r = { x * SCALE, y * SCALE, SCALE, SCALE };
                SDL_RenderFillRect(ren, &r);
            }
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
