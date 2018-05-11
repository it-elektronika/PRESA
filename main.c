/* 

   GRABENER PRESSEN 
    IT ELEKTRONIKA
         2018


Prenova stroja za presanje izvijacev

Program sestoji iz dveh glavnih delov:

- presa ; delovna logika prese
- graphics ; risanje na ekran ( z nekaj izjemami)

*/



#include <stdio.h>
#include "graphics_sdl.h"
#include "presa.h"
#include "kunbus.h"
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
    encoder = round((readVariableValue("InputValue_3_i05")-substract)/divisor);
    fp_encoder = fopen("/home/pi/PRESA/data/enc.txt", "a");
    fprintf(fp_encoder, "%d\n", encoder);
    fclose(fp_encoder);
      */  
    /*
    printf("x: %d y: %d\n", touchLocation.x, touchLocation.y);
    printf("page: %d, sbText: %d\n", page, sbarText);*/
    /*printf("setCurrent:%d\n", setCurrent);*/
  }
  return 0;
}
