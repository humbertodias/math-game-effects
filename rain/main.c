#include <SDL3/SDL.h>
#include <stdlib.h>
#include <time.h>

#define W 320
#define H 200
#define SCALE 3
#define DROPS 400

typedef struct {
    float x, y;
    float speed;
    int len;
} Drop;

Drop rain[DROPS];

void resetDrop(int i) {
    rain[i].x = (float)(rand() % W);
    rain[i].y = (float)(-(rand() % H));
    rain[i].speed = 1.5f + (rand() % 30) / 10.0f;
    rain[i].len = 4 + rand() % 6;
}

int main(void) {
    srand((unsigned)time(NULL));
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Rain ",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);

    for (int i = 0; i < DROPS; ++i)
        resetDrop(i);

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

        SDL_SetRenderDrawColor(ren, 120, 160, 255, 255);

        for (int i = 0; i < DROPS; ++i) {
            rain[i].y += rain[i].speed;

            if (rain[i].y > H)
                resetDrop(i);

            SDL_RenderLine(
                ren,
                (int)(rain[i].x * SCALE),
                (int)(rain[i].y * SCALE),
                (int)(rain[i].x * SCALE),
                (int)((rain[i].y + rain[i].len) * SCALE)
            );
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
