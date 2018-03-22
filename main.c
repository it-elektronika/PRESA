#include <stdio.h>
#include "graphics_sdl.h"
#include "presa.h"
int main()
{
  init();
  initVars(); 
  maxCount = 2147483000;
  while(program == 1)
  {
    maxCount++;
    touchUpdate();
    if(page!=4)
    {
      /*if(readVariableValue("I_8"))*/
      readSensors();
    }
    logicTree();
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
