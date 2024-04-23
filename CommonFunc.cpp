#include "CommonFunc.h"
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* rabbitTexture = NULL;
SDL_Texture* carrotTexture = NULL;
SDL_Texture* backGroundTexture = NULL;
SDL_Texture* crosshairTexture = NULL;
SDL_Texture* menuBackGround = NULL;
SDL_Texture* playButton = NULL;
SDL_Texture* quitButton = NULL;
SDL_Texture* gameOverBackground = NULL;
SDL_Texture* ruleBackGround = NULL;
TTF_Font* font = NULL;
Mix_Chunk* hitSfx = NULL;
Mix_Music* gameOverSfx = NULL;
Mix_Music* gameStartSfx = NULL;
Mix_Music* newMaxScore = NULL;
extern SDL_Texture* sBoard =NULL;
extern SDL_Texture* tBoard =NULL;

float duration = 70;
bool loadMedia() {
	SDL_Surface* rabbitSurface = IMG_Load("imgs/rabbit_spritesheetv2.png");
	if (rabbitSurface == nullptr) {
		std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	rabbitTexture = SDL_CreateTextureFromSurface(renderer, rabbitSurface);
	SDL_FreeSurface(rabbitSurface);
	if (rabbitTexture == nullptr) {
		std::cerr << "Unable to create texture from image! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_Surface* carrotSurface = IMG_Load("imgs/carrot.png");
	if (carrotSurface == nullptr) {
		std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	carrotTexture = SDL_CreateTextureFromSurface(renderer, carrotSurface);
	SDL_FreeSurface(carrotSurface);
	if (carrotTexture == nullptr) {
		std::cerr << "Unable to create texture from image! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_Surface* bgSurface = IMG_Load("imgs/bg.png");
	if (bgSurface == nullptr) {
		std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	backGroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
	SDL_FreeSurface(bgSurface);
	if (backGroundTexture == nullptr) {
		std::cerr << "Unable to create texture from image! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_Surface* crSurface = IMG_Load("imgs/crosshair.png");
	if (crSurface == nullptr) {
		std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	
	crosshairTexture = SDL_CreateTextureFromSurface(renderer, crSurface);
	SDL_FreeSurface(crSurface);
	if (crosshairTexture == nullptr) {
		std::cerr << "Unable to create texture from image! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	menuBackGround = IMG_LoadTexture(renderer,"imgs/menu.png");
	playButton = IMG_LoadTexture(renderer,"imgs/playButton.png");
	quitButton = IMG_LoadTexture(renderer,"imgs/quitButton.png");
	gameOverBackground = IMG_LoadTexture(renderer,"imgs/gameOver.png");
	ruleBackGround = IMG_LoadTexture(renderer,"imgs/rule.png");
	sBoard = IMG_LoadTexture(renderer, "imgs/Score.png");
	tBoard = IMG_LoadTexture(renderer, "imgs/Time.png");
	font = TTF_OpenFont("font/BLOBBER.ttf", 48);
	if (font == NULL)
	{
		std::cerr << "Failed to load lazy font! SDL_ttf Error: " << TTF_GetError() << std::endl;
		return  false;
	}
	hitSfx = Mix_LoadWAV("sfx/punch.mp3");
	gameStartSfx = Mix_LoadMUS("sfx/lakhrkhunthrrm.mp3");
	gameOverSfx = Mix_LoadMUS("sfx/conan.mp3");
	newMaxScore = Mix_LoadMUS("sfx/applause_2.mp3");
	return true;
}
bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	window = SDL_CreateWindow("Dap Thor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}
	if (TTF_Init() == -1)
	{
		std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
		return  false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	return true;
}

bool checkOverlap(SDL_Rect rect1, SDL_Rect rect2) {
	return  rect1.x < rect2.x + rect2.w &&
		rect1.x + rect1.w > rect2.x &&
		rect1.y < rect2.y + rect2.h &&
		rect1.y + rect1.h > rect2.y;
}
void close() {
	SDL_DestroyTexture(rabbitTexture);
	SDL_DestroyTexture(carrotTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}
