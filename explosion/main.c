#include <SDL3/SDL.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define W 320
#define H 200
#define SCALE 3
#define MAX_PARTS 600

typedef struct {
    float x, y;
    float vx, vy;
    float life;
} Particle;

Particle p[MAX_PARTS];

void spawnExplosion(float cx, float cy) {
    for (int i = 0; i < MAX_PARTS; ++i) {
        float ang = ((float)rand() / (float)RAND_MAX) * 2.0f * 3.1415926f;
        float spd = 2.0f + ((float)rand() / (float)RAND_MAX) * 4.0f;

        p[i].x = cx;
        p[i].y = cy;
        p[i].vx = cosf(ang) * spd;
        p[i].vy = sinf(ang) * spd;
        p[i].life = 1.0f;
    }
}

int allDead(void) {
    for (int i = 0; i < MAX_PARTS; ++i)
        if (p[i].life > 0.0f)
            return 0;
    return 1;
}

int main(void) {
    srand((unsigned)time(NULL));
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Explosion Loop Random",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);

    // Primeira explosão em posição aleatória
    spawnExplosion(rand() % W, rand() % H);

    int running = 1;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                running = 0;
        }

        // Quando todas morrerem, nova explosão em ponto aleatório
        if (allDead()) {
            float x = (float)(rand() % W);
            float y = (float)(rand() % H);
            spawnExplosion(x, y);
        }

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        for (int i = 0; i < MAX_PARTS; ++i) {
            if (p[i].life <= 0.0f) continue;

            p[i].x += p[i].vx;
            p[i].y += p[i].vy;
            p[i].vy += 0.05f;      // gravidade
            p[i].life -= 0.01f;   // decaimento

            int alpha = (int)(p[i].life * 255);
            if (alpha < 0) alpha = 0;

            SDL_SetRenderDrawColor(ren, 255, 180, 60, alpha);
            SDL_FRect r = {
                (int)(p[i].x * SCALE),
                (int)(p[i].y * SCALE),
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
