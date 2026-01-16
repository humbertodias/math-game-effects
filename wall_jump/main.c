#include <SDL2/SDL.h>

#define W 800
#define H 600

typedef struct {
    float x, y;
    float vx, vy;
    int onGround;
    int onWall;     // -1 = esquerda, 1 = direita, 0 = nenhuma
    int lockFrames; // trava controle após wall jump
} Player;

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *win = SDL_CreateWindow(
        "Wall Jump - MegaMan Style",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        W, H, 0);

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    Player p = {100, 300, 0, 0, 0, 0, 0};

    const float gravity = 0.5f;
    const float jump = -10.0f;
    const float wallJumpY = -11.0f;
    const float wallJumpX = 9.0f;
    const float wallSlideSpeed = 2.0f;
    const float speed = 3.0f;

    int running = 1;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
        }

        const Uint8 *k = SDL_GetKeyboardState(NULL);

        // Controle horizontal (se não estiver travado pelo wall jump)
        if (p.lockFrames == 0) {
            if (k[SDL_SCANCODE_LEFT])       p.vx = -speed;
            else if (k[SDL_SCANCODE_RIGHT]) p.vx =  speed;
            else                            p.vx =  0;
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
                p.lockFrames = 6; // mantém o impulso por alguns frames
            }
        }

        // Gravidade
        p.vy += gravity;

        // Integração
        p.x += p.vx;
        p.y += p.vy;

        // Reset estados
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

        // Render
        SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 80, 80, 80, 255);
        SDL_Rect leftWall  = {0, 0, 50, H};
        SDL_Rect rightWall = {W - 50, 0, 50, H};
        SDL_RenderFillRect(ren, &leftWall);
        SDL_RenderFillRect(ren, &rightWall);

        SDL_SetRenderDrawColor(ren, 0, 200, 255, 255);
        SDL_Rect r = {(int)p.x, (int)p.y, 20, 40};
        SDL_RenderFillRect(ren, &r);

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    SDL_Quit();
    return 0;
}
