#include <SDL2/SDL.h>
#include <stdio.h>
#include <chrono>

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;
const char * TITLE = "bouncy ball";
const double GRAVITY_CONST = 1.62; //moon gravity
const double BOUNCE_FACTOR = 0.8;
const double METER_TO_PIXEL = 1000/(0.26*100);
//The real aproximation of size of a pixel is 0.26 milimeters, however we want to present a more zoomed out picture so we will pretend that 1 pixel is 26 milimeters
//i.e. we will asume that the image on screen has a ratio of 1:100 to real world.

//All physics stuff will use meters as units when working with positions, velocities etc. When drawing and using graphics, position values will be translated to pixels
//by multiplying the position value by METER_TO_PIXEL and casting to unsigned int
class Ball {
private:
  double radius_;
  double posX_;
  double posY_;
  double velX_;
  double velY_;
  unsigned int colorRed_;
  unsigned int colorGreen_;
  unsigned int colorBlue_;
public:
  Ball(double radius = 10, double posX = SCREEN_WIDTH/2, double posY = SCREEN_HEIGHT/2, double velX = 0, double velY = 0,
    unsigned int colorRed = 255, unsigned int colorGreen = 255, unsigned int colorBlue = 255) : 
    radius_{radius}, posX_{posX}, posY_{posY}, velX_{velX}, velY_{velY}, colorRed_{colorRed}, colorGreen_{colorGreen}, colorBlue_{colorBlue} {};
  double get_radius() {return radius_;};
  double get_posX() {return posX_;};
  double get_posY() {return posY_;};
  double get_velX() {return velX_;};
  double get_velY() {return velY_;};
  void set_radius(double radius) {radius_ = radius;};
  void set_posX(double posX) {posX_ = posX;};
  void set_posY(double posY) {posY_ = posY;};
  void set_velX(double velX) {velX_ = velX;};
  void set_velY(double velY) {velY_ = velY;};
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

void updateState(Ball& ball, std::chrono::duration<double> deltaT) {
  if (ball.get_posY() + ball.get_radius() >= (SCREEN_HEIGHT - 10)/METER_TO_PIXEL) {
    if (ball.get_velY() > 0) ball.set_velY(-BOUNCE_FACTOR*ball.get_velY());
  }
  else {
    ball.set_velY(ball.get_velY() + GRAVITY_CONST*deltaT.count());
  }
  ball.set_posX(ball.get_posX() + ball.get_velX()*deltaT.count());
  ball.set_posY(ball.get_posY() + ball.get_velY()*deltaT.count());
}


int main(int argc, char* argv[]) {
  if (!SDL_init()) {
    printf("Could not initialize SDL stuff, :(\n");
    return 1;
  }
  Ball ball(0.5, SCREEN_WIDTH/(2*METER_TO_PIXEL), SCREEN_HEIGHT/(10*METER_TO_PIXEL), 0, 0, 0xFF, 0xFF, 0xFF);
  SDL_Rect floor = {0, SCREEN_HEIGHT-10, SCREEN_WIDTH, SCREEN_HEIGHT};
  bool running = true;
  std::chrono::duration<double> deltaT = std::chrono::seconds(0);
  while (running) {
    auto startTime = std::chrono::high_resolution_clock::now();
    //parse events
    //update world state
    updateState(ball, deltaT);
    //redraw screen
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0x10, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    fill_circle(METER_TO_PIXEL*ball.get_radius(), METER_TO_PIXEL*ball.get_posX(), METER_TO_PIXEL*ball.get_posY());
    SDL_RenderFillRect(gRenderer, &floor);
    SDL_RenderPresent(gRenderer);
    auto endTime = std::chrono::high_resolution_clock::now();
    deltaT = endTime - startTime;
  }
  SDL_end();
}
