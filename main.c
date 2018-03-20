#include <stdio.h>
#include "graphics_sdl.h"
#include "presa.h"
int main()
{
  init();
  initVars(); 
  maxCount = 2147483647;
  while(program == 1)
  {
    touchUpdate();
    if(page!=4)
    {
      /*if(readVariableValue("I_8"))*/
      readSensors();
    }
    logicTree();
    printf("MAX Count: %lu\n", maxCount); 
    renderBackground();
    renderStatusBar(); 
    renderContent();
    SDL_RenderPresent(renderer);
    /*
    printf("x: %d y: %d\n", touchLocation.x, touchLocation.y);
    printf("page: %d, sbText: %d\n", page, sbarText);*/
  }
  return 0;
}
