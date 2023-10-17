#include <SDL2/SDL.h>
#include <stdio.h>
#include <chrono>

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;
const char * TITLE = "bouncy ball";

class Ball {
private:
  unsigned int radius_;
  unsigned int posX_;
  unsigned int posY_;
  unsigned int colorRed_;
  unsigned int colorGreen_;
  unsigned int colorBlue_;
public:
  Ball(unsigned int radius = 10, unsigned int posX = SCREEN_WIDTH/2, unsigned int posY = SCREEN_HEIGHT/2,
    unsigned int colorRed = 255, unsigned int colorGreen = 255, unsigned int colorBlue = 255) : 
    radius_{radius}, posX_{posX}, posY_{posY}, colorRed_{colorRed}, colorGreen_{colorGreen}, colorBlue_{colorBlue} {};
  unsigned int get_radius() {return radius_;};
  unsigned int get_posX() {return posX_;};
  unsigned int get_posY() {return posY_;};
  void set_radius(unsigned int radius) {radius_ = radius;};
  void set_posX(unsigned int posX) {posX_ = posX;};
  void set_posY(unsigned int posY) {posY_ = posY;};
};

bool SDL_init();
void SDL_end();
void fill_circle(unsigned int radius, unsigned int posX, unsigned int posY);

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

void fill_circle(unsigned int radius, unsigned int posX, unsigned int posY) {
  unsigned int beginX = posX > radius ? posX - radius : 0;
  unsigned int beginY = posY > radius ? posY - radius : 0; // these two lines are only in case the circle is going into negative coordinates [up or left outside screen]
  for (unsigned int i = beginX; i < posX + radius; ++i) {
    for (unsigned int j = beginY; j < posY + radius; ++j) {
      unsigned int distX = i > posX ? i-posX : posX - i;
      unsigned int distY = j > posY ? j-posY : posY - j;
      if (distX*distX + distY*distY <= radius*radius) {
        SDL_RenderDrawPoint(gRenderer, i, j);
      }
    }
  }
}


int main(int argc, char* argv[]) {
  if (!SDL_init()) {
    printf("Could not initialize SDL stuff, :(\n");
    return 1;
  }
  Ball ball(20, SCREEN_WIDTH/2, SCREEN_HEIGHT/10, 0xFF, 0xFF, 0xFF);
  SDL_Rect floor = {0, SCREEN_HEIGHT-10, SCREEN_WIDTH, SCREEN_HEIGHT};
  bool running = true;
  std::chrono::duration<double> deltaT = std::chrono::seconds(0);
  while (running) {
    auto startTime = std::chrono::high_resolution_clock::now();
    //parse events
    //update world state
    //redraw screen
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0x10, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    fill_circle(ball.get_radius(), ball.get_posX(), ball.get_posY());
    SDL_RenderFillRect(gRenderer, &floor);
    SDL_RenderPresent(gRenderer);
    auto endTime = std::chrono::high_resolution_clock::now();
    deltaT = endTime - startTime;
  }
  SDL_end();
}
