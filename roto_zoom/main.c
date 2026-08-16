#include <SDL3/SDL.h>
#include <math.h>

#define W 320
#define H 200
#define SCALE 3
#define TEX 128

unsigned char tex[TEX][TEX];

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "RotoZoom",
        W * SCALE, H * SCALE, 0
    );

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);

    // Gera uma textura xadrez procedural
    for (int y = 0; y < TEX; ++y)
        for (int x = 0; x < TEX; ++x)
            tex[x][y] = ((x / 8 + y / 8) & 1) ? 255 : 40;

    int running = 1;
    float a = 0.0f;
    float zoom = 1.0f;

    int cx = W / 2;
    int cy = H / 2;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                running = 0;
        }

        float ca = cosf(a);
        float sa = sinf(a);

        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                float dx = (x - cx) / zoom;
                float dy = (y - cy) / zoom;

                float u =  dx * ca - dy * sa;
                float v =  dx * sa + dy * ca;

                int tx = ((int)u & (TEX - 1));
                int ty = ((int)v & (TEX - 1));

                int c = tex[tx][ty];
                SDL_SetRenderDrawColor(ren, c, c, c, 255);

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

        a += 0.01f;
        zoom = 1.0f + sinf(a * 0.7f) * 0.5f;
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
