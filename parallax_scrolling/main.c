#include <SDL3/SDL.h>
#include <math.h>

#define W 320
#define H 180
#define SCALE 3

typedef struct {
    float offset;
    float speed;
    float amplitude;
    float frequency;
    SDL_Color color;
    int baseY;
} Layer;

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Parallax Scrolling - Improved",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);

    Layer layers[3] = {
        {0.0f, 0.2f, 8.0f, 0.02f, {40, 60, 120, 255}, 70},  // fundo
        {0.0f, 0.5f, 14.0f, 0.04f, {60, 120, 60, 255}, 95}, // meio
        {0.0f, 1.0f, 20.0f, 0.06f, {120, 80, 40, 255}, 120} // frente
    };

    int running = 1;
    float time = 0.0f;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                running = 0;
        }

        time += 0.016f;

        SDL_SetRenderDrawColor(ren, 15, 20, 30, 255);
        SDL_RenderClear(ren);

        for (int i = 0; i < 3; ++i) {
            Layer* L = &layers[i];
            L->offset += L->speed;

            SDL_SetRenderDrawColor(ren, L->color.r, L->color.g, L->color.b, 255);

            for (int x = 0; x < W; ++x) {
                float worldX = x + L->offset;
                float y = sinf(worldX * L->frequency + time) * L->amplitude;
                int py = (int)(L->baseY + y);

                SDL_RenderLine(
                    ren,
                    x * SCALE,
                    py * SCALE,
                    x * SCALE,
                    H * SCALE
                );
            }
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
