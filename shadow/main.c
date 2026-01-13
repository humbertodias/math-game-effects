#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>

#define W 640
#define H 240
#define FPS 60

// Desenha um círculo preenchido (bruto, mas suficiente)
void draw_filled_circle(SDL_Renderer* r, int cx, int cy, int radius) {
    for (int y = -radius; y <= radius; y++) {
        int dx = (int)sqrt(radius * radius - y * y);
        SDL_RenderDrawLine(r, cx - dx, cy + y, cx + dx, cy + y);
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow("shadow",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, 0);
    SDL_Renderer* r = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    bool run = true;
    SDL_Event e;

    float t = 0.0f;
    int ground_y = H - 40;

    while (run) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) run = false;
        }

        // Fundo
        SDL_SetRenderDrawColor(r, 20, 20, 30, 255);
        SDL_RenderClear(r);

        // Altura simulada (pulo)
        float height = (sinf(t) * 0.5f + 0.5f); // 0..1
        float y_offset = height * 60.0f;

        int x = W / 2;
        int y = ground_y - (int)y_offset;

        // --- SOMBRA (16-bits style) ---
        float shadow_scale = 1.0f - height * 0.6f;
        int shadow_w = (int)(24 * shadow_scale);
        int shadow_h = (int)(8  * shadow_scale);

        SDL_SetRenderDrawColor(r, 0, 0, 0, 120);
        for (int i = -shadow_h; i <= shadow_h; i++) {
            int dx = (int)(shadow_w * sqrtf(1.0f - (float)(i*i)/(shadow_h*shadow_h)));
            SDL_RenderDrawLine(r, x - dx, ground_y + i, x + dx, ground_y + i);
        }

        // --- OBJETO ---
        SDL_SetRenderDrawColor(r, 80, 200, 255, 255);
        draw_filled_circle(r, x, y, 12);

        SDL_RenderPresent(r);

        t += 0.05f;
        SDL_Delay(1000 / FPS);
    }

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
