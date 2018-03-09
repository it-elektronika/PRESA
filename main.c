#include <stdio.h>
#include "graphics_sdl.h"
#include "presa.h"
int main()
{
  timer(10);
      
  init();
  initVars(); 
  while(program == 1)
  {

    touchUpdate();
    if(page!=4)
    {
      readSensors();
    }
    checkError();
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
