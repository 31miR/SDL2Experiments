#include <SDL2/SDL.h>
#include <stdio.h>

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;
const char * TITLE = "bouncy ball";

bool SDL_init();
void SDL_end();
bool fill_circle();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool SDL_init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    return false;
  }
  gWindow = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED,
  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (gWindow == NULL) {
    printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    return false;
  }
  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
  if (gRenderer == NULL) {
    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    return false;
  }
  return true;
}

void SDL_end() {
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  gRenderer= NULL;
}


int main(int argc, char* argv[]) {
  if (!SDL_init()) {
    printf("Could not initialize SDL stuff, :(\n");
    return 1;
  }
  bool running = true;
  while (running) {
    //parse events
    //update world state
    //redraw screen
  }
  SDL_end();
}
