
#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_  

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_Texture* rabbitTexture = nullptr;
static SDL_Texture* hammerTexture = nullptr;

// Screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;


#endif // COMMON_FUNCTION_H_
