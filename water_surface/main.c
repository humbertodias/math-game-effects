#include <SDL3/SDL.h>
#include <stdlib.h>
#include <time.h>

#define W 200
#define H 150
#define SCALE 4

float buf1[W][H];
float buf2[W][H];
float damping = 0.99f;

int main(void) {
    srand((unsigned)time(NULL));
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Water Ripple",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);

    int running = 1;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                running = 0;

            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                int x = e.button.x / SCALE;
                int y = e.button.y / SCALE;
                if (x > 1 && x < W-1 && y > 1 && y < H-1)
                    buf1[x][y] = 50.0f; // “gota” na água
            }
        }

        // Simulação
        for (int y = 1; y < H-1; ++y) {
            for (int x = 1; x < W-1; ++x) {
                buf2[x][y] =
                    (buf1[x-1][y] + buf1[x+1][y] +
                     buf1[x][y-1] + buf1[x][y+1]) / 2.0f
                    - buf2[x][y];

                buf2[x][y] *= damping;
            }
        }

        // Troca buffers
        for (int y = 0; y < H; ++y)
            for (int x = 0; x < W; ++x)
                buf1[x][y] = buf2[x][y];

        SDL_SetRenderDrawColor(ren, 10, 20, 40, 255);
        SDL_RenderClear(ren);

        // Render
        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                int c = (int)(128 + buf1[x][y]);
                if (c < 0) c = 0;
                if (c > 255) c = 255;

                SDL_SetRenderDrawColor(ren, 0, c, 200, 255);
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
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
