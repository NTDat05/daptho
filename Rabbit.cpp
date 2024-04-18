#include "Rabbit.h"
Rabbit::Rabbit(SDL_Rect _rect, int _frameDuration) : rect(_rect), frameDuration(_frameDuration) {
	currentFrame = 0;
	lastFrameTime = SDL_GetTicks();
	state = Normal;
	hitStartTime = 0;
	rect = _rect;
	frameHeight = _rect.h;
	frameWidth = _rect.w;
	y = _rect.y;
	x = _rect.x;
}
void Rabbit::animate() {
	Uint32 currentTime = SDL_GetTicks();
	Uint32 elapsedTime = currentTime - lastFrameTime;

	if (state == Normal) {
		if (currentFrame == 3 || currentFrame == 4) {
			if (elapsedTime >= duration * 15) {
				toNextFrame(currentTime);
			}
		}
		else if (elapsedTime >= duration) {
			toNextFrame(currentTime);
		}
	}
	else {
		
		if (currentFrame == 0 || currentFrame == 1) {
			if (elapsedTime >= frameDuration * 10) {
				toNextFrame(currentTime);
			}
		}
		else if (elapsedTime >= frameDuration) {
			toNextFrame(currentTime);
		}
	}
}
void Rabbit::toNextFrame(Uint32 currentTime) {
	lastFrameTime = currentTime;
	currentFrame = (currentFrame + 1) % 10;
}