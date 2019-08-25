#pragma once
#include <SDL2/SDL.h>
#include "bullet.h"

#define SHIP_SIZE 10

class ship {
public:
    double x;
    double y;
    double deg;
    double v;
    Uint32 c;
    SDL_Scancode l;
    SDL_Scancode r;

    ship(double x, double y, double deg, Uint32 c, SDL_Scancode l, SDL_Scancode r)
        : x(x), y(y), deg(deg), v(2), c(c), l(l), r(r) {
    }

    void move() {
        x += pow2_sign(cos(deg)) * v;
        y += pow2_sign(sin(deg)) * v;

        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if(keys[l]) {
            deg -= 0.05;
            if(deg < 0)
                deg += 2*M_PI;
        }
        if(keys[r]) {
            deg += 0.05;
            if(deg > 2*M_PI)
                deg -= 2*M_PI;
        }
    }

    double pow2_sign(double x) {
        return x;
        double t = x*x;
        if(x < 0)
            return -t;
        else
            return t;
    }

    bullet *spawn_bullet(double min_bound, double max_bound, Uint32 c) {
        bullet *b = new bullet(x, y, pow2_sign(cos(deg)) * v * 2, pow2_sign(sin(deg)) * v * 2, c);
        for(int i = 0; i < 3; ++i)
            b->move(min_bound, max_bound);
        return b;
    }

    bool sides(double min_bound, double max_bound) {
        return x < min_bound || x > max_bound
               || y < min_bound || y > max_bound;
   }

    SDL_Rect rect() {
        SDL_Rect r;
        r.x = x-SHIP_SIZE/2;
        r.y = y-SHIP_SIZE/2;
        r.w = SHIP_SIZE;
        r.h = SHIP_SIZE;

        return r;
    }
};
