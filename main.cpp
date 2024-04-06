#include "CommonFunc.h"

int hammerX, hammerY;
class Rabbit{
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

    Rabbit(int _x, int _y, int _frameWidth, int _frameHeight, int _frameDuration)
        : x(_x), y(_y), frameWidth(_frameWidth), frameHeight(_frameHeight), frameDuration(_frameDuration) {
        currentFrame = 0;
        lastFrameTime = SDL_GetTicks();
        state = Normal;
        hitStartTime = 0;
    }

    void animate() {
        Uint32 currentTime = SDL_GetTicks();
        if (state == Hit && currentTime - hitStartTime >= frameDuration) {
            // Hit animation duration expired, change state back to Normal
            state = Normal;
        }

        if (currentTime - lastFrameTime >= frameDuration) {
            if (state == Hit) {
                // Change animation for hit state
                currentFrame = 6; // Example: Set current frame to 6 for hit animation
            } else {
                // Default animation for normal state
                currentFrame = (currentFrame + 1) % 10;
            }
            lastFrameTime = currentTime;
        }
    }
};
std::vector<Rabbit> rabbits;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Whack-a-Rabbit", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    return true;
}

bool loadMedia() {
    SDL_Surface* rabbitSurface = IMG_Load("rabbit_spritesheet.png");
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

    SDL_Surface* hammerSurface = IMG_Load("hammer.png");
    if (hammerSurface == nullptr) {
        std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    hammerTexture = SDL_CreateTextureFromSurface(renderer, hammerSurface);
    SDL_FreeSurface(hammerSurface);
    if (hammerTexture == nullptr) {
        std::cerr << "Unable to create texture from image! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void close() {
    SDL_DestroyTexture(rabbitTexture);
    SDL_DestroyTexture(hammerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void spawnRabbit() {
    int x, y;
    do {
        x = rand() % (SCREEN_WIDTH - 110);
        y = rand() % (SCREEN_HEIGHT - 110);
        bool overlap = false;
        for (const auto& rabbit : rabbits) {
            if (x + 110 >= rabbit.x && x <= rabbit.x + 110 && y + 110 >= rabbit.y && y <= rabbit.y + 110) {
                overlap = true;
                break;
            }
        }
        if (!overlap) {
            break;
        }
    } while (true);
    Rabbit newRabbit(x, y, 110, 110, 100); // default frame duration
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
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    for (auto it = rabbits.begin(); it != rabbits.end(); ) {
        if (it->state == Rabbit::Normal) {
            SDL_Rect srcRect = { it->currentFrame * it->frameWidth, 0, it->frameWidth, it->frameHeight };
            SDL_Rect destRect = { it->x, it->y, it->frameWidth, it->frameHeight };
            SDL_RenderCopy(renderer, rabbitTexture, &srcRect, &destRect);
            ++it;
        }
        else if (it->state == Rabbit::Hit) {
            // Render hit animation
            SDL_Rect srcRect = { it->currentFrame * it->frameWidth, it->frameHeight, it->frameWidth, it->frameHeight };
            SDL_Rect destRect = { it->x, it->y, it->frameWidth, it->frameHeight };
            SDL_RenderCopy(renderer, rabbitTexture, &srcRect, &destRect);
            if (SDL_GetTicks() - it->hitStartTime >= it->frameDuration) {
                // Hit animation duration expired, remove the rabbit
                it = rabbits.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    SDL_Rect hammerRect = { hammerX, hammerY, 50, 50 };
    SDL_RenderCopy(renderer, hammerTexture, nullptr, &hammerRect);

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
    srand(time(nullptr));

    bool quit = false;
    SDL_Event e;

    spawnRabbit();
    spawnRabbit();
    spawnRabbit();
    spawnRabbit();
    spawnRabbit();
    spawnRabbit();
    spawnRabbit();
    spawnRabbit();
    spawnRabbit();
    spawnRabbit();

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_MOUSEMOTION) {
                SDL_GetMouseState(&hammerX, &hammerY);
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                for (auto& rabbit : rabbits) {
                    if (x >= rabbit.x && x <= rabbit.x + 110 && y >= rabbit.y && y <= rabbit.y + 110) {
                        // Mark rabbit as hit and start hit animation
                        rabbit.state = Rabbit::Hit;
                        rabbit.hitStartTime = SDL_GetTicks();
                        break;
                    }
                }
            }
            
        }

        for (auto& rabbit : rabbits) {
            rabbit.animate();
        }

        render();
    }

    close();
    return 0;
}
