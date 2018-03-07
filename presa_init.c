#include <stdio.h>

int page = 0;
int sbarText = 4;
int backgroundColor = 1;

char fileBuff[10][50];
char fileBuffRm[10][50];

int sensors[11];
int sensorsValue[10];
int sensorsDust[11];
int sensorsValueDust[10];

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

char currentText[40];
int setCurrent = 10000;

char airText[20];
char oilText[20];
int margin;
int currentMargin[10];
int highThr;
int savedHighThr[10];
int errorMode = 0;
int encoder;

float divisor = 44.441666667;
int substract = 3983;

int screwdHigh = 300;
int screwdLow = 110;

int dustHigh = 299;
int dustLow = 111;

int program = 1;

int PiControlHandle_g = -1;

FILE *fp_sens;
FILE *fp_curr;
char *line;
size_t len;
