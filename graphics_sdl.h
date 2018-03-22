#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define RPI 1

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;
extern SDL_Point touchLocation;
extern SDL_Event event;

extern SDL_Color whiteColor;
extern SDL_Color blackColor;

extern SDL_Rect bar1;
extern SDL_Rect bar2;
extern SDL_Rect holes[5];

extern TTF_Font *smallText;
extern TTF_Font *regularText;

extern int textureWidth;
extern int textureHeight;

extern int timestamp;
extern int oldtimestamp;
extern int cycleCounter;
extern int cycleCheck;
extern int page_intro_FirstLoad;
extern int page_main_FirstLoad;

extern int page_settings_angle_FirstLoad;

extern int page_settings_dust_FirstLoadTwo;

extern int page_settings_screwdriver_FirstLoad;
extern int page_settings_screwdriver_FirstLoadTwo;
extern int page_settings_dust_FirstLoad;
extern int page_error_SecondLoad;

extern int left_button_selected;
extern int right_button_selected;
extern int pageNumber;

extern int pageLoadCount;
int blinkCounter;
int blinkerCounter;

int selected[8];
int noButtonSelected;
extern int id;

int init();
void freeTexture();
void renderBackground();
void render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip);
int renderText(const char *text, TTF_Font *textFont,  SDL_Color textColor);

void renderStatusBar();
void renderAdmin(int x, int y, int w, int h, int gotoNum);
void renderStatOne();
void renderContent();
void renderTurnSelect();
void renderModeSelect();
void renderErrorMode1();
void renderErrorMode2();
void renderError();
void renderSettingsAngle();
void renderSettingsScrewdriver();
void renderSettingsDust();
void renderHoming();
void renderManualMove();
void touchUpdate();
void up_button(int x, int y, int *incrementee, int incrementor);
void down_button(int x, int y, int *decrementee, int decrementor);
void left_button(int x, int y);
void right_button(int x, int y);

void button(int x, int y, int w, int h, char *text, int id);
void renderHoleLabels();
void button_save(int x, int y, int w, int h, int sel);
void sens_sel_button(int x, int y, int w, int h, int id);
void dust_sel_button(int x, int y, int w, int h, int id);
