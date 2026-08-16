#include <SDL3/SDL.h>
#include <math.h>

#define W 320
#define H 200
#define SCALE 3

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Sine Wave Scroll",
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

        int baseY = H / 2;
        float amplitude = 20.0f;
        float freq = 0.08f;

        for (int x = 0; x < W; ++x) {
            int y = (int)(baseY + sinf(x * freq + t) * amplitude);

            SDL_SetRenderDrawColor(ren, 80, 200, 255, 255);
            SDL_FRect r = {
                x * SCALE,
                y * SCALE,
                SCALE,
                SCALE * 3
            };
            SDL_RenderFillRect(ren, &r);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
        t += 0.08f;
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
