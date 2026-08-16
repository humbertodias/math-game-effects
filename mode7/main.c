#include <SDL3/SDL.h>
#include <math.h>

#define W 320
#define H 200
#define SCALE 3
#define TEX 64

unsigned char tex[TEX][TEX];

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Mode 7 ",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);

    // textura xadrez procedural
    for (int y = 0; y < TEX; ++y)
        for (int x = 0; x < TEX; ++x)
            tex[x][y] = ((x / 8 + y / 8) & 1) ? 220 : 60;

    float camX = 0.0f;
    float camY = 0.0f;
    float camH = 40.0f;   // altura da câmera
    float angle = 0.0f;

    int running = 1;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
            if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_ESCAPE) running = 0;
                if (e.key.key == SDLK_LEFT)  angle -= 0.05f;
                if (e.key.key == SDLK_RIGHT) angle += 0.05f;
                if (e.key.key == SDLK_UP)    camY += 2.0f;
                if (e.key.key == SDLK_DOWN)  camY -= 2.0f;
            }
        }

        SDL_SetRenderDrawColor(ren, 100, 150, 220, 255); // céu
        SDL_RenderClear(ren);

        float ca = cosf(angle);
        float sa = sinf(angle);

        int horizon = H / 3;

        for (int sy = horizon; sy < H; ++sy) {
            float dist = (float)(sy - horizon);
            float scale = camH / dist;

            for (int sx = 0; sx < W; ++sx) {
                float dx = (sx - W / 2) * scale;

                float wx = camX + dx * ca - dist * sa;
                float wy = camY + dx * sa + dist * ca;

                int tx = ((int)wx) & (TEX - 1);
                int ty = ((int)wy) & (TEX - 1);

                int c = tex[tx][ty];
                SDL_SetRenderDrawColor(ren, c, c, c, 255);

                SDL_RenderPoint(
                    ren,
                    sx * SCALE,
                    sy * SCALE
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
