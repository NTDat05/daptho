#pragma once
#include "CommonFunc.h"

class Rabbit {
public:
    enum State {
        Normal,
        Hit
    };
    enum Type {
        Yellow=0,
        Red=4,
        Blue=8

    };
    Type type;
    int x, y;
    int currentFrame;
    int frameWidth, frameHeight;
    int frameDuration;
    Uint64 lastFrameTime;
    Uint64 hitStartTime;
    State state;
    SDL_Rect rect;

    Rabbit(SDL_Rect _rect, int _frameDuration,int _type);
       
    void animate();
    void toNextFrame(Uint32 currentTime);
};