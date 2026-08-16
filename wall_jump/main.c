#include <SDL3/SDL.h>
#include <stdbool.h>

#define W 800
#define H 600
#define MAX_BULLETS 32

typedef struct {
    float x, y;
    float vx, vy;
    int onGround;
    int onWall;     // -1 = esquerda, 1 = direita, 0 = nenhuma
    int lockFrames;
    int facing;     // -1 esquerda, 1 direita
} Player;

typedef struct {
    float x, y;
    float vx;
    int w, h;
    int active;
} Bullet;

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *win = SDL_CreateWindow(
        "MegaMan - Wall Jump + Charged Shot",
        W, H, 0
    );

    SDL_Renderer *ren = SDL_CreateRenderer(win, NULL);

    Player p = {100, 300, 0, 0, 0, 0, 0, 1};
    Bullet bullets[MAX_BULLETS] = {0};

    const float gravity = 0.5f;
    const float jump = -10.0f;
    const float wallJumpY = -11.0f;
    const float wallJumpX = 9.0f;
    const float wallSlideSpeed = 2.0f;
    const float speed = 3.0f;
    const float bulletSpeed = 8.0f;

    int running = 1;

    int chargeFrames = 0;
    int wasShooting = 0;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
        }

        const bool *k = SDL_GetKeyboardState(NULL);

        // Movimento horizontal
        if (p.lockFrames == 0) {
            if (k[SDL_SCANCODE_LEFT]) {
                p.vx = -speed;
                p.facing = -1;
            } else if (k[SDL_SCANCODE_RIGHT]) {
                p.vx = speed;
                p.facing = 1;
            } else {
                p.vx = 0;
            }
        } else {
            p.lockFrames--;
        }

        // Jump
        if (k[SDL_SCANCODE_Z]) {
            if (p.onGround) {
                p.vy = jump;
                p.onGround = 0;
            }
            else if (p.onWall != 0) {
                p.vy = wallJumpY;
                p.vx = -p.onWall * wallJumpX;
                p.onWall = 0;
                p.lockFrames = 6;
            }
        }

        // Charged Shot (X)
        if (k[SDL_SCANCODE_X]) {
            chargeFrames++;
            wasShooting = 1;
        } else if (wasShooting) {
            // Soltou o botão → dispara
            int charged = chargeFrames > 30; // ~0.5s

            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!bullets[i].active) {
                    bullets[i].active = 1;
                    bullets[i].x = p.x + (p.facing == 1 ? 20 : -8);
                    bullets[i].y = p.y + 15;
                    bullets[i].vx = p.facing * (charged ? 12.0f : bulletSpeed);
                    bullets[i].w = charged ? 14 : 6;
                    bullets[i].h = charged ? 10 : 4;
                    break;
                }
            }

            chargeFrames = 0;
            wasShooting = 0;
        }

        // Física do player
        p.vy += gravity;
        p.x += p.vx;
        p.y += p.vy;

        p.onGround = 0;
        p.onWall = 0;

        // Chão
        if (p.y > H - 40) {
            p.y = H - 40;
            p.vy = 0;
            p.onGround = 1;
        }

        // Parede esquerda
        if (p.x < 50) {
            p.x = 50;
            if (p.vy > 0) {
                p.onWall = -1;
                if (p.vy > wallSlideSpeed)
                    p.vy = wallSlideSpeed;
            }
        }

        // Parede direita
        if (p.x > W - 70) {
            p.x = W - 70;
            if (p.vy > 0) {
                p.onWall = 1;
                if (p.vy > wallSlideSpeed)
                    p.vy = wallSlideSpeed;
            }
        }

        // Atualiza balas
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                bullets[i].x += bullets[i].vx;
                if (bullets[i].x < 0 || bullets[i].x > W)
                    bullets[i].active = 0;
            }
        }

        // Render
        SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 80, 80, 80, 255);
        SDL_FRect leftWall  = {0, 0, 50, H};
        SDL_FRect rightWall = {W - 50, 0, 50, H};
        SDL_RenderFillRect(ren, &leftWall);
        SDL_RenderFillRect(ren, &rightWall);

        // Player
        SDL_SetRenderDrawColor(ren, 0, 200, 255, 255);
        SDL_FRect r = {(int)p.x, (int)p.y, 20, 40};
        SDL_RenderFillRect(ren, &r);

        // Bullets
        SDL_SetRenderDrawColor(ren, 255, 240, 120, 255);
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                SDL_FRect b = {
                    (int)bullets[i].x,
                    (int)bullets[i].y,
                    bullets[i].w,
                    bullets[i].h
                };
                SDL_RenderFillRect(ren, &b);
            }
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    SDL_Quit();
    return 0;
}
