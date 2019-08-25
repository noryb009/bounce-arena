#pragma once

#include <SDL2/SDL.h>

#define BULLET_SIZE 10

class bullet {
public:
    double x;
    double y;
    double vx;
    double vy;
    Uint32 c;

    bullet(double x, double y, double vx, double vy, Uint32 c)
        : x(x), y(y), vx(vx), vy(vy), c(c) {
    }

    void move(int minBound, int maxBound) {
        x += vx;
        y += vy;
        if(x < minBound) {
            x += (minBound - x) * 2;
            vx = -vx;
        }
        else if(y < minBound) {
            y += (minBound - y) * 2;
            vy = -vy;
        }

        if(x > maxBound) {
            x -= (x - maxBound) * 2;
            vx = -vx;
        }
        else if(y > maxBound) {
            y -= (y - maxBound) * 2;
            vy = -vy;
        }
    }

    SDL_Rect rect() {
        SDL_Rect r;
        r.x = x-BULLET_SIZE/2;
        r.y = y-BULLET_SIZE/2;
        r.w = BULLET_SIZE;
        r.h = BULLET_SIZE;

        return r;
    }
};
