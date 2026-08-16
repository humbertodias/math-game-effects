#include <SDL3/SDL.h>

#define W 320
#define H 200
#define SCALE 3

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Bounce ",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);

    float x = W / 2;
    float y = 20;
    float vy = 0.0f;
    float gravity = 0.4f;
    float bounce = 0.85f;

    int ground = H - 20;

    int running = 1;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                running = 0;
        }

        // Física
        y += vy;
        vy += gravity;

        if (y >= ground) {
            y = ground;
            vy = -vy * bounce;
        }

        SDL_SetRenderDrawColor(ren, 20, 20, 30, 255);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 255, 180, 60, 255);
        SDL_FRect r = {
            (int)(x * SCALE) - SCALE * 4,
            (int)(y * SCALE) - SCALE * 4,
            SCALE * 8,
            SCALE * 8
        };
        SDL_RenderFillRect(ren, &r);

        // chão
        SDL_SetRenderDrawColor(ren, 80, 80, 80, 255);
        SDL_RenderLine(
            ren,
            0, ground * SCALE,
            W * SCALE, ground * SCALE
        );

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
