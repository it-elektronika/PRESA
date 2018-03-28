#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "graphics_sdl.h"
#include "presa.h"
#include "kunbus.h"

int init()    /* things needed to start sdl2 properly */
{
  int flags;
  int innited;
 
  flags = IMG_INIT_JPG|IMG_INIT_PNG;
  innited = IMG_Init(flags);
  	
  if((SDL_Init(SDL_INIT_VIDEO||SDL_INIT_EVENTS)) != 0)
  {
    SDL_Log("Unable to initialize SDL:%s ", SDL_GetError());
    return 1;                                                                               
  }  

  window = SDL_CreateWindow("IT-Elektronika", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
  
  if(window == NULL)
  {
    return 1;
  }		  

  renderer = SDL_CreateRenderer(window, - 1, SDL_RENDERER_SOFTWARE);
  if(renderer == NULL)
  {
    ;
    /*printf("RENDERER IS NULL\n");*/
  }

  if((innited&flags) != flags)
  {
    
    printf("IMG_INIT: %s\n", IMG_GetError());
  }

  if(TTF_Init() == -1)
  {
    printf("TTF ERROR\n");
  }
  
  #ifdef RPI
  smallText = TTF_OpenFont("/home/pi/PRESA/font/DejaVuSansMono.ttf", 30);
  if(smallText == NULL)
  {
    printf("NO FONT FOUND\n");
  }
  regularText = TTF_OpenFont("/home/pi/PRESA/font/DejaVuSansMono.ttf", 45);
  if(regularText == NULL)
  {
    printf("NO FONT FOUND\n");
  }
  #endif
  
  #ifdef LUKA
  smallText = TTF_OpenFont("/home/luka/PRESA/font/DejaVuSansMono.ttf", 30);
  if(smallText == NULL)
  {
    printf("NO FONT FOUND\n");
  }
  regularText = TTF_OpenFont("/home/luka/PRESA/font/DejaVuSansMono.ttf", 45);
  if(regularText == NULL)
  {
    printf("NO FONT FOUND\n");
  }
  #endif
  return 15;
}

void freeTexture(void)  
{
  if(texture != NULL)
  {
    SDL_DestroyTexture(texture);	  
    texture = NULL;
    textureWidth = 0;
    textureHeight = 0;
  }
}

void renderBackground(void)     
{
  switch(backgroundColor)
  {
    case 0:
      SDL_SetRenderDrawColor(renderer, 0, 178, 0, 255);
      break;
    case 1:
      SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
      break;
    case 2:
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      break;
  }
  SDL_RenderClear(renderer);
}

void render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
  SDL_Rect renderQuad;
  renderQuad.x = x;
  renderQuad.y = y;
  renderQuad.w = textureWidth;
  renderQuad.h = textureHeight;

  if(clip != NULL)
  {
    renderQuad.w = clip -> w;
    renderQuad.h = clip -> h;
  }
  SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
}

int renderText(const char *text, TTF_Font *textFont,  SDL_Color textColor)  
{
  SDL_Surface* textSurface;
  textSurface = TTF_RenderText_Solid(textFont, text, textColor);
  freeTexture();

  if(textSurface == NULL)
  {
    printf("Unable to render text surface! SDL Error: %s\n", SDL_GetError());
  }
  else
  {
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if(texture == NULL)
    {
      printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
      textureWidth = textSurface -> w;
      textureHeight = textSurface -> h;
    }
    SDL_FreeSurface(textSurface);
  }
  return texture != NULL;
}

void renderStatusBar()
{
  int i;
  int x1_1;
  int x1_2;
  int y1_1; 
  int y1_2;

  int x2_1;
  int x2_2;
  int y2_1; 
  int y2_2;

  int x3_1;
  int x3_2;
  int y3_1; 
  int y3_2;

  x1_1 = 0;
  y1_1 = 80;
  x1_2 = 1280;
  y1_2 = 80;

  x2_1 = 0;
  y2_1 = 0;
  x2_2 = 1280;
  y2_2 = 0;

  x3_1 = 0;
  y3_1 = 0;
  x3_2 = 0;
  y3_2 = 80;
 
  for(i = 0; i < 10; i++)
  {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderDrawLine(renderer, x1_1, y1_1+i, x1_2, y1_2+i);
    SDL_RenderDrawLine(renderer, x2_1, y2_1, x2_2, y2_2);
    SDL_RenderDrawLine(renderer, x3_1, y3_1, x3_2, y3_2);
  }

  switch(sbarText)
  {
    case 0:
      renderText("POMIKANJE V IZHODISCE", regularText, blackColor); 
      break;

    case 1:
      renderText("ROCNO POMIKANJE", regularText, blackColor);
      break;

    case 2:
      renderText("POL-AVTOMATSKI NACIN", regularText, blackColor);
      break;

    case 3:
      renderText("AVTOMATSKI NACIN", regularText, blackColor);
      break;
    
    case 4: 
      renderText("IZBIRA SMERI VRTENJA", regularText, blackColor);
      break;

    case 5: 
      renderText("IZBIRA NACINA DELOVANJA", regularText, blackColor);
      break;    

    case 6: 
      renderText("NASTAVITVE", regularText, blackColor);
      break;    
    
    case 7: 
      renderText("IZVIJAC IZVEN POZICIJE", regularText, blackColor);
      break;    
 
    case 8:
      renderText("SMET NA SENZORJU", regularText, blackColor);
      break;

    case 9:
      renderText("SENZORJI - IZVIJAC", regularText, blackColor);
      break;

    case 10:
      renderText("SENZORJI - SMET", regularText, blackColor);
      break;

    case 11:
      renderText("PRESA STISNJENA", regularText, blackColor);
      break;

    case 12:
      renderText("KOT ZAZNAVANJA", regularText, blackColor);
  } 
  
  if(page == 5 || page == 6) 
  {
    render(30, 10, NULL, 0.0, NULL, SDL_FLIP_NONE);
  }
  else
  {
    render(100, 10, NULL, 0.0, NULL, SDL_FLIP_NONE);
  }
}

void renderInCycle()
{
  int i;
  int y;
  int sens_c;
  int count_num;
  int s_count;
  
  sens_c = 0;
  y = 200;
  
  readSensParams(&page_settings_screwdriver_FirstLoad);
  readDustParams(&page_settings_dust_FirstLoad);

  SDL_RenderDrawRect(renderer, &bar1);
  SDL_RenderDrawRect(renderer, &bar2);
  
  fp_press_count = fopen("/home/pi/PRESA/data/press_count.txt", "w");
  fprintf(fp_press_count, "%lu\n", press_count);
  fclose(fp_press_count);
  
  fp_press_count_all = fopen("/home/pi/PRESA/data/press_count_all.txt", "w");
  fprintf(fp_press_count_all, "%lu\n", press_count_all);
  fclose(fp_press_count_all);


  for(i = 0; i < 5; i++)
  {
    holes[i].x = 50;
    holes[i].y = y;
    holes[i].w = 310;
    holes[i].h = 10;
    y = y + 100;
  }
  
  renderHoleLabels(); 
  
  sprintf(encoderVal, "POZICIJA: %d%c", encoder, 0x00B0);
  renderText(encoderVal, smallText,  blackColor);
  render(500, 720, NULL, 0.0, NULL, SDL_FLIP_NONE);

  sprintf(piece_count_text, "ST. KOSOV: %lu", press_count);
  renderText(piece_count_text, smallText,  blackColor);
  render(500, 680, NULL, 0.0, NULL, SDL_FLIP_NONE);



/*
  sprintf(savedPlus, "MAKSIMALNA DOVOLJENA VREDNOST: %d", savedHighThr[0]);
  renderText(savedPlus, smallText, blackColor);
  render(500, 680, NULL, 0.0, NULL, SDL_FLIP_NONE);
*/    
  for(i = 0; i < 5; i++)
  {
    if(sensors[sens_c] > 0 || sensors[sens_c+1] > 0)
    {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderFillRect(renderer, &holes[i]);
    }
    else
    {
      SDL_RenderDrawRect(renderer, &holes[i]);
    }
    sens_c = sens_c + 2;
  }

  count_num = 0;
  s_count = 1;
  y=150;
  for(i = 0; i < 5; i++)
  {
    sprintf(sensorVals[count_num], "S%d: %d", s_count, sensorsValue[count_num]);
    renderText(sensorVals[count_num], regularText,  blackColor);
    render(500, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    y = y + 100;
    count_num = count_num + 2;
    s_count = s_count + 2;
  }
  count_num = 1;
  s_count = 2;
  y = 150;
  for(i = 0; i < 5; i++)
  {
    sprintf(sensorVals[count_num], "S%d: %d", s_count, sensorsValue[count_num]);
    renderText(sensorVals[count_num], regularText,  blackColor);
    render(800, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    y = y + 100;
    count_num = count_num + 2;
    s_count = s_count + 2;
  }
} 
void renderSettings()
{
  
  renderAdmin(1200, 0, 80, 80,1);
  button(100, 150, 570, 155, "SENZORJI - IZVIJAC", 5);
  button(100, 300, 570, 155, "SENZORJI - SMET", 6);
  button(100, 450, 570, 155, "KOTI ZAZNAVANJA", 7);
}

void renderSettingsAngle()
{
  renderAdmin(1200, 0, 80, 80, 3);
  readThrParams(&page_settings_angle_FirstLoad);
 
  renderText("IZVIJAC NA SENZORJU", smallText, blackColor);
  render(100, 130, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
  sprintf(sensorLowThrText, "SPODNJA MEJA: %d", screwdLowThr);
  renderText(sensorLowThrText, regularText,  blackColor);
  render(100, 180, NULL, 0.0, NULL, SDL_FLIP_NONE);

  sprintf(sensorHighThrText, "ZGORNJA MEJA: %d", screwdHighThr);
  renderText(sensorHighThrText, regularText,  blackColor);
  render(100, 260, NULL, 0.0, NULL, SDL_FLIP_NONE);

  
  renderText("SMET NA SENZORJU", smallText, blackColor);
  render(100, 400, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
  sprintf(dustLowThrText, "SPODNJA MEJA: %d", dustLowThr);
  renderText(dustLowThrText, regularText,  blackColor);
  render(100, 450, NULL, 0.0, NULL, SDL_FLIP_NONE);

  sprintf(dustHighThrText, "ZGORNJA MEJA: %d", dustHighThr);
  renderText(dustHighThrText, regularText,  blackColor);
  render(100,  550, NULL, 0.0, NULL, SDL_FLIP_NONE);

  up_button(600, 180, &screwdLowThr, 1);
  down_button(700, 180, &screwdLowThr, 1);

  up_button(600, 260, &screwdHighThr, 1);
  down_button(700, 260, &screwdHighThr, 1);

  up_button(600, 450, &dustLowThr, 1);
  down_button(700, 450, &dustLowThr, 1);

  up_button(600, 550, &dustHighThr, 1);
  down_button(700, 550, &dustHighThr, 1);

  button_save(100, 650, 150, 65, -1);
}

void renderSettingsScrewdriver()
{
  int i;
  int j;
  int x1;
  int x2;
  int y1;
  int y2;
  
  int y;
  int ya;
  y = 165;
  ya = 150;
  x1 = 0;
  y1 = 150;
  
  x2 = 1280;
  y2 = 150;

  readSensParams(&page_settings_screwdriver_FirstLoad);
  readSensSelectParams(&page_settings_screwdriver_FirstLoadTwo);

  renderAdmin(1200, 0, 80, 80,3); 
  
  for(i = 0; i < 10; ++i)
  {
    sprintf(currentValue[i], "%d", sensorsValue[i]);
    sprintf(marginValue[i], "%d", currentMargin[i]);
    sprintf(currentPlus[i], "%d", sensorsValue[i] + currentMargin[i]);
    sprintf(savedPlus[i], "%d",  savedHighThr[i]);
    
    if(i!=9)
    {  
      sens_sel_button(30, y, 35, 35, i);
    }
    else
    {
      sens_sel_button(30, y, 50, 35, i);
    }
    if(sens_sel[i])
    {
      renderText(currentValue[i], smallText, blackColor);
      render(170, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
      renderText(marginValue[i], smallText, blackColor);
      render(450, y, NULL, 0.0, NULL, SDL_FLIP_NONE);

      renderText(currentPlus[i], smallText, blackColor);
      render(700, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
      renderText(savedPlus[i], smallText, blackColor);
      render(900, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    
      renderText(sensorLabels[i], smallText,  blackColor);
      render(30, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    
      up_button(540, y, &currentMargin[i], 1000);
      down_button(620, y, &currentMargin[i], 1000);
      button_save(1150, ya, 130, 65, i);
   
    }
    for(j = 0; j < 5; j++)
    {
      SDL_RenderDrawLine(renderer, x1, y1+j, x2, y2+j);
    }
    
    y1 = y1 + 65;
    y2 = y2 + 65;
    ya = ya + 65;
    y = y + 65;
  }

  SDL_RenderDrawLine(renderer, 160, 90, 160, 800);
  SDL_RenderDrawLine(renderer, 440, 90, 440, 800);
  SDL_RenderDrawLine(renderer, 690, 90, 690, 800);
  SDL_RenderDrawLine(renderer, 890, 90, 890, 800);

  renderText("SENZOR", smallText, blackColor);
  render(30, 100, NULL, 0.0, NULL,  SDL_FLIP_NONE);

  renderText("TRENUTNA VRED.", smallText, blackColor);
  render(170, 100, NULL, 0.0, NULL,  SDL_FLIP_NONE);

  renderText("TOLERANCA", smallText, blackColor);
  render(450, 100, NULL, 0.0, NULL,  SDL_FLIP_NONE);

  renderText("MAKS.VRED.", smallText, blackColor);
  render(700, 100, NULL, 0.0, NULL,  SDL_FLIP_NONE);

  renderText("SHRANJENA MAKS.VRED.", smallText, blackColor);
  render(900, 100, NULL, 0.0, NULL,  SDL_FLIP_NONE);
}
 

void renderSettingsDust()
{
  int i;
  int j;
  int x1;
  int x2;
  int y1;
  int y2;
  
  int y;
  int ya;
  y = 165;
  ya = 150;
  x1 = 0;
  y1 = 150;
  
  x2 = 1280;
  y2 = 150;

  readDustParams(&page_settings_dust_FirstLoad);
  readDustSelectParams(&page_settings_dust_FirstLoadTwo);

  renderAdmin(1200, 0, 80, 80,3); 
  
  for(i = 0; i < 10; ++i)
  {
    sprintf(currentDustValue[i], "%d", sensorsValue[i]);
    sprintf(marginDustValue[i], "%d", currentDustMargin[i]);
    sprintf(currentMinus[i], "%d", sensorsValue[i] - currentDustMargin[i]);
    sprintf(savedMinus[i], "%d",  savedLowThr[i]);
    
    if(i!=9)
    {  
      dust_sel_button(30, y, 35, 35, i);
    }
    else
    {
      dust_sel_button(30, y, 50, 35, i);
    }
 
    if(dust_sel[i])
    {
      renderText(currentDustValue[i], smallText, blackColor);
      render(170, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
      renderText(marginDustValue[i], smallText, blackColor);
      render(450, y, NULL, 0.0, NULL, SDL_FLIP_NONE);

      renderText(currentMinus[i], smallText, blackColor);
      render(700, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
      renderText(savedMinus[i], smallText, blackColor);
      render(900, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    
      renderText(sensorLabels[i], smallText,  blackColor);
      render(30, y, NULL, 0.0, NULL, SDL_FLIP_NONE);

      up_button(540, y, &currentDustMargin[i], 1000);
      down_button(620, y, &currentDustMargin[i], 1000);
      button_save(1150, ya, 130, 65, i);
    }

    for(j = 0; j < 5; j++)
    {
      SDL_RenderDrawLine(renderer, x1, y1+j, x2, y2+j);
    }
    
    y1 = y1 + 65;
    y2 = y2 + 65;
    ya = ya + 65;
    y = y + 65;
  }

  SDL_RenderDrawLine(renderer, 160, 90, 160, 800);
  SDL_RenderDrawLine(renderer, 440, 90, 440, 800);
  SDL_RenderDrawLine(renderer, 690, 90, 690, 800);
  SDL_RenderDrawLine(renderer, 890, 90, 890, 800);

  renderText("SENZOR", smallText, blackColor);
  render(30, 100, NULL, 0.0, NULL,  SDL_FLIP_NONE);

  renderText("TRENUTNA VRED.", smallText, blackColor);
  render(170, 100, NULL, 0.0, NULL,  SDL_FLIP_NONE);

  renderText("TOLERANCA", smallText, blackColor);
  render(450, 100, NULL, 0.0, NULL,  SDL_FLIP_NONE);

  renderText("MAKS.VRED.", smallText, blackColor);
  render(700, 100, NULL, 0.0, NULL,  SDL_FLIP_NONE);

  renderText("SHRANJENA MAKS.VRED.", smallText, blackColor);
  render(900, 100, NULL, 0.0, NULL,  SDL_FLIP_NONE);
 
}

void renderTurnSelect()
{
  int i;
  
  sprintf(currentText, "STEVILO UDARCEV NA MINUTO: %d", (int)round(setCurrent/138.888888889));
  
  if(setCurrent <=9999)
  {
    up_button(1000, 700, &setCurrent, 138.888888889);
  }
  if(setCurrent >= 4341)
  {
    down_button(1100, 700, &setCurrent, 138.888888889);
  }
 
  renderText(currentText, regularText, blackColor);
  render(200, 700, NULL, 0.0, NULL, SDL_FLIP_NONE);
 
  left_button(400, 250);
  right_button(700, 250); 
  
  for(i = 0; i < 10; i++)
  {
    SDL_RenderDrawLine(renderer, 0, 600+i, 1280, 600+i);
  }
}

void renderModeSelect()
{
  if(right_button_selected)
  {

    button(100, 150, 570, 155, "POMIK V IZHODISCE", 0);
    button(100, 300, 570, 155, "ROCNI NACIN", 1);
   
    button(100, 450, 570, 155, "POL-AVTOMATSKI NACIN", 2);
    button(100, 600, 570, 155, "AVTOMATSKI NACIN", 3);
  
    button(800, 150, 400, 155, "NASTAVITVE", 4);
  }

  else if(left_button_selected)
  {
    button(100, 150, 570, 155, "POMIK V IZHODISCE", 0);
    button(100, 300, 570, 155, "ROCNI NACIN", 1);
  }

  if(checkAir())
  {
    sprintf(airText, "PRITISK ZRAKA: V REDU");
    renderText(airText, smallText, blackColor);
    render(800, 600, NULL, 0.0, NULL, SDL_FLIP_NONE);
  }
  else if(checkAir() == 0 && blinkerCounter > 10)
  {
    sprintf(airText, "PRITISK ZRAKA: NI V REDU");
    renderText(airText, smallText, blackColor);
    render(800, 600, NULL, 0.0, NULL, SDL_FLIP_NONE);
  }
  
  if(checkOil())
  {
    sprintf(oilText, "NIVO OLJA: V REDU");
    renderText(oilText, smallText, blackColor);
    render(800, 650, NULL, 0.0, NULL, SDL_FLIP_NONE);
  }
  else if(checkOil() == 0 && blinkerCounter > 10)
  {
    sprintf(oilText, "NIVO OLJA: NI V REDU");
    renderText(oilText, smallText, blackColor);
    render(800, 650, NULL, 0.0, NULL, SDL_FLIP_NONE);
  }
 
  sprintf(piece_count_text, "ST. KOSOV: %lu", press_count);
  renderText(piece_count_text, smallText,  blackColor);
  render(800, 400, NULL, 0.0, NULL, SDL_FLIP_NONE);
 
  button(1100, 400, 30, 30, "R", 8);

  sprintf(piece_count_text_all, "ST. KOSOV SKUPAJ: %lu", press_count_all);
  renderText(piece_count_text_all, smallText,  blackColor);
  render(800, 450, NULL, 0.0, NULL, SDL_FLIP_NONE);

  sprintf(encoderVal, "POZICIJA: %d%c", encoder, 0x00B0);
  renderText(encoderVal, smallText,  blackColor);
  render(800, 700, NULL, 0.0, NULL, SDL_FLIP_NONE);

  if(blinkerCounter > 50)  
  {
    blinkerCounter = 0;
  }
  blinkerCounter++;

}

void renderErrorMode1()
{
  int i;
  
  int y;
  int count_num;
  int s_count;
  int sens_c;
  
  SDL_RenderDrawRect(renderer, &bar1);
  SDL_RenderDrawRect(renderer, &bar2);
  renderAdmin(1200, 0, 80, 80, 1);

  sprintf(encoderVal, "POZICIJA: %d%c", encoder, 0x00B0);
  renderText(encoderVal, smallText,  blackColor);
  render(500, 720, NULL, 0.0, NULL, SDL_FLIP_NONE);

  /*sprintf(savedPlus, "MAKSIMALNA DOVOLJENA VREDNOST: %d", savedHighThr[0]);
  renderText(savedPlus, smallText, blackColor);
  render(500, 680, NULL, 0.0, NULL, SDL_FLIP_NONE);   
  */   

  renderHoleLabels();   

  y = 150;
  count_num = 0;
  s_count = 1;

  for(i = 0; i < 5; i++)
  {
    if(sensors[count_num] > 0 && blinkCounter > 10)
    {
      sprintf(sensorVals[count_num], "S%d: %d", s_count, sensorsValue[count_num]);
      renderText(sensorVals[count_num], regularText, blackColor);
      render(500, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    }
    else if(sensors[count_num] < 1)
    {
      sprintf(sensorVals[count_num], "S%d: %d", s_count, sensorsValue[count_num]);
      renderText(sensorVals[count_num], regularText, blackColor);
      render(500, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    }
    y = y + 100;
    count_num = count_num + 2;
    s_count = s_count + 2;
  }
  y = 150;
  count_num = 1;
  s_count = 2;

  for(i = 0; i < 5; i++)
  {
    if(sensors[count_num] > 0 && blinkCounter > 10)
    {
      sprintf(sensorVals[count_num], "S%d: %d", s_count, sensorsValue[count_num]);
      renderText(sensorVals[count_num], regularText, blackColor);
      render(800, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    }
    else if(sensors[count_num] < 1)
    {
      sprintf(sensorVals[count_num], "S%d: %d", s_count, sensorsValue[count_num]);
      renderText(sensorVals[count_num], regularText, blackColor);
      render(800, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    }
    y = y + 100;
    count_num = count_num + 2;
    s_count = s_count + 2;
  }
    
  if(blinkCounter > 50)  
  {
    blinkCounter = 0;
  }
  blinkCounter++;
  sens_c = 0;  
  for(i = 0; i < 5; i++)
  {
    if(sensors[sens_c] > 0 || sensors[sens_c+1] > 0)
    {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderFillRect(renderer, &holes[i]);
    }
    else
    {
      SDL_RenderDrawRect(renderer, &holes[i]);
    }
    sens_c = sens_c + 2;
  }

}

void renderErrorMode2()
{
  int i;
  int y;
  int count_num;
  int s_count;
  int sens_c;
  renderAdmin(1200, 0, 80,80, 1);
  
  SDL_RenderDrawRect(renderer, &bar1);
  SDL_RenderDrawRect(renderer, &bar2);

  sprintf(encoderVal, "POZICIJA: %d%c", encoder, 0x00B0);
  renderText(encoderVal, smallText,  blackColor);
  render(500, 720, NULL, 0.0, NULL, SDL_FLIP_NONE);

  /*sprintf(savedPlus, "MAKSIMALNA DOVOLJENA VREDNOST: %d", savedHighThr[0]);
  renderText(savedPlus, smallText, blackColor);
  render(500, 680, NULL, 0.0, NULL, SDL_FLIP_NONE);   
  */

  renderHoleLabels(); 
    
  y = 150;
  count_num = 0;
  s_count = 1;

  for(i = 0; i < 5; i++)
  {
    if(sensorsDust[count_num] > 0 && blinkCounter > 10)
    {
      sprintf(sensorDustVals[count_num], "S%d: %d", s_count, sensorsValue[count_num]);
      renderText(sensorDustVals[count_num], regularText, blackColor);
      render(500, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    }
    else if(sensorsDust[count_num] < 1)
    {
      sprintf(sensorDustVals[count_num], "S%d: %d", s_count, sensorsValue[count_num]);
      renderText(sensorDustVals[count_num], regularText, blackColor);
      render(500, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    }
    y = y + 100;
    count_num = count_num + 2;
    s_count = s_count + 2;
  }
  y = 150;
  count_num = 1;
  s_count = 2;

  for(i = 0; i < 5; i++)
  {
    if(sensorsDust[count_num] > 0 && blinkCounter > 10)
    {
      sprintf(sensorDustVals[count_num], "S%d: %d", s_count, sensorsValue[count_num]);
      renderText(sensorDustVals[count_num], regularText, blackColor);
      render(800, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    }
    else if(sensorsDust[count_num] < 1)
    {
      sprintf(sensorDustVals[count_num], "S%d: %d", s_count, sensorsValue[count_num]);
      renderText(sensorDustVals[count_num], regularText, blackColor);
      render(800, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    }
    y = y + 100;
    count_num = count_num + 2;
    s_count = s_count + 2;
  }
    
  if(blinkCounter > 50)  
  {
    blinkCounter = 0;
  }
  blinkCounter++;
  sens_c = 0;  
  
  for(i = 0; i < 5; i++)
  {
    if(sensorsDust[sens_c] > 0 || sensorsDust[sens_c+1] > 0)
    {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderFillRect(renderer, &holes[i]);
    }
    else
    {
      SDL_RenderDrawRect(renderer, &holes[i]);
    }
    sens_c = sens_c + 2;
  }
}

void renderHoming()
{
  sprintf(encoderVal, "POZICIJA: %d%c", encoder, 0x00B0);
  renderText(encoderVal, regularText,  blackColor);
  render(100, 400, NULL, 0.0, NULL, SDL_FLIP_NONE);
}

void renderManualMove()
{
  sprintf(encoderVal, "POZICIJA: %d%c", encoder, 0x00B0);
  renderText(encoderVal, regularText,  blackColor);
  render(100, 400, NULL, 0.0, NULL, SDL_FLIP_NONE);
}

void renderError()
{
  switch(errorMode)
  {
    case 1:
      renderErrorMode1();
      break;
    case 2:
      renderErrorMode2();      
      break; 
  }
}



void renderContent()
{
  switch(page)
  {
    case 0:
      backgroundColor = 1;
      renderTurnSelect();
      break;
    case 1:
      backgroundColor = 1;
      renderModeSelect();
      break;
    case 2:
      backgroundColor = 0;
      renderInCycle();
      break;
    case 3:
      backgroundColor = 1;
      renderSettings();
      break;
    case 4:
      backgroundColor = 2;
      renderError();
      break;
    case 5:
      renderSettingsScrewdriver();
      backgroundColor = 1;
      break;
    case 6:
      renderSettingsDust();
      backgroundColor = 1;
      break;
    case 7:
      renderHoming();
      backgroundColor = 1;
      break;
    case 8:
      renderManualMove();
      backgroundColor = 1;
      break;
    case 9:
      renderSettingsAngle();
      backgroundColor = 1;
      break;
  }
  oldtimestamp=timestamp;
  cycleCounter++;
}

void touchUpdate()   /* handling touch events */
{
  while(SDL_PollEvent(&event) != 0 )
  {
    #ifdef RPI
    if(event.type == SDL_FINGERDOWN)  
    {
      
      timestamp = event.tfinger.timestamp;
      touchLocation.x = event.tfinger.x;
      touchLocation.y = event.tfinger.y;
    }
    #endif
    #ifdef LUKA
    if(event.type == SDL_MOUSEBUTTONDOWN)
    {
      timestamp = event.button.timestamp;
      touchLocation.x = event.button.x;
      touchLocation.y = event.button.y;
    }
    #endif
  }  
}

void up_button(int x,  int y, int *incrementee, int incrementor)   
{
  SDL_Surface *imageSurface;
  freeTexture();
  #ifdef RPI
  imageSurface = IMG_Load("/home/pi/PRESA/images/up_black.png");
  #endif
  #ifdef LUKA
  imageSurface = IMG_Load("/home/luka/PRESA/images/up_black.png");
  #endif
  if(imageSurface == NULL)
  {
    printf("Unable to render image surface! SDL_ImageError: %s\n", IMG_GetError());
  }
  else
  {
    texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if(texture == NULL)
    {
      printf("Unable to create texture from rendered image! SDL_ImageError: %s\n", SDL_GetError());
    }
    else
    {
      textureWidth = imageSurface -> w;
      textureHeight = imageSurface -> h;
    }
    SDL_FreeSurface(imageSurface);
  }

  render(x, y, NULL, 0.0, NULL, SDL_FLIP_NONE);

  if(touchLocation.x > x && touchLocation.x < x+imageSurface->w && touchLocation.y > y && touchLocation.y < y + imageSurface->h && timestamp > oldtimestamp)
  {
    if(page==0)
    {
      if(*incrementee <= 9999)
      {
      
        *incrementee = *incrementee + incrementor;
        #ifdef RPI
        fp_curr = fopen("/home/pi/PRESA/data/param_curr.txt", "w");
        #endif
        #ifdef LUKA
        fp_curr = fopen("/home/luka/PRESA/data/param_curr.txt", "w");
        #endif 
        fprintf(fp_curr, "%d\n", setCurrent);
        fclose(fp_curr);
      }
      writeVariableValue("OutputValue_4_i05", setCurrent);
    } 
    else if(page!=0)
    {
      if(*incrementee <= 9999)
      {
        *incrementee = *incrementee + incrementor;
      }
    }
  }
}

void down_button(int x, int y, int *decrementee, int decrementor)     
{
  SDL_Surface *imageSurface;
  freeTexture();
  #ifdef RPI
  imageSurface = IMG_Load("/home/pi/PRESA/images/down_black.png");
  #endif
  #ifdef LUKA
  imageSurface = IMG_Load("/home/luka/PRESA/images/down_black.png");
  #endif
  if(imageSurface == NULL)
  {
    printf("Unable to render image surface! SDL_ImageError: %s\n", IMG_GetError());
  }
  else
  {
    texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if(texture == NULL)
    {
      printf("Unable to create texture from rendered image! SDL_ImageError: %s\n", SDL_GetError());
    }
    else
    {
      textureWidth = imageSurface -> w;
      textureHeight = imageSurface -> h;
    }
    SDL_FreeSurface(imageSurface);
  }	
  render(x, y, NULL, 0.0, NULL, SDL_FLIP_NONE);

  if(touchLocation.x > x && touchLocation.x < x+imageSurface->w && touchLocation.y > y && touchLocation.y < y + imageSurface->h && timestamp > oldtimestamp)
  {
    if(page==0)
    {
      if(*decrementee > 0)
      {
       *decrementee = *decrementee - decrementor;
       #ifdef RPI
       fp_curr = fopen("/home/pi/PRESA/data/param_curr.txt", "w");
       #endif
       #ifdef LUKA
       fp_curr = fopen("/home/luka/PRESA/data/param_curr.txt", "w");
       #endif
       fprintf(fp_curr, "%d\n", setCurrent);
       fclose(fp_curr);
      }
      /*writeVariableValue("OutputValue_1_i05", setCurrent);*/
    }
    else if(page!=0)
    {
      if(*decrementee > 0)
      {
       *decrementee = *decrementee - decrementor;
      }
    }
  }
}

void left_button(int x,  int y)   
{
  int i;
  SDL_Surface *imageSurface;
  freeTexture();
  #ifdef RPI
  imageSurface = IMG_Load("/home/pi/PRESA/images/left_200.png");
  #endif
  #ifdef LUKA
  imageSurface = IMG_Load("/home/luka/PRESA/images/left_200.png");
  #endif
  
  if(imageSurface == NULL)
  {
    printf("Unable to render image surface! SDL_ImageError: %s\n", IMG_GetError());
  }
  else
  {
    texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if(texture == NULL)
    {
      printf("Unable to create texture from rendered image! SDL_ImageError: %s\n", SDL_GetError());
    }
    else
    {
      textureWidth = imageSurface -> w;
      textureHeight = imageSurface -> h;
    }
    SDL_FreeSurface(imageSurface);
  }

  render(x, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
  if(touchLocation.x > x && touchLocation.x < x+imageSurface->w && touchLocation.y > y && touchLocation.y < y + imageSurface->h && timestamp > oldtimestamp)
  {
    if(left_button_selected == 0 /*&& right_button_selected == 0*/)
    {
      right_button_selected = 0;
      left_button_selected = 1;
    }
    else
    {
      left_button_selected = 0;
    }
  }

  if(left_button_selected == 1)
  {
    for(i = 0; i < 10; i++)
    {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderDrawLine(renderer, x, y+i, (x+imageSurface->w), y+i);
      SDL_RenderDrawLine(renderer, (x+imageSurface->w+i), y, (x+imageSurface->w+i), (y+imageSurface->h)); 
      SDL_RenderDrawLine(renderer, (x+imageSurface->w), (y+imageSurface->h-i), x, (y+imageSurface->h-i));
      SDL_RenderDrawLine(renderer, x+i, (y+imageSurface->h), x+i, y);
    }
  }

  
}

void right_button(int x,  int y)   
{
  int i;
  SDL_Surface *imageSurface;
  freeTexture();
  #ifdef RPI
  imageSurface = IMG_Load("/home/pi/PRESA/images/right_200.png");
  #endif
  #ifdef LUKA
  imageSurface = IMG_Load("/home/luka/PRESA/images/right_200.png");
  #endif
  
  if(imageSurface == NULL)
  {
    printf("Unable to render image surface! SDL_ImageError: %s\n", IMG_GetError());
  }
  else
  {
    texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if(texture == NULL)
    {
      printf("Unable to create texture from rendered image! SDL_ImageError: %s\n", SDL_GetError());
    }
    else
    {
      textureWidth = imageSurface -> w;
      textureHeight = imageSurface -> h;
    }
    SDL_FreeSurface(imageSurface);
  }

  render(x, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
  if(touchLocation.x > x && touchLocation.x < x+imageSurface->w && touchLocation.y > y && touchLocation.y < y + imageSurface->h && timestamp > oldtimestamp)
  {
    if(right_button_selected == 0/* && left_button_selected == 0*/)
    {
      left_button_selected = 0;
      right_button_selected = 1;
      
    }
    else
    {
      right_button_selected = 0;
    }
  }
  
  if(right_button_selected == 1)
  {
    for(i = 0; i < 10; i++)
    {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderDrawLine(renderer, x, y+i, (x+imageSurface->w), y+i);
      SDL_RenderDrawLine(renderer, (x+imageSurface->w+i), y, (x+imageSurface->w+i), (y+imageSurface->h)); 
      SDL_RenderDrawLine(renderer, (x+imageSurface->w), (y+imageSurface->h-i), x, (y+imageSurface->h-i));
      SDL_RenderDrawLine(renderer, x+i, (y+imageSurface->h), x+i, y);
    }
  }
 
}

void button_save(int x, int y, int w, int h, int sel)  
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderDrawLine(renderer, x, y, (x+w), y);
  SDL_RenderDrawLine(renderer, (x+w), y, (x+w), (y+h)); 
  SDL_RenderDrawLine(renderer, (x+w), (y+h), x, (y+h));
  SDL_RenderDrawLine(renderer, x, (y+h), x, y);
  renderText("SHRANI", smallText,  blackColor);
  render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 
  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
  {
    if(page == 5)
    {
      savedHighThr[sel] = sensorsValue[sel] + currentMargin[sel];
      #ifdef RPI
      sprintf(fileBuff[sel], "/home/pi/PRESA/data/sensor_%d_param.txt", sel);
      sprintf(fileBuffRm[sel],"rm %s", fileBuff[sel]);
 
      system(fileBuffRm[sel]);	  
      fp_sens[sel] = fopen(fileBuff[sel], "w");
      #endif
    
      #ifdef LUKA
      sprintf(fileBuff[sel], "/home/luka/PRESA/data/sensor_%d_param.txt", sel);
      sprintf(fileBuffRm[sel],"rm %s", fileBuff[sel]);
  
      system(fileBuffRm[sel]);	  
      fp_sens[sel] = fopen(fileBuff[sel], "w");
      #endif
      fprintf(fp_sens[sel], "%d\n", currentMargin[sel]);    
      fprintf(fp_sens[sel], "%d\n", savedHighThr[sel]);
      fclose(fp_sens[sel]);
    }
    if(page == 6)
    {
      savedLowThr[sel] = sensorsValue[sel] - currentDustMargin[sel];
      #ifdef RPI
      sprintf(fileBuffDust[sel], "/home/pi/PRESA/data/dust_%d_param.txt", sel);
      sprintf(fileBuffRmDust[sel],"rm %s", fileBuffDust[sel]);
 
      system(fileBuffRmDust[sel]);	  
      fp_dust[sel] = fopen(fileBuffDust[sel], "w");
      #endif
    
      #ifdef LUKA
      sprintf(fileBuffDust[sel], "/home/luka/PRESA/data/dust_%d_param.txt", sel);
      sprintf(fileBuffRmDust[sel],"rm %s", fileBuffDust[sel]);
  
      system(fileBuffRmDust[sel]);	  
      fp_dust[sel] = fopen(fileBuffDust[sel], "w");
      #endif
      fprintf(fp_dust[sel], "%d\n", currentDustMargin[sel]);    
      fprintf(fp_dust[sel], "%d\n", savedLowThr[sel]);
      fclose(fp_dust[sel]);
    }
    if(page == 9)
    {
      #ifdef RPI
      system("rm /home/pi/PRESA/data/param_thr.txt");
      fp_thr = fopen("/home/pi/PRESA/data/param_thr.txt", "w");	  
      #endif
    
      #ifdef LUKA
      system("rm /home/luka/PRESA/data/param_thr.txt");
      fp_thr = fopen("/home/luka/PRESA/data/param_thr.txt", "w");
      
      #endif
      fprintf(fp_thr, "%d\n", screwdLowThr);    
      fprintf(fp_thr, "%d\n", screwdHighThr);     
      fprintf(fp_thr, "%d\n", dustLowThr);
      fprintf(fp_thr, "%d\n", dustHighThr);
      fclose(fp_thr);
    }
  }
}

void button(int x, int y, int w, int h, char *text, int id)  /* save row/column data button */
{
  int i;
  
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

  if(id == 0 || id == 1 || id == 2 || id == 3)
  {

    if(selected[id] == 0)
    {
      for(i = 0; i < 5; i++)
      {
        SDL_RenderDrawLine(renderer, x, y+i, (x+w), y+i);
        SDL_RenderDrawLine(renderer, (x+w+i), y, (x+w+i), (y+h)); 
        SDL_RenderDrawLine(renderer, (x+w), (y+h-i), x, (y+h-i));
        SDL_RenderDrawLine(renderer, x+i, (y+h), x+i, y);      
      }
    }
    if(selected[id] == 1)
    {
      for(i = 0; i < 15; i++)
      {
        SDL_RenderDrawLine(renderer, x, y+i, (x+w), y+i);
        SDL_RenderDrawLine(renderer, (x+w+i), y, (x+w+i), (y+h)); 
        SDL_RenderDrawLine(renderer, (x+w), (y+h-i), x, (y+h-i));
        SDL_RenderDrawLine(renderer, x+i, (y+h), x+i, y);      
      }
    }

    renderText(text, regularText, blackColor);
    render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 
  
    if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
    {
      for(i = 0; i < 4; i++)
      {
        if(selected[i] == 1)
        {
          noButtonSelected = 0;
          break;
        }
        else if(selected[i] == 0)
        {  
          noButtonSelected = 1;
        }
      }
      if(selected[id] == 0 /*&& noButtonSelected == 1*/) 
      {
        for(i = 0; i < 4; i++)
        {
          selected[i] = 0;
        }
        selected[id] = 1;
      }
      else if(selected[id] == 1)
      {
        selected[id] = 0;
        sbarText = 5;
      }
    }
  }
  
  
  
  else if(id == 4)
  {
    for(i = 0; i < 5; i++)
    {
      SDL_RenderDrawLine(renderer, x, y+i, (x+w), y+i);
      SDL_RenderDrawLine(renderer, (x+w+i), y, (x+w+i), (y+h)); 
      SDL_RenderDrawLine(renderer, (x+w), (y+h-i), x, (y+h-i));
      SDL_RenderDrawLine(renderer, x+i, (y+h), x+i, y);      
    }
    

    renderText(text, regularText, blackColor);
    render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 
  
    if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
    {
      page = 3;
      sbarText = 6;
    }
  }

  else if(id == 5)
  {
    for(i = 0; i < 5; i++)
    {
      SDL_RenderDrawLine(renderer, x, y+i, (x+w), y+i);
      SDL_RenderDrawLine(renderer, (x+w+i), y, (x+w+i), (y+h)); 
      SDL_RenderDrawLine(renderer, (x+w), (y+h-i), x, (y+h-i));
      SDL_RenderDrawLine(renderer, x+i, (y+h), x+i, y);      
    }

    renderText(text, regularText, blackColor);
    render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 
  
    if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
    {
      /*page_settings_screwdriver_FirstLoad = 1;*/
      page = 5;
      sbarText = 9;
    }
  }

  else if(id == 6)
  {
    
    for(i = 0; i < 5; i++)
    {
      SDL_RenderDrawLine(renderer, x, y+i, (x+w), y+i);
      SDL_RenderDrawLine(renderer, (x+w+i), y, (x+w+i), (y+h)); 
      SDL_RenderDrawLine(renderer, (x+w), (y+h-i), x, (y+h-i));
      SDL_RenderDrawLine(renderer, x+i, (y+h), x+i, y);      
    }
    
    renderText(text, regularText, blackColor);
    render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 
  
    if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
    {
      /*page_settings_dust_FirstLoad = 1;*/
      page = 6;
      sbarText = 10;
    }
  }
  else if(id == 7)
  {
    for(i = 0; i < 5; i++)
    {
      SDL_RenderDrawLine(renderer, x, y+i, (x+w), y+i);
      SDL_RenderDrawLine(renderer, (x+w+i), y, (x+w+i), (y+h)); 
      SDL_RenderDrawLine(renderer, (x+w), (y+h-i), x, (y+h-i));
      SDL_RenderDrawLine(renderer, x+i, (y+h), x+i, y);      
    }
    
    renderText(text, regularText, blackColor);
    render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 
  
    if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
    {
      /*page_settings_angle_FirstLoad = 1;*/
      page = 9;
      sbarText = 12;
    }
  }

  else if(id == 8)
  {
    SDL_RenderDrawLine(renderer, x, y, (x+w), y);
    SDL_RenderDrawLine(renderer, (x+w), y, (x+w), (y+h)); 
    SDL_RenderDrawLine(renderer, (x+w), (y+h), x, (y+h));
    SDL_RenderDrawLine(renderer, x, (y+h), x, y);      
    
    renderText(text, smallText, blackColor);
    render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 
  
    if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
    {
      /*page_settings_angle_FirstLoad = 1;*/
      press_count = 0;
      fp_press_count = fopen("/home/pi/PRESA/data/press_count.txt", "w");
      fprintf(fp_press_count, "%lu\n", press_count);
      fclose(fp_press_count);
    }
  }
}

/*
void keypad(int x, int y, int w, int h) 
{
  int i = 1;
  int j = 1;
  int nums = 1;
  int origX = x;
  int origY = y;
  
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderDrawLine(renderer, x, y-h, (x+w*3), y-h);
  SDL_RenderDrawLine(renderer, (x+w*3), y-h, (x+w*3), y); 
  SDL_RenderDrawLine(renderer, (x+w*3), y, x, y);
  SDL_RenderDrawLine(renderer, x, y, x, y-h);
  writeText(passText, regularText, textColor);
  render(x, y - h , NULL, 0.0, NULL, SDL_FLIP_NONE); 

  for(j = 1; j < 4; ++j)
  {  
    for(i = 1; i < 4; ++i)
    {
      SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
      SDL_RenderDrawLine(renderer, x, y, (x+w), y);
      SDL_RenderDrawLine(renderer, (x+w), y, (x+w), (y+h)); 
      SDL_RenderDrawLine(renderer, (x+w), (y+h), x, (y+h));
      SDL_RenderDrawLine(renderer, x, (y+h), x, y);
      sprintf(numBuff, "%d", nums);
      writeText(numBuff, regularText, textColor);
      render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 
      if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp && cycleCheck != cycleCounter)
      {
        cycleCheck = cycleCounter;
        if(passText[4] == '\0')
        {
          strcat(passText, numBuff);
        }
      }
      if(passText[4] != '\0')
      {
        int ret;
        ret = strcmp(DROWSAPP, passText);
        if(ret == 0)
        {
          pageNumber = 3;  
        }
        memset(&passText[0], 0, 5);
      }
      x = x + w;
      nums++;
    }
    x = origX;
    y = y + h;
  }
  y = origY;
}
*/

void renderAdmin(int x, int y, int w, int h, int gotoNum) 
{
  int i;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  for(i = 0; i < 5; ++i)
  {
    SDL_RenderDrawLine(renderer, x, y+i, (x+w), y+i);
    SDL_RenderDrawLine(renderer, (x+w+i), y, (x+w+i), (y+h)); 
  
    SDL_RenderDrawLine(renderer, x+i, (y+h), x+i, y);
  }
  renderText("...", regularText, blackColor);
  render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2+10)), NULL, 0.0, NULL, SDL_FLIP_NONE); 

  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp && cycleCounter != cycleCheck)
  {
    cycleCheck = cycleCounter;
    
    page = gotoNum;
    if(page == 1)
    {
      page_main_FirstLoad = 1;
      sbarText = 5;

    }
    else if(page == 3)
    {
      sbarText = 6;
      page_settings_screwdriver_FirstLoad = 1;
      page_settings_dust_FirstLoad = 1;
      page_settings_angle_FirstLoad = 1;
      page_settings_screwdriver_FirstLoadTwo = 1;
      page_settings_dust_FirstLoadTwo = 1;
      
      fp_sens_sel = fopen("/home/pi/PRESA/data/param_sens_sel.txt", "w");

      for(i = 0; i < 10; i++)
      {
        fprintf(fp_sens_sel, "%d\n", sens_sel[i]);
      }
      fclose(fp_sens_sel);
      
      fp_dust_sel = fopen("/home/pi/PRESA/data/param_dust_sel.txt", "w");

      for(i = 0; i < 10; i++)
      {
        fprintf(fp_dust_sel, "%d\n", dust_sel[i]);
      }
      fclose(fp_dust_sel);


    }   
    /* memset(&passText[0], 0, 5); enable if using keypad*/ 
  }
}

void renderHoleLabels()
{
  int i;
  int y;
  int count_num;

  
  y = 160;
  count_num = 0;
  
  for(i = 0; i < 5; i++)
  {
    renderText(sensorLabels[count_num], smallText,  blackColor);
    render(60, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    y = y + 100;
    count_num = count_num + 2;
  }
  y = 160;
  count_num = 1;
  
  for(i = 0; i < 5; i++)
  {
    renderText(sensorLabels[count_num], smallText,  blackColor);
    render(315, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    y = y + 100;
    count_num = count_num + 2;
  }
}

/*
void DrawCircle(SDL_Renderer *Renderer, s32 _x, s32 _y, s32 radius)
{ 
   s32 x = radius - 1;
   s32 y = 0;
   s32 tx = 1;
   s32 ty = 1;
   s32 err = tx - (radius << 1); 
   while (x >= y)
   {
     
      SDL_RenderDrawPoint(Renderer, _x + x, _y - y);
      SDL_RenderDrawPoint(Renderer, _x + x, _y + y);
      SDL_RenderDrawPoint(Renderer, _x - x, _y - y);
      SDL_RenderDrawPoint(Renderer, _x - x, _y + y);
      SDL_RenderDrawPoint(Renderer, _x + y, _y - x);
      SDL_RenderDrawPoint(Renderer, _x + y, _y + x);
      SDL_RenderDrawPoint(Renderer, _x - y, _y - x);
      SDL_RenderDrawPoint(Renderer, _x - y, _y + x);

      if (err <= 0)
      {
         y++;
         err += ty;
         ty += 2;
      }
      else if (err > 0)
      {
         x--;
         tx += 2;
         err += tx - (radius << 1);
      }
   }
}



*/

void sens_sel_button(int x, int y, int w, int h, int id)
{
  SDL_RenderDrawLine(renderer, x, y, (x+w), y);
  SDL_RenderDrawLine(renderer, (x+w), y, (x+w), (y+h)); 
  SDL_RenderDrawLine(renderer, (x+w), (y+h), x, (y+h));
  SDL_RenderDrawLine(renderer, x, (y+h), x, y);      

  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
  {
    if(sens_sel[id]==1)
    {
      sens_sel[id] = 0; 
    }
    else
    {
      sens_sel[id] = 1;
    }
  } 
}



void dust_sel_button(int x, int y, int w, int h, int id)
{
  SDL_RenderDrawLine(renderer, x, y, (x+w), y);
  SDL_RenderDrawLine(renderer, (x+w), y, (x+w), (y+h)); 
  SDL_RenderDrawLine(renderer, (x+w), (y+h), x, (y+h));
  SDL_RenderDrawLine(renderer, x, (y+h), x, y);      

  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
  {
    if(dust_sel[id]==1)
    {
      dust_sel[id] = 0; 
    }
    else
    {
      dust_sel[id] = 1;
    }
  } 
}
