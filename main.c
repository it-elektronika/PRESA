#include <stdio.h>
#include "graphics_sdl.h"
#include "presa.h"
int main()
{
  init();
  while(program == 1)
  {
    touchUpdate();
    renderBackground();
    renderStatusBar();
    readSensors();
    logicTree();
    renderContent();
    SDL_RenderPresent(renderer);
    printf("x: %d y: %d\n", touchLocation.x, touchLocation.y);
    printf("page: %d, sbText: %d\n", page, sbarText);
  }
  return 0;
}
