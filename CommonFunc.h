
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

extern SDL_Window* window ;
extern SDL_Renderer* renderer ;
extern SDL_Texture* rabbitTexture ;
extern SDL_Texture* backGroundTexture;
extern SDL_Texture* carrotTexture;
extern SDL_Texture* crosshairTexture;
extern TTF_Font* font;
const int MAX_USAGI=19;
extern float duration;
// Screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int USAGI_SIZE=110;
const int CARROT_WIDTH = 290;
const int CARROT_HEIGHT = 35;
const int CARROT_OFFSET = 40;
const SDL_Rect BAN_ZONE[6] = {
	{0,0,300,170},
	{0,170,190,200},
	{0,370,150,230},
	{960,0,380,170},
	{1020,130,250,200},
	{1090,300,200,230},
};
bool loadMedia();
bool init();
bool checkOverlap(SDL_Rect rect1, SDL_Rect rect2);

#endif // COMMON_FUNCTION_H_
