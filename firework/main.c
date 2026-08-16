#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define W 800
#define H 600

#define MAX_ROCKETS 5
#define MAX_PARTICLES 500

typedef struct {
    int x;
    int y;
    int vx;
    int vy;
    int target;
    int active;
} Rocket;

typedef struct {
    int x;
    int y;
    int old_x;
    int old_y;

    int vx;
    int vy;
    int gravity;

    int life;
    int active;
    int color;
} Particle;

static Rocket rockets[MAX_ROCKETS];
static Particle particles[MAX_PARTICLES];

static int random_int(int min, int max)
{
    return min + rand() % (max - min + 1);
}

static void set_color(SDL_Renderer *renderer, int color)
{
    Uint8 r = (color >> 16) & 0xff;
    Uint8 g = (color >> 8) & 0xff;
    Uint8 b = color & 0xff;

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

/*
 * =================================
 * CREATE ROCKET
 * =================================
 */

static void create_rocket(void)
{
    int i;
    int direction;

    for (i = 0; i < MAX_ROCKETS; i++) {

        if (!rockets[i].active) {

            rockets[i].x = random_int(80, 720);
            rockets[i].y = 590;

            rockets[i].vy = random_int(7, 11);

            direction = random_int(-4, 4);

            if (direction == 0)
                direction = random_int(-2, 2);

            rockets[i].vx = direction;

            rockets[i].target = random_int(120, 360);

            rockets[i].active = 1;

            break;
        }
    }
}


/*
 * =================================
 * CREATE EXPLOSION
 * =================================
 */

static void create_explosion(int x, int y)
{
    int count;
    int p;

    count = random_int(35, 100);

    for (p = 0; p < count; p++) {

        int index = -1;

        /*
         * Find an unused particle.
         */
        for (int i = 0; i < MAX_PARTICLES; i++) {

            if (!particles[i].active) {
                index = i;
                break;
            }
        }

        if (index < 0)
            continue;

        Particle *particle = &particles[index];

        /*
         * Start position.
         */
        particle->x = x + random_int(-3, 3);
        particle->y = y + random_int(-3, 3);

        particle->old_x = particle->x;
        particle->old_y = particle->y;

        /*
         * Random direction.
         */
        int vx = random_int(-10, 10);
        int vy = random_int(-10, 10);

        int speed = random_int(1, 5);

        particle->vx = vx * speed / 3;
        particle->vy = vy * speed / 3;

        /*
         * Prevent stationary particles.
         */
        if (particle->vx == 0) {

            particle->vx = random_int(-2, 2);

            if (particle->vx == 0)
                particle->vx = 1;
        }

        if (particle->vy == 0) {

            particle->vy = random_int(-2, 2);

            if (particle->vy == 0)
                particle->vy = -1;
        }

        /*
         * Additional impulse.
         */
        if (random_int(0, 3) == 0) {

            particle->vx += random_int(-3, 3);
            particle->vy += random_int(-3, 3);
        }

        /*
         * Individual gravity.
         */
        particle->gravity = random_int(1, 3);

        /*
         * Lifetime.
         */
        particle->life = random_int(20, 80);

        particle->active = 1;

        /*
         * Color.
         */
        particle->color = random_int(0, 5);
    }
}


/*
 * =================================
 * PARTICLE COLOR
 * =================================
 */

static void set_particle_color(
    SDL_Renderer *renderer,
    int color)
{
    switch (color) {

        case 0:
            set_color(renderer, 16753920);
            break;

        case 1:
            set_color(renderer, 65535);
            break;

        case 2:
            set_color(renderer, 16711935);
            break;

        case 3:
            set_color(renderer, 16776960);
            break;

        case 4:
            set_color(renderer, 16777215);
            break;

        case 5:
            set_color(renderer, 65280);
            break;
    }
}


/*
 * =================================
 * MAIN
 * =================================
 */

int main(void)
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init: %s", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "Fireworks",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        W,
        H,
        0
    );

    if (!window) {
        SDL_Log("SDL_CreateWindow: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!renderer) {
        SDL_Log("SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    srand((unsigned int)time(NULL));

    /*
     * =================================
     * INITIALIZE
     * =================================
     */

    for (int i = 0; i < MAX_ROCKETS; i++)
        rockets[i].active = 0;

    for (int i = 0; i < MAX_PARTICLES; i++)
        particles[i].active = 0;

    /*
     * =================================
     * INTRO
     * =================================
     *
     */

    Uint32 intro_start = SDL_GetTicks();

    int intro_skipped = 0;

    while (SDL_GetTicks() - intro_start < 2000) {

        SDL_Event event;

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT) {

                intro_skipped = 1;
            }

            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE) {

                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }
        }

        if (intro_skipped)
            break;

        SDL_SetRenderDrawColor(
            renderer,
            0,
            0,
            0,
            255
        );

        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    /*
     * Wait for mouse release if intro
     * was skipped with a click.
     */
    if (intro_skipped) {

        int mouse_down = 1;

        while (mouse_down) {

            SDL_Event event;

            while (SDL_PollEvent(&event)) {

                if (event.type == SDL_QUIT) {
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return 0;
                }
            }

            Uint32 buttons = SDL_GetMouseState(NULL, NULL);

            mouse_down =
                (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;

            SDL_Delay(1);
        }
    }

    /*
     * =================================
     * MAIN VARIABLES
     * =================================
     */

    int timer = 0;
    int next_rocket = random_int(10, 40);

    int mouse_was_down = 0;

    int running = 1;

    /*
     * =================================
     * MAIN LOOP
     * =================================
     */

    Uint32 previous_frame = SDL_GetTicks();

    while (running) {

        /*
         * =================================
         * EVENTS
         * =================================
         */

        SDL_Event event;

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT)
                running = 0;

            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE) {

                running = 0;
            }
        }

        /*
         * =================================
         * BACKGROUND
         * =================================
         */

        SDL_SetRenderDrawColor(
            renderer,
            0,
            0,
            0,
            255
        );

        SDL_RenderClear(renderer);

        /*
         * =================================
         * MOUSE FIREWORK
         * =================================
         */

        Uint32 mouse_buttons =
            SDL_GetMouseState(NULL, NULL);

        int mouse_down =
            (mouse_buttons &
             SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;

        if (mouse_down && !mouse_was_down) {

            int mouse_x;
            int mouse_y;

            SDL_GetMouseState(
                &mouse_x,
                &mouse_y
            );

            create_explosion(
                mouse_x,
                mouse_y
            );
        }

        mouse_was_down = mouse_down;

        /*
         * =================================
         * AUTOMATIC ROCKETS
         * =================================
         */

        timer++;

        if (timer >= next_rocket) {

            create_rocket();

            timer = 0;

            next_rocket =
                random_int(8, 45);
        }

        /*
         * =================================
         * ROCKETS
         * =================================
         */

        for (int i = 0; i < MAX_ROCKETS; i++) {

            Rocket *rocket = &rockets[i];

            if (!rocket->active)
                continue;

            /*
             * Rocket head.
             */
            set_color(renderer, 16777215);

            SDL_Rect rect = {
                rocket->x - 2,
                rocket->y - 2,
                5,
                5
            };

            SDL_RenderFillRect(
                renderer,
                &rect
            );

            /*
             * Rocket trail.
             */
            set_color(renderer, 8421504);

            SDL_RenderDrawLine(
                renderer,
                rocket->x,
                rocket->y,
                rocket->x -
                    rocket->vx * 3,
                rocket->y +
                    rocket->vy * 3
            );

            /*
             * Movement.
             */
            rocket->x += rocket->vx;
            rocket->y -= rocket->vy;

            /*
             * Occasionally change direction.
             */
            if (random_int(0, 20) == 0) {

                rocket->vx +=
                    random_int(-1, 1);
            }

            /*
             * Limit horizontal speed.
             */
            if (rocket->vx > 5)
                rocket->vx = 5;

            if (rocket->vx < -5)
                rocket->vx = -5;

            /*
             * Explosion.
             */
            if (rocket->y <= rocket->target) {

                create_explosion(
                    rocket->x,
                    rocket->y
                );

                rocket->active = 0;
            }

            /*
             * Outside screen.
             */
            if (rocket->x < 0 ||
                rocket->x > W) {

                rocket->active = 0;
            }
        }

        /*
         * =================================
         * PARTICLES
         * =================================
         */

        for (int i = 0; i < MAX_PARTICLES; i++) {

            Particle *particle = &particles[i];

            if (!particle->active)
                continue;

            /*
             * Previous position.
             */
            particle->old_x =
                particle->x;

            particle->old_y =
                particle->y;

            /*
             * Movement.
             */
            particle->x +=
                particle->vx;

            particle->y +=
                particle->vy;

            /*
             * Gravity.
             */
            particle->vy +=
                particle->gravity;

            /*
             * Color.
             */
            set_particle_color(
                renderer,
                particle->color
            );

            /*
             * Trail.
             */
            SDL_RenderDrawLine(
                renderer,
                particle->old_x,
                particle->old_y,
                particle->x,
                particle->y
            );

            /*
             * Particle head.
             */
            SDL_Rect rect = {
                particle->x - 1,
                particle->y - 1,
                3,
                3
            };

            SDL_RenderFillRect(
                renderer,
                &rect
            );

            /*
             * Lifetime.
             */
            particle->life--;

            if (particle->life <= 0)
                particle->active = 0;

            /*
             * Outside screen.
             */
            if (particle->y > H ||
                particle->x < 0 ||
                particle->x > W) {

                particle->active = 0;
            }
        }

        /*
         * =================================
         * PRESENT
         * =================================
         */

        SDL_RenderPresent(renderer);

        /*
         * =================================
         * ~60 FPS
         * =================================
         */

        Uint32 now = SDL_GetTicks();
        Uint32 elapsed = now - previous_frame;

        if (elapsed < 16)
            SDL_Delay(16 - elapsed);

        previous_frame = SDL_GetTicks();
    }

    /*
     * =================================
     * CLEANUP
     * =================================
     */

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
