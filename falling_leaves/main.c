#include <SDL.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define W 800
#define H 600
#define LEAVES 64
#define PI 3.1415926535f

typedef struct {
    float x, y;
    float speed;
    float phase;
    float rot;
} Leaf;

Leaf leaves[LEAVES];

float frand(float a, float b) {
    return a + (b - a) * (float)rand() / (float)RAND_MAX;
}

void reset_leaf(Leaf* l) {
    l->x = frand(0, W);
    l->y = frand(-H, 0);
    l->speed = frand(30, 80);
    l->phase = frand(0, 2 * PI);
    l->rot = frand(0, 2 * PI);
}

void draw_leaf(SDL_Renderer* r, float x, float y, float rot) {
    float s = 6.0f;

    float cx = cosf(rot);
    float sx = sinf(rot);

    SDL_FPoint p[5] = {
        { x + ( 0*cx - -s*sx), y + ( 0*sx + -s*cx) },
        { x + ( s*cx -  0*sx), y + ( s*sx +  0*cx) },
        { x + ( 0*cx -  s*sx), y + ( 0*sx +  s*cx) },
        { x + (-s*cx -  0*sx), y + (-s*sx +  0*cx) },
        { x + ( 0*cx - -s*sx), y + ( 0*sx + -s*cx) }
    };

    SDL_RenderDrawLinesF(r, p, 5);
}

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Falling Leaves",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        W, H, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    srand((unsigned)time(NULL));

    for (int i = 0; i < LEAVES; ++i)
        reset_leaf(&leaves[i]);

    Uint32 last = SDL_GetTicks();
    int running = 1;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT) running = 0;

        Uint32 now = SDL_GetTicks();
        float dt = (now - last) / 1000.0f;
        last = now;

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 0, 200, 0, 255);

        for (int i = 0; i < LEAVES; ++i) {
            Leaf* l = &leaves[i];

            l->y += l->speed * dt;
            l->phase += dt * 2.0f;
            l->rot += dt;

            float sway = sinf(l->phase) * 40.0f;
            float x = l->x + sway;

            if (l->y > H + 20)
                reset_leaf(l);

            draw_leaf(ren, x, l->y, l->rot);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
