#include <SDL3/SDL.h>

#define W 320
#define H 200
#define SCALE 3
#define BANDS 32

SDL_Color palette[BANDS];

void initPalette(void) {
    for (int i = 0; i < BANDS; ++i) {
        palette[i].r = 0;
        palette[i].g = 50 + i * 6;
        palette[i].b = 150 + i * 3;
        palette[i].a = 255;
    }
}

void cyclePalette(void) {
    SDL_Color first = palette[0];
    for (int i = 0; i < BANDS - 1; ++i)
        palette[i] = palette[i + 1];
    palette[BANDS - 1] = first;
}

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Color Cycling ",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);

    initPalette();

    int running = 1;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                running = 0;
        }

        // gira a paleta
        cyclePalette();

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        int bandHeight = H / BANDS;

        for (int i = 0; i < BANDS; ++i) {
            SDL_Color c = palette[i];
            SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, 255);

            SDL_FRect r = {
                0,
                i * bandHeight * SCALE,
                W * SCALE,
                bandHeight * SCALE
            };
            SDL_RenderFillRect(ren, &r);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(60);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
