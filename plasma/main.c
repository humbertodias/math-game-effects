#include <SDL3/SDL.h>
#include <math.h>

#define W 320
#define H 200
#define SCALE 3

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Plasma",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);

    int running = 1;
    float t = 0.0f;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                running = 0;
        }

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                float v =
                    sinf(x * 0.04f + t) +
                    sinf(y * 0.05f + t) +
                    sinf((x + y) * 0.03f + t);

                int c = (int)((v + 3.0f) / 6.0f * 255.0f);
                if (c < 0) c = 0;
                if (c > 255) c = 255;

                SDL_SetRenderDrawColor(ren, c, 128, 255 - c, 255);

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
        t += 0.05f;
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
