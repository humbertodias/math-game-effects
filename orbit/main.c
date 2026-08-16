#include <SDL3/SDL.h>
#include <math.h>

#define W 320
#define H 200
#define SCALE 3
#define ORBS 6

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Orbit ",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);

    float t = 0.0f;
    int cx = W / 2;
    int cy = H / 2;

    int running = 1;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                running = 0;
        }

        SDL_SetRenderDrawColor(ren, 10, 10, 20, 255);
        SDL_RenderClear(ren);

        // centro
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_FRect center = {
            cx * SCALE - SCALE * 2,
            cy * SCALE - SCALE * 2,
            SCALE * 4,
            SCALE * 4
        };
        SDL_RenderFillRect(ren, &center);

        for (int i = 0; i < ORBS; ++i) {
            float angle = t + i * (2.0f * 3.1415926f / ORBS);
            float radius = 30.0f + i * 18.0f;

            float x = cx + cosf(angle) * radius;
            float y = cy + sinf(angle) * radius;

            SDL_SetRenderDrawColor(ren, 80 + i * 25, 150, 255 - i * 30, 255);

            SDL_FRect r = {
                (int)(x * SCALE) - SCALE * 2,
                (int)(y * SCALE) - SCALE * 2,
                SCALE * 4,
                SCALE * 4
            };
            SDL_RenderFillRect(ren, &r);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
        t += 0.02f;
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
