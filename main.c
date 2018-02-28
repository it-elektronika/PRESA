#include <stdio.h>
#include "graphics_sdl.h"
#include "presa.h"
int main()
{
  init();
  while(1)
  {
    touchUpdate();
    renderBackground();
    renderStatusBar();
    renderAdmin(1200, 0, 75, 50, 5);
    readSensors();
    renderStatOne();
    renderContent();
    SDL_RenderPresent(renderer);
  }
  return 0;
}
