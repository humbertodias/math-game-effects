#include <SDL3/SDL.h>
#include <math.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 200
#define FPS 60

// Função que gera um batimento cardíaco simples em função do tempo local
float ecg_wave(float t) {
    // Onda P: pequena e larga
    float p = 5.0f * expf(-powf(t - 0.1f, 2) / 0.002f);

    // Complexo QRS: alto e estreito
    float qrs = -15.0f * expf(-powf(t - 0.25f, 2) / 0.0005f)
                + 50.0f * expf(-powf(t - 0.27f, 2) / 0.0002f)
                - 15.0f * expf(-powf(t - 0.29f, 2) / 0.0005f);

    // Onda T: média e lenta
    float t_wave = 10.0f * expf(-powf(t - 0.4f, 2) / 0.005f);

    return p + qrs + t_wave;
}

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "ECG Realista",
        WINDOW_WIDTH, WINDOW_HEIGHT, 0
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    bool running = true;
    SDL_Event event;

    float t_offset = 0.0f; // tempo global para animação
    float beat_length = 2.0f; // duração de um batimento em segundos

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
        }

        // Limpa tela
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        for (int x = 0; x < WINDOW_WIDTH; x++) {
            // Tempo relativo ao batimento
            float t = fmodf(t_offset + x * (beat_length / WINDOW_WIDTH), beat_length);
            float y = WINDOW_HEIGHT / 2 - ecg_wave(t);
            SDL_RenderPoint(renderer, x, (int)y);
        }

        t_offset += 0.02f; // velocidade de rolagem
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / FPS);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
