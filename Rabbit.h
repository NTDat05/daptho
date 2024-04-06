#pragma once
#include "CommonFunc.h"
class Rabbit {
public:
    enum State {
        Normal,
        Hit
    };

    int x, y;
    int currentFrame;
    int frameWidth, frameHeight;
    int frameDuration;
    Uint32 lastFrameTime;
    State state;
    Uint32 hitStartTime;
    SDL_Rect rect;

    Rabbit(SDL_Rect _rect, int _frameDuration);
       
    void animate();
};