#include "Rabbit.h"

int mouseX, mouseY;
std::vector<Rabbit> rabbits;
bool hit = false;
bool quit = false;
int maxScore;
int currentScore;
int playedTimes=0;
Uint64 startTime;
Uint64 remainTime=0;
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
			if (checkOverlap(rect, rabbit.rect)) {
				overlap = true;
				break;
			}
		}
		if (!overlap && allow) {
			break;
		}
	} while (true);
	
	Rabbit newRabbit(rect, 70, rand() % 3); // default frame duration
	rabbits.push_back(newRabbit);
}
void render() {
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, backGroundTexture, NULL, NULL);
	for (auto it = rabbits.begin(); it != rabbits.end(); ) {
		if (it->state == Rabbit::Normal) {
			
			SDL_Rect srcRect = { it->currentFrame * it->frameWidth, (static_cast<int>(it->type ) / 2+1)*it->frameHeight, it->frameWidth, it->frameHeight};
			SDL_Rect destRect = { it->x, it->y, it->frameWidth, it->frameHeight };
			SDL_RenderCopy(renderer, rabbitTexture, &srcRect, &destRect);
			if (it->currentFrame == 8) {
				it = rabbits.erase(rabbits.begin() + std::distance(rabbits.begin(), it));
			}
			else it++;
		}
		else if (it->state == Rabbit::Hit) {
			SDL_Rect srcRect = { it->currentFrame * it->frameWidth,(static_cast<int>(it->type) / 2) * it->frameHeight, it->frameWidth, it->frameHeight };
			SDL_Rect destRect = { it->x, it->y, it->frameWidth, it->frameHeight };
			SDL_RenderCopy(renderer, rabbitTexture, &srcRect, &destRect);
			if (it->currentFrame == 6) {


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

	SDL_Surface* score = TTF_RenderText_Solid(font,std::to_string(currentScore).c_str(), blackText);
	SDL_Surface* time = TTF_RenderText_Solid(font, std::to_string(60 - remainTime / 1000).c_str(), blackText);
	SDL_Texture* scoreText = SDL_CreateTextureFromSurface(renderer,score);
	SDL_Texture* timeText = SDL_CreateTextureFromSurface(renderer,time);
	SDL_Rect scoreR= { 100 - score->w / 2,85,score->w,score->h };
	SDL_Rect timeR = { SCREEN_WIDTH - 100-time->w/2,85,time->w,time->h };
	SDL_RenderCopy(renderer, sBoard, NULL, &ScoreBoard);
	SDL_RenderCopy(renderer, tBoard, NULL, &TimeBoard);
	SDL_RenderCopy(renderer, scoreText, NULL,&scoreR );

	SDL_RenderCopy(renderer, timeText, NULL,&timeR );
	SDL_RenderPresent(renderer);
}
void showMenu() {
	Mix_PlayMusic(gameStartSfx, 0);

	std::ifstream maxS("maxScore.txt");
	maxS >> maxScore;
	maxS.close();
	std::stringstream ss;
	ss << "max score: ";
	ss << maxScore;
	

	SDL_Surface* mSS = TTF_RenderText_Solid(font, ss.str().c_str(), textColor);
	SDL_Texture* mST = SDL_CreateTextureFromSurface(renderer, mSS);

	SDL_Rect maxScoreRect;
	maxScoreRect.x = (SCREEN_WIDTH - mSS->w) / 3 + 150;
	maxScoreRect.y = 4 * SCREEN_HEIGHT / 5;
	maxScoreRect.w = mSS->w;
	maxScoreRect.h = mSS->h;


	SDL_Event e;
	SDL_Point pos;
	bool inMenu = true;
	while (!quit && inMenu) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&pos.x, &pos.y);

				if (SDL_PointInRect(&pos, &playButtonRect)) {
					inMenu = false;
				}
				if (SDL_PointInRect(&pos, &quitButtonRect)) {
					quit = true;
				}
			}
		}


		// Render the menu
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, menuBackGround, NULL, NULL);
		SDL_RenderCopy(renderer, playButton, NULL, &playButtonRect);
		SDL_RenderCopy(renderer, quitButton, NULL, &quitButtonRect);
		SDL_RenderCopy(renderer, mST, NULL, &maxScoreRect);
		SDL_RenderPresent(renderer);
	}
}
void showRule() {

	SDL_Event e;
	SDL_Point pos;
	bool inRule = true;
	while (!quit && inRule) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			if (e.type == SDL_KEYDOWN&& e.key.keysym.sym == SDLK_ESCAPE) {
				 inRule = false;
			}
		}


		// Render the menu
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, ruleBackGround, NULL, NULL);

		SDL_RenderPresent(renderer);
	}
}
void play() {
	currentScore = 0;
	if (Mix_PlayingMusic() == 1) {
		Mix_PauseMusic();
	}
	SDL_Event e;
	spawnRabbit();
	SDL_ShowCursor(SDL_FALSE);
	Uint32 preTime = SDL_GetTicks();
	Uint32 curTime;
	startTime = SDL_GetTicks64();
	SDL_GetMouseState(&mouseX, &mouseY);
	while (!quit && remainTime<=ONE_MINUTE) {
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
						Mix_PlayChannel(-1, hitSfx, 0);
						rabbit.state = Rabbit::Hit;
						rabbit.currentFrame = 0;
						rabbit.hitStartTime = SDL_GetTicks();
						switch (rabbit.type) {
						case Rabbit::Red:
							currentScore -= 40;
							break;
						case Rabbit::Yellow:
							currentScore += 10;
							break;
						case Rabbit::Blue:
							currentScore += 20;
							break;

						default:
							break;

						}
						break;
					}
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP) hit = false;

		}
		curTime = SDL_GetTicks();

		if (curTime - preTime >= 500 * (duration / 70)) {
			spawnRabbit();
			preTime = curTime;
			duration = std::max(45.0f, float(duration - 1.0f));
		}
		for (auto& rabbit : rabbits) {
			rabbit.animate();
		}
		
		remainTime = SDL_GetTicks64() - startTime;
		
		render();
	}
}
bool gameOver() {

	//Play the music
	maxScore = std::max(maxScore, currentScore);
	if (maxScore==currentScore) {
		Mix_PlayMusic(newMaxScore,0);
	}
	else {
		Mix_PlayMusic(gameOverSfx, 0);
	}
	//reset game stats
	rabbits.clear();
	duration = 70;
	remainTime = 0;
	playedTimes++;

	std::ofstream maxS("maxScore.txt");
	maxS << maxScore;
	maxS.close();

	std::stringstream ss;
	ss << "max score: ";
	ss << maxScore;
	std::stringstream ss2;
	ss2 << "your score: ";
	ss2 << currentScore;

	
	SDL_Surface* mSS = TTF_RenderText_Solid(font, ss.str().c_str(), textColor);
	SDL_Texture* mST = SDL_CreateTextureFromSurface(renderer, mSS);
	SDL_Surface* cSS = TTF_RenderText_Solid(font, ss2.str().c_str(), textColor);
	SDL_Texture* cST = SDL_CreateTextureFromSurface(renderer, cSS);

	SDL_Rect maxScoreRect;
	maxScoreRect.x = (SCREEN_WIDTH - mSS->w) / 3 + 150;
	maxScoreRect.y = 1 * SCREEN_HEIGHT / 5;
	maxScoreRect.w = mSS->w;
	maxScoreRect.h = mSS->h;
	SDL_Rect currentScoreRect;
	currentScoreRect.x = maxScoreRect.x;
	currentScoreRect.y = maxScoreRect.y + maxScoreRect.h;
	currentScoreRect.w = cSS->w;
	currentScoreRect.h = cSS->h;
	std::stringstream ss3;
	ss3 << "played: ";
	ss3 << playedTimes;
	ss3 << " times";
	SDL_Surface* pTS = TTF_RenderText_Solid(font, ss3.str().c_str(), textColor);
	SDL_Texture* pTT = SDL_CreateTextureFromSurface(renderer, pTS);
	SDL_Rect playedRect;

	playedRect.x = currentScoreRect.x;
	playedRect.y = currentScoreRect.y + currentScoreRect.h;
	playedRect.w = pTS->w;
	playedRect.h = pTS->h;



	bool replay = false;

	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
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
		SDL_RenderCopy(renderer, gameOverBackground, NULL, NULL);
		SDL_RenderCopy(renderer, mST, NULL, &maxScoreRect);
		SDL_RenderCopy(renderer, cST, NULL, &currentScoreRect);
		SDL_RenderCopy(renderer, pTT, NULL, &playedRect);

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
	showRule();
	srand(time(NULL));
	do {
		play();
	} while (gameOver());
	close();
	return 0;
}
