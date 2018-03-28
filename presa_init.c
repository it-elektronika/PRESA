#include <stdio.h>

int page = 0;
int sbarText = 4;
int backgroundColor = 1;

char fileBuff[10][50];
char fileBuffRm[10][50];
char fileBuffDust[10][50];
char fileBuffRmDust[10][50];

int procedure = 0;

int sensors[11];
int sensorsValue[10];
int sensorsDust[11];
int sensorsValueDust[10];


char sensorLowThrText[30];
char sensorHighThrText[30];
char dustLowThrText[30];
char dustHighThrText[30];

unsigned long maxCount = 0;

int dustThr = 14000;
char textInsert[20];
char sensorVals[10][10];
char sensorDustVals[10][10];
char sensorLabels[10][10];
char labPlusVal[10][10];
int labels[10];

char encoderVal[50];
char savedValue[50];

char currentValue[10][50];
char marginValue[10][30];
char savedPlus[10][50];
char currentPlus[10][50];


char currentDustValue[10][50];
char marginDustValue[10][30];
char savedMinus[10][50];
char currentMinus[10][50];

char piece_count_text[30];
char piece_count_text_all[30];


char currentText[40];
int setCurrent;

char airText[30];
char oilText[30];
int margin;
int currentMargin[10];
int highThr;
int savedHighThr[10];

int dustMargin;
int currentDustMargin[10];
int lowThr;
int savedLowThr[10];

int errorMode = 0;
int encoder;

int sens_sel[10];
int dust_sel[10];


int counted = 0;
long press_count = 0;
long press_count_all = 0;
float divisor = 44.579387187;   /* (HIGHEST VALUE - LOWEST VALUE)/359*/
int substract = 4026;           /* LOWEST VALUE */

int screwdHighThr = 300;
int screwdLowThr = 120;

int dustHighThr = 299;
int dustLowThr = 121;

int program = 1;

int PiControlHandle_g = -1;

FILE *fp_sens;
FILE *fp_curr;
FILE *fp_dust;
FILE *fp_thr;
FILE *fp_press_count;
FILE *fp_press_count_all;
FILE *fp_sens_sel;
FILE *fp_dust_sel;

char *line;
size_t len;
