#include <SDL2/SDL.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "bullet.h"
#include "ship.h"
using namespace std;

#define WIDTH 600
#define HEIGHT 600

bool quit;
bool running;
SDL_Surface *screen;
SDL_Window *window;
vector<ship*> ships;
vector<bullet*> bullets;
typedef vector<ship*> shipvec;
typedef shipvec::iterator ship_it;
typedef vector<bullet*> bulletvec;
typedef bulletvec::iterator bullet_it;

void init() {
    quit = false;
    running = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("error: %s\n", SDL_GetError());
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
        printf("Could not create window: %s\n", SDL_GetError());
        exit(1);
    }

    screen = SDL_GetWindowSurface(window);
}

void cleanup() {
    SDL_DestroyWindow(window);

    SDL_Quit();
}

bool did_collide(ship* s) {
    if(s->sides(0, 600)) {
        delete s;
        return true;
    }

    for(bullet_it bit = bullets.begin(); bit != bullets.end(); ++bit) {
        if(pow((*bit)->x - s->x, 2) + pow((*bit)->y - s->y, 2) < 20) {
            delete s;
            return true;
        }
    }
    return false;
}

void loop() {
    int maxBullet = 100;
    static int nextBullet = maxBullet;

    for(bullet_it it = bullets.begin(); it != bullets.end(); ++it) {
        (*it)->move(0, 600);
    }

    for(ship_it it = ships.begin(); it != ships.end(); ++it) {
        ship *s = *it;
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
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

    for(ship_it it = ships.begin(); it != ships.end(); ++it) {
        (*it)->move();
        SDL_Rect r = (*it)->rect();
        SDL_FillRect(screen, &r, (*it)->c);
    }
    for(bullet_it it = bullets.begin(); it != bullets.end(); ++it) {
        (*it)->move(0, 600);
        SDL_Rect r = (*it)->rect();
        SDL_FillRect(screen, &r, (*it)->c);
    }

    SDL_UpdateWindowSurface(window);
}

void onEvent(SDL_Event *e) {
    if(e->type == SDL_QUIT)
        quit = true;
}

void handleEvents() {
    SDL_Event e;
    while(SDL_PollEvent(&e))
        onEvent(&e);
}

int main(int argc, char* argv[]) {
    int np;
    cout << "Number of players:" << endl;
    cin >> np;

    init();

    while(!quit) {
        ships.push_back(new ship(10, 10, 0, SDL_MapRGB(screen->format, 255, 0, 0), SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT));
        if(np > 1)
            ships.push_back(new ship(580, 580, M_PI, SDL_MapRGB(screen->format, 0, 255, 0), SDL_SCANCODE_A, SDL_SCANCODE_D));
        if(np > 2)
            ships.push_back(new ship(10, 580, M_PI, SDL_MapRGB(screen->format, 255, 0, 255), SDL_SCANCODE_V, SDL_SCANCODE_N));
        if(np > 3)
            ships.push_back(new ship(580, 10, M_PI, SDL_MapRGB(screen->format, 255, 0, 255), SDL_SCANCODE_I, SDL_SCANCODE_P));
        if(np > 4)
            ships.push_back(new ship(10, 380, M_PI, SDL_MapRGB(screen->format, 255, 0, 255), SDL_SCANCODE_J, SDL_SCANCODE_K));
        if(np > 5)
            ships.push_back(new ship(380, 10, M_PI, SDL_MapRGB(screen->format, 255, 0, 255), SDL_SCANCODE_MINUS, SDL_SCANCODE_EQUALS));
        while(!quit && running) {
            handleEvents();
            loop();
            render();
            SDL_Delay(10);
        }

        for(ship_it it = ships.begin(); it != ships.end(); ++it) {
            delete *it;
        }
        ships.clear();
        for(bullet_it it = bullets.begin(); it != bullets.end(); ++it) {
            delete *it;
        }
        bullets.clear();
        running = true;
        if(!quit) {
            SDL_Delay(1000);
        }
    }
    cleanup();

    return 0;
}
