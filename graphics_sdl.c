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
  } 
  if(page !=3)
  {
    render(100, 10, NULL, 0.0, NULL, SDL_FLIP_NONE);
  }
  else
  {
    render(30, 10, NULL, 0.0, NULL, SDL_FLIP_NONE);
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
  
  readSensParams(&page_main_FirstLoad);
  SDL_RenderDrawRect(renderer, &bar1);
  SDL_RenderDrawRect(renderer, &bar2);
  
  for(i = 0; i < 5; i++)
  {
    holes[i].x = 50;
    holes[i].y = y;
    holes[i].w = 310;
    holes[i].h = 10;
    y = y + 100;
  }
  
  sprintf(encoderVal, "ENCODER: %d", encoder);
  renderText(encoderVal, smallText,  blackColor);
  render(500, 720, NULL, 0.0, NULL, SDL_FLIP_NONE);
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
  y=180;
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
  y = 180;
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

  readSensParams(&page_settings_FirstLoad);
  
  renderAdmin(1200, 0, 80, 80,1); 
  
  for(i = 0; i < 10; ++i)
  {
    sprintf(currentValue[i], "%d", sensorsValue[i]);
    sprintf(marginValue[i], "%d", currentMargin[i]);
    sprintf(currentPlus[i], "%d", sensorsValue[i] + currentMargin[i]);
    sprintf(savedPlus[i], "%d",  savedHighThr[i]);

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



    for(j = 0; j < 5; j++)
    {
      SDL_RenderDrawLine(renderer, x1, y1+j, x2, y2+j);
    }
    
    y1 = y1 + 65;
    y2 = y2 + 65;
    ya = ya + 65;
    y = y + 65;
  }

  /*
  up_button(400, 190, &currentMargin, 100);
  down_button(500, 190, &currentMargin, 100);

  */
 
    

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
  readCurrParams(&page_main_FirstLoad);
  sprintf(currentText, "NASTAVITEV NAPETOSTI: %d V", setCurrent/1000);

  up_button(1050, 700, &setCurrent, 1000);
  down_button(1150, 700, &setCurrent, 1000);

  renderText(currentText, regularText, blackColor);
  render(300, 700, NULL, 0.0, NULL, SDL_FLIP_NONE);
 
  left_button(400, 250);
  right_button(700, 250); 
  
  for(i = 0; i < 10; i++)
  {
    SDL_RenderDrawLine(renderer, 0, 600+i, 1280, 600+i);
  }
}

void renderModeSelect()
{
  button(100, 150, 550, 150, "POMIK V IZHODISCE", 0);
  button(100, 300, 550, 150, "ROCNI NACIN", 1);
  button(100, 450, 550, 150, "POL-AVTOMATSKI NACIN", 2);
  button(100, 600, 550, 150, "AVTOMATSKI NACIN", 3);
  
  button(800, 150, 400, 150, "SENZORJI", 5);
  
  if(readVariableValue("I_4"))
  {
    sprintf(airText, "PRITISK ZRAKA: V REDU");
  }
  else
  {
    sprintf(airText, "PRITISK ZRAKA: NI V REDU");
  }
  
  if(readVariableValue("I_5"))
  {
    sprintf(oilText, "NIVO OLJA: V REDU");
  }
  else
  {
    sprintf(oilText, "NIVO OLJA: NI V REDU");
  }
  renderText(airText, smallText, blackColor);
  render(800, 600, NULL, 0.0, NULL, SDL_FLIP_NONE);
 
  renderText(oilText, smallText, blackColor);
  render(800, 650, NULL, 0.0, NULL, SDL_FLIP_NONE);

  sprintf(encoderVal, "POZICIJA: %d%c", encoder, 0x00B0);
  renderText(encoderVal, smallText,  blackColor);
  render(800, 700, NULL, 0.0, NULL, SDL_FLIP_NONE);
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

  sprintf(encoderVal, "ENCODER: %d", encoder);
  renderText(encoderVal, smallText,  blackColor);
  render(500, 720, NULL, 0.0, NULL, SDL_FLIP_NONE);

  /*sprintf(savedPlus, "MAKSIMALNA DOVOLJENA VREDNOST: %d", savedHighThr[0]);
  renderText(savedPlus, smallText, blackColor);
  render(500, 680, NULL, 0.0, NULL, SDL_FLIP_NONE);   
  */   
  y = 180;
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
  y = 180;
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

  sprintf(encoderVal, "ENCODER: %d", encoder);
  renderText(encoderVal, smallText,  blackColor);
  render(500, 720, NULL, 0.0, NULL, SDL_FLIP_NONE);

  /*sprintf(savedPlus, "MAKSIMALNA DOVOLJENA VREDNOST: %d", savedHighThr[0]);
  renderText(savedPlus, smallText, blackColor);
  render(500, 680, NULL, 0.0, NULL, SDL_FLIP_NONE);   
  */  
  y = 180;
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
  y = 180;
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
    
    if(*incrementee <= 9000)
    {
      *incrementee = *incrementee + incrementor;
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
    if(*decrementee > 0)
    {
     *decrementee = *decrementee - decrementor;
    }
  }
}

void left_button(int x,  int y)   
{
  int i;
  SDL_Surface *imageSurface;
  freeTexture();
  #ifdef RPI
  imageSurface = IMG_Load("/home/pi/PRESA/images/left_200_black.png");
  #endif
  #ifdef LUKA
  imageSurface = IMG_Load("/home/luka/PRESA/images/left_200_black.png");
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
  imageSurface = IMG_Load("/home/pi/PRESA/images/right_200_black.png");
  #endif
  #ifdef LUKA
  imageSurface = IMG_Load("/home/luka/PRESA/images/right_200_black.png");
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
  /*
  else if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp && sel == 1)
  {
    #ifdef RPI
    system("rm /home/pi/PRESA/data/param_curr.txt");	  
    fp_curr = fopen("/home/pi/PRESA/data/param_curr.txt", "w");
    #endif
    #ifdef LUKA
    system("rm /home/luka/PRESA/data/param_curr.txt");	  
    fp_curr = fopen("/home/luka/PRESA/data/param_curr.txt", "w");
    #endif
    fprintf(fp_curr, "%d\n", setCurrent);
    fclose(fp_curr);
    
  }*/
}

void button(int x, int y, int w, int h, char *text, int id)  /* save row/column data button */
{
  int i;
  
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

  if(id != 5)
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
  else
  {
    page_settings_FirstLoad = 1;
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
  render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 

  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp && cycleCounter != cycleCheck)
  {
    cycleCheck = cycleCounter;
    
    page = gotoNum;
    sbarText = 5;
    if(page == 1)
    {
      page_main_FirstLoad = 1;
    }
    else if(page == 5)
    {
      page_settings_FirstLoad = 1;
    }
    /* memset(&passText[0], 0, 5); enable if using keypad*/ 
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
