int status = 1;
int regime = 1;

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
char currentValue[50];
char marginValue[50];
char savedPlus[50];
char currentPlus[50];
char currentText[10];
int setCurrent;

int margin;
int currentMargin;
int highThr;
int savedHighThr;

int encoder;

float divisor = 44.441666667;
int substract = 3983;

int screwdHigh = 300;
int screwdLow = 110;

int dustHigh = 299;
int dustLow = 111;

int PiControlHandle_g = -1;

