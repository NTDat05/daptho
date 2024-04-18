#include "Rabbit.h"

int mouseX, mouseY;
std::vector<Rabbit> rabbits;
bool hit = false;

void close() {
	SDL_DestroyTexture(rabbitTexture);
	SDL_DestroyTexture(carrotTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

void spawnRabbit() {
	int x, y;
	SDL_Rect rect = {};
	int time = 0;
	do {
		
		x = rand() % (SCREEN_WIDTH - USAGI_SIZE);
		y = rand() % (SCREEN_HEIGHT - USAGI_SIZE);
		bool allow = true;
		rect = { x,y,USAGI_SIZE,USAGI_SIZE };
		for (auto r : BAN_ZONE) 
		{
			if (checkOverlap(r, rect)) allow = false;
		}
		bool overlap = false;
		for (const auto& rabbit : rabbits) {
			if (checkOverlap(rect,rabbit.rect)) {
				overlap = true;
				break;
			}
		}
		if (!overlap&&allow) {
			break;
		}
	} while (true);
	Rabbit newRabbit(rect,70); // default frame duration
	rabbits.push_back(newRabbit);
}
void render() {
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, backGroundTexture, NULL, NULL);
	for (auto it = rabbits.begin(); it != rabbits.end(); ) {
		if (it->state == Rabbit::Normal) {
			SDL_Rect srcRect = { it->currentFrame * it->frameWidth, it->frameHeight, it->frameWidth, it->frameHeight };
			SDL_Rect destRect = { it->x, it->y, it->frameWidth, it->frameHeight };
			SDL_RenderCopy(renderer, rabbitTexture, &srcRect, &destRect);
			//if(SDL_GetTicks() - it->hitStartTime >= it->frameDuration * 12) it = rabbits.erase(rabbits.begin() + std::distance(rabbits.begin(), it));
			it++;
		}
		else if (it->state == Rabbit::Hit) {
			SDL_Rect srcRect = { it->currentFrame * it->frameWidth,0, it->frameWidth, it->frameHeight };
			SDL_Rect destRect = { it->x, it->y, it->frameWidth, it->frameHeight };
			SDL_RenderCopy(renderer, rabbitTexture, &srcRect, &destRect);
			if (/*SDL_GetTicks() - it->hitStartTime >= it->frameDuration * 6*/it->currentFrame == 6) {


				it = rabbits.erase(rabbits.begin() + std::distance(rabbits.begin(), it));
			}
			else {
				it++;
			}
		}
	}

	SDL_Rect carrotRect = { mouseX - CARROT_OFFSET, mouseY - CARROT_HEIGHT / 2, CARROT_WIDTH, CARROT_HEIGHT };
	SDL_Rect crosshair = { mouseX - 20, mouseY - 20, 40, 40 };
	SDL_RenderCopyEx(renderer, carrotTexture, NULL, &carrotRect, hit ? 0 : 45, NULL, SDL_FLIP_NONE);
	SDL_RenderCopy(renderer, crosshairTexture, NULL, &crosshair);

	SDL_RenderPresent(renderer);
}
void showMenu() {
	bool inMenu = true;
	SDL_Event e;

	while (inMenu) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				inMenu = false;
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_1:
					// Start the game
					inMenu = false;
					break;
				case SDLK_2:
					// Show high scores
					break;
				case SDLK_3:
					// Exit the game
					SDL_Quit();
					exit(0);
				}
			}
		}

		// Render the menu
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}
}
void play() {
	bool quit = false;
	SDL_Event e;
	spawnRabbit();
	spawnRabbit();
	Uint32 preTime = SDL_GetTicks();
	Uint32 curTime;
	
	while (!quit&&rabbits.size()<= MAX_USAGI-5) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEMOTION) {
				SDL_GetMouseState(&mouseX, &mouseY);
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				hit = true;
				SDL_Point mousePos;
				SDL_GetMouseState(&mousePos.x, &mousePos.y);
				hit = true;
				for (auto& rabbit : rabbits) {
					if (SDL_PointInRect(&mousePos, &rabbit.rect) && rabbit.state == Rabbit::Normal) {
						rabbit.state = Rabbit::Hit;
						rabbit.currentFrame = 0;
						rabbit.hitStartTime = SDL_GetTicks();
						break;
					}
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP) hit = false;

		}
		curTime = SDL_GetTicks();

		if (curTime - preTime >= 1000) {
			spawnRabbit();
			preTime = curTime;
			duration -= 3;
		}
		for (auto& rabbit : rabbits) {
			rabbit.animate();
		}

		render();
	}
}
bool gameOver() {
	//reset game stats
	rabbits.clear();
	duration = 70;

	bool replay = false;
	bool inGameOver = true;
	SDL_Event e;
	while (inGameOver) {
		while (SDL_PollEvent(&e) ) {
			if (e.type == SDL_QUIT) {
				inGameOver = false;
			}
			else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_r) {
					replay = true;
					return replay;
				}
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					replay = false;
					return replay;
				}
				
			}
		}

		SDL_RenderClear(renderer);
		SDL_Color textColor = { 255, 0, 0 };
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Game Over", textColor);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		SDL_Rect textRect;
		textRect.x = (SCREEN_WIDTH - textSurface->w) / 2;
		textRect.y = (SCREEN_HEIGHT - textSurface->h) / 2;
		textRect.w = textSurface->w;
		textRect.h = textSurface->h;

		SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

		SDL_RenderPresent(renderer);

	}

	return replay;
}
int main(int argc, char* args[]) {
	
	if (!init()) {
		std::cerr << "Failed to initialize!" << std::endl;
		return -1;
	}

	if (!loadMedia()) {
		std::cerr << "Failed to load media!" << std::endl;
		return -1;
	}
	showMenu();
	srand(time(NULL));
	do {
		play();
	} while (gameOver());
	close();
	return 0;
}
