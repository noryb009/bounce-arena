#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>

#include "bullet.h"
#include "ship.h"

#define WIDTH 600
#define HEIGHT 600

bool quit;
bool running;
SDL_Surface *screen;
SDL_Window *window;

using shipvec = std::vector<std::unique_ptr<ship>>;
using bulletvec = std::vector<std::unique_ptr<bullet>>;

shipvec ships;
bulletvec bullets;

void init() {
    quit = false;
    running = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "error: " << SDL_GetError() << "\n";
        exit(1);
    }

    window = SDL_CreateWindow(
        "An SDL2 window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        0
    );

    if (window == NULL) {
        std::cerr << "Could not create window: " << SDL_GetError() << "\n";
        exit(1);
    }

    screen = SDL_GetWindowSurface(window);
}

void cleanup() {
    SDL_DestroyWindow(window);

    SDL_Quit();
}

bool did_collide(std::unique_ptr<ship> &s) {
    if(s->sides(0, 600)) {
        return true;
    }

    for(auto &b : bullets) {
        if(pow(b->x - s->x, 2) + pow(b->y - s->y, 2) < 20) {
            return true;
        }
    }
    return false;
}

void loop() {
    int maxBullet = 100;
    static int nextBullet = maxBullet;

    for(auto &b : bullets) {
        b->move(0, 600);
    }

    for(auto &s : ships) {
        s->move();
        if(nextBullet == 0)
            bullets.push_back(s->spawn_bullet(0, 600, SDL_MapRGB(screen->format, 0, 0, 255)));
    }

    nextBullet--;
    if(nextBullet == -1)
        nextBullet = maxBullet;

    ships.erase(std::remove_if(ships.begin(), ships.end(), did_collide), ships.end());
    if(ships.size() <= 1) {
        running = false;
    }
}

void render() {
    SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));

    for(auto &s : ships) {
        s->move();
        SDL_Rect r = s->rect();
        SDL_FillRect(screen, &r, s->c);
    }
    for(auto &b : bullets) {
        b->move(0, 600);
        SDL_Rect r = b->rect();
        SDL_FillRect(screen, &r, b->c);
    }

    SDL_UpdateWindowSurface(window);
}

void onEvent(SDL_Event *e) {
    if(e->type == SDL_QUIT) {
        quit = true;
    }
}

void handleEvents() {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        onEvent(&e);
    }
}

int main() {
    int np;

    std::cout << "Number of players:" << std::endl;
    std::cin >> np;

    init();

    while(!quit) {
        switch(np) {
          case 6:
            ships.push_back(std::make_unique<ship>(380, 10, M_PI, SDL_MapRGB(screen->format, 255, 0, 255), SDL_SCANCODE_MINUS, SDL_SCANCODE_EQUALS));
            [[fallthrough]];
          case 5:
            ships.push_back(std::make_unique<ship>(10, 380, M_PI, SDL_MapRGB(screen->format, 255, 0, 255), SDL_SCANCODE_J, SDL_SCANCODE_K));
            [[fallthrough]];
          case 4:
            ships.push_back(std::make_unique<ship>(580, 10, M_PI, SDL_MapRGB(screen->format, 255, 0, 255), SDL_SCANCODE_I, SDL_SCANCODE_P));
            [[fallthrough]];
          case 3:
            ships.push_back(std::make_unique<ship>(10, 580, M_PI, SDL_MapRGB(screen->format, 255, 0, 255), SDL_SCANCODE_V, SDL_SCANCODE_N));
            [[fallthrough]];
          case 2:
            ships.push_back(std::make_unique<ship>(580, 580, M_PI, SDL_MapRGB(screen->format, 0, 255, 0), SDL_SCANCODE_A, SDL_SCANCODE_D));
            ships.push_back(std::make_unique<ship>(10, 10, 0, SDL_MapRGB(screen->format, 255, 0, 0), SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT));
            break;
          default:
            std::cerr << "Only 2-6 players is supported." << std::endl;
        }

        while(!quit && running) {
            handleEvents();
            loop();
            render();
            SDL_Delay(10);
        }

        ships.clear();
        bullets.clear();
        running = true;
        if(!quit) {
            SDL_Delay(1000);
        }
    }
    cleanup();

    return 0;
}
