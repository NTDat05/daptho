
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
#include <sstream>
#include<string>
#include <fstream>
extern SDL_Window* window ;
extern SDL_Renderer* renderer ;
extern SDL_Texture* rabbitTexture ;
extern SDL_Texture* backGroundTexture;
extern SDL_Texture* carrotTexture;
extern SDL_Texture* crosshairTexture;
extern SDL_Texture* menuBackGround;
extern SDL_Texture* playButton;
extern SDL_Texture* quitButton;
extern SDL_Texture* gameOverBackground;
extern SDL_Texture* ruleBackGround;
extern SDL_Texture* sBoard;
extern SDL_Texture* tBoard;

extern Mix_Chunk* hitSfx;
extern Mix_Music* gameOverSfx;
extern Mix_Music* gameStartSfx;
extern Mix_Music* newMaxScore;
extern TTF_Font* font;
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
const SDL_Color textColor = { 255, 255, 255 };
const SDL_Color blackText = { 0, 0, 0 };
const SDL_Rect ScoreBoard = { 0,-60,200,200 };
const SDL_Rect TimeBoard = { SCREEN_WIDTH-200,-60,200,200 };
const Uint64 ONE_MINUTE = 60000;
const int buttonWidth = 212;
const int buttonHeight = 96;
const SDL_Rect playButtonRect = {SCREEN_WIDTH/2-buttonWidth/2,SCREEN_HEIGHT/2-buttonHeight/2,buttonWidth,buttonHeight};
const SDL_Rect quitButtonRect = {SCREEN_WIDTH/2-buttonWidth/2,2*SCREEN_HEIGHT/3-buttonHeight/2,buttonWidth,buttonHeight};

bool loadMedia();
bool init();
bool checkOverlap(SDL_Rect rect1, SDL_Rect rect2);
void close();
#endif // COMMON_FUNCTION_H_
