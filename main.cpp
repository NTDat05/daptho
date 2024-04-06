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
	Rabbit newRabbit(rect,50); // default frame duration
	rabbits.push_back(newRabbit);
}
void showMenu() {
	bool inMenu = true;
	SDL_Event e;

	while (inMenu) {
		while (SDL_PollEvent(&e) != 0) {
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

void render() {
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer,backGroundTexture,NULL,NULL);
	for (auto it = rabbits.begin(); it != rabbits.end(); ) {
		if (it->state == Rabbit::Normal) {
			SDL_Rect srcRect = { it->currentFrame * it->frameWidth, it->frameHeight, it->frameWidth, it->frameHeight };
			SDL_Rect destRect = { it->x, it->y, it->frameWidth, it->frameHeight };
			SDL_RenderCopy(renderer, rabbitTexture, &srcRect, &destRect);
			++it;
		}
		else if (it->state == Rabbit::Hit) {
			SDL_Rect srcRect = { it->currentFrame * it->frameWidth,0, it->frameWidth, it->frameHeight };
			SDL_Rect destRect = { it->x, it->y, it->frameWidth, it->frameHeight };
			SDL_RenderCopy(renderer, rabbitTexture, &srcRect, &destRect);
			if (SDL_GetTicks() - it->hitStartTime >= it->frameDuration * 6) {
				

				it= rabbits.erase(rabbits.begin() + std::distance(rabbits.begin(), it));
			}
			else {
				++it;
			}
		}
	}

	SDL_Rect carrotRect = { mouseX-CARROT_OFFSET, mouseY- CARROT_HEIGHT/2, CARROT_WIDTH, CARROT_HEIGHT };
	SDL_Rect crosshair = { mouseX - 20, mouseY - 20, 40, 40 };
	SDL_RenderCopyEx(renderer, carrotTexture, NULL, &carrotRect, hit?0:45,NULL, SDL_FLIP_NONE);
	SDL_RenderCopy(renderer, crosshairTexture, NULL, &crosshair);

	SDL_RenderPresent(renderer);
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

	bool quit = false;
	SDL_Event e;
	spawnRabbit();
	spawnRabbit();
	Uint32 preTime = SDL_GetTicks();
	Uint32 curTime;
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
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
					if (SDL_PointInRect(&mousePos,&rabbit.rect)&&rabbit.state==Rabbit::Normal) {
						rabbit.state = Rabbit::Hit;
						rabbit.hitStartTime = SDL_GetTicks();
						break;
					}
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP) hit = false;

		}
		curTime = SDL_GetTicks();
		
		if (curTime-preTime>=1000) {
			spawnRabbit();
			preTime = curTime;
		}
		for (auto& rabbit : rabbits) {
			rabbit.animate();
		}

		render();
	}

	close();
	return 0;
}
