#include <SDL3/SDL.h>
#include <stdlib.h>
#include <time.h>

#define W 320
#define H 200
#define SCALE 3
#define STARS 600

typedef struct {
    float x, y, z;
} Star;

Star stars[STARS];
float speed = 0.5f;

void resetStar(Star* s) {
    s->x = (float)(rand() % W - W / 2);
    s->y = (float)(rand() % H - H / 2);
    s->z = (float)(rand() % W + 1);
}

int main(void) {
    srand((unsigned)time(NULL));
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Starfield",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);

    for (int i = 0; i < STARS; ++i)
        resetStar(&stars[i]);

    int running = 1;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
            if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_ESCAPE) running = 0;
                if (e.key.key == SDLK_UP) speed += 0.2f;
                if (e.key.key == SDLK_DOWN && speed > 0.2f) speed -= 0.2f;
            }
        }

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        for (int i = 0; i < STARS; ++i) {
            Star* s = &stars[i];

            s->z -= speed;
            if (s->z <= 0.1f)
                resetStar(s);

            float fov = 120.0f;
            int sx = (int)((s->x / s->z) * fov + W / 2);
            int sy = (int)((s->y / s->z) * fov + H / 2);

            if (sx < 0 || sx >= W || sy < 0 || sy >= H) {
                resetStar(s);
                continue;
            }

            int bright = (int)(255.0f * (1.0f - s->z / W));
            if (bright < 50) bright = 50;
            if (bright > 255) bright = 255;

            SDL_SetRenderDrawColor(ren, bright, bright, bright, 255);
            SDL_FRect r = {
                sx * SCALE,
                sy * SCALE,
                SCALE,
                SCALE
            };
            SDL_RenderFillRect(ren, &r);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
