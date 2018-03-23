#include <stdio.h>
#include "graphics_sdl.h"
#include "presa.h"
int main()
{
  init();
  initVars(); 
  while(program == 1)
  {
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
    /*printf("setCurrent:%d\n", setCurrent);*/
  }
  return 0;
}
