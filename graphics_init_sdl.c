#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

SDL_Renderer *renderer;
SDL_Window *window;
SDL_Texture *texture;
SDL_Event event;

SDL_Point touchLocation = {-1, -1};

SDL_Color whiteColor = {255, 255, 255, 0};
SDL_Color blackColor = {0, 0, 0, 0};

TTF_Font *smallText;
TTF_Font *regularText;

SDL_Rect bar1 = {100, 150, 10, 500};
SDL_Rect bar2 = {300, 150, 10, 500};

SDL_Rect holes[5];


int textureWidth = 0;
int textureHeight = 0;

int timestamp = 0;
int oldtimestamp = 0;

int pageNumber = 0;
int cycleCounter = 0;
int cycleCheck = 0;

int page_main_FirstLoad = 1;
int page_settings_FirstLoad = 1;

int left_button_selected = 0;
int right_button_selected = 0;

int selected[5];
int noButtonSelected = 1;
