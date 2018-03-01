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
  
  if (window == NULL)
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
  
  smallText = TTF_OpenFont("/home/pi/presa_program/DejaVuSansMono.ttf", 15);
  if(smallText == NULL)
  {
    printf("NO FONT FOUND\n");
  }
  regularText = TTF_OpenFont("/home/pi/presa_program/DejaVuSansMono.ttf", 30);
  if(regularText == NULL)
  {
    printf("NO FONT FOUND\n");
  }
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

  switch(status)
  {
    case 1:
      SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
      break;
    case 2:
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      break;
    case 3:
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
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderDrawLine(renderer, 0, 50, 1200, 50);
  SDL_RenderDrawLine(renderer, 0, 0, 1200, 0);
  SDL_RenderDrawLine(renderer, 0, 0, 0, 50);
 
 

  switch(regime)
  {
    case 1:
      renderText("STATUS: MIROVANJE", regularText, blackColor); 
      break;

    case 2:
      renderText("STATUS: DELOVANJE", regularText, whiteColor);
      break;

    case 3:
      renderText("STATUS: DELOVANJE - polavtomatsko", regularText, whiteColor);
      break;

    case 4:
      renderText("STATUS: NAPAKA", regularText, whiteColor);
      break;
    
    case 5: 
      renderText("NASTAVITVE", regularText, blackColor);
      break;
  } 
  render(100, 10, NULL, 0.0, NULL, SDL_FLIP_NONE);
}

void renderStatOne()
{
  int i;
  int y;
  int sens_c;
  int count_num;
  int s_count;
  sens_c = 0;
  y = 200;
  readParams(&page_main_FirstLoad);
  renderAdmin(1200, 0, 75, 50, 5);

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

  sprintf(savedPlus, "MAKSIMALNA DOVOLJENA VREDNOST: %d", savedHighThr);
  renderText(savedPlus, smallText, blackColor);
  render(500, 680, NULL, 0.0, NULL, SDL_FLIP_NONE);
    
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
    render(700, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    y = y + 100;
    count_num = count_num + 2;
    s_count = s_count + 2;
  }
} 

void renderSettings()
{
  readParams(&page_settings_FirstLoad);
  renderAdmin(1200, 0, 75, 50,1); 
 
  sprintf(currentValue, "TRENUTNA VREDNOST: %d", sensorsValue[0]);
  sprintf(marginValue, "TOLERANCA: %d ", currentMargin);
  sprintf(currentPlus, "TRENUTNA MAKSIMALNA DOVOLJENA VREDNOST: %d", sensorsValue[0] + currentMargin);
  sprintf(savedPlus, "SHRANJENA MAKSIMALNA DOVOLJENA VREDNOST: %d", savedHighThr);
  sprintf(currentText, "NASTAVITEV NAPETOSTI: %d V", setCurrent/1000);

  renderText(currentText, regularText, blackColor);
  render(100, 500, NULL, 0.0, NULL, SDL_FLIP_NONE);
   
  renderText(currentValue, regularText, blackColor);
  render(100, 100, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
  renderText(marginValue, regularText, blackColor);
  render(100, 200, NULL, 0.0, NULL, SDL_FLIP_NONE);

  renderText(currentPlus, regularText, blackColor);
  render(100, 300, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
  renderText(savedPlus, regularText, blackColor);
  render(100, 400, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
  up_button(400, 190, &currentMargin, 100);
  down_button(500, 190, &currentMargin, 100);

  up_button(600, 500, &setCurrent, 1000);
  down_button(700, 500, &setCurrent, 1000);
  button_save(1000, 390, 190, 50, 0);
  button_save(1000, 500, 190, 50, 1);
  printf("OUT_ANALOG: %d\n", readVariableValue("OutputValue_1_i04"));

}

void renderContent()
{
  switch(regime)
  {
    case 1:
      renderStatOne();
      break;
    case 5:
      renderSettings();
      break;
  }
  oldtimestamp=timestamp;
  cycleCounter++;
}

void touchUpdate()   /* handling touch events */
{
  while(SDL_PollEvent(&event) != 0 )
  {
    if(event.type == SDL_FINGERDOWN)  
    {
      
      timestamp = event.tfinger.timestamp;
      touchLocation.x = event.tfinger.x;
      touchLocation.y = event.tfinger.y;
    }
  }
  
  
}



void up_button(int x,  int y, int *incrementee, int incrementor)   
{
  SDL_Surface *imageSurface;
  freeTexture();
  imageSurface = IMG_Load("/home/pi/PRESA/images/up_black.png");

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

  if(touchLocation.x > x && touchLocation.x < x+100 && touchLocation.y > y && touchLocation.y < y + 100 && timestamp > oldtimestamp)
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
  imageSurface = IMG_Load("/home/pi/PRESA/images/down_black.png");

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

  if(touchLocation.x > x && touchLocation.x < x+100 && touchLocation.y > y && touchLocation.y < y + 100 && timestamp > oldtimestamp)
  {
    if(*decrementee > 0)
    {
     *decrementee = *decrementee - decrementor;
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
  renderText("SHRANI", regularText,  blackColor);
  render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 
  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp && sel == 0)
  {
    savedHighThr = sensorsValue[0] + currentMargin;
    system("rm /home/pi/PRESA/data/param_sens.txt");	  
    fp_sens = fopen("/home/pi/PRESA/data/param_sens.txt", "w");
    fprintf(fp_sens, "%d\n", currentMargin);    
    fprintf(fp_sens, "%d\n", savedHighThr);
    fclose(fp_sens);
    
  }
  else if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp && sel == 1)
  {
    system("rm /home/pi/PRESA/data/param_curr.txt");	  
    fp_curr = fopen("/home/pi/PRESA/data/param_curr.txt", "w");
    fprintf(fp_curr, "%d\n", setCurrent);
    fclose(fp_curr);
    
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
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderDrawLine(renderer, x, y, (x+w), y);
  SDL_RenderDrawLine(renderer, (x+w), y, (x+w), (y+h)); 
  SDL_RenderDrawLine(renderer, (x+w), (y+h), x, (y+h));
  SDL_RenderDrawLine(renderer, x, (y+h), x, y);
  renderText("...", regularText, blackColor);
  render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 

  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp && cycleCounter != cycleCheck)
  {
    cycleCheck = cycleCounter;
    
    regime = gotoNum;
    if(regime == 1)
    {
      page_main_FirstLoad = 1;
    }
    else if(regime == 5)
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
