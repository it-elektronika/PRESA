extern FILE *fp_sens[10];
extern FILE *fp_curr;

extern char fileBuff[10][50];
extern char fileBuffRm[10][50];

extern char *line;
extern size_t len;

extern int sbarText;
extern int page;
extern int backgroundColor;

extern int sensors[11];
extern int sensorsValue[10];
extern int sensorsDust[11];
extern int sensorsValueDust[10];

extern int dustThr;
extern char textInsert[20];
extern char sensorVals[10][10];
extern char sensorDustVals[10][10];
extern char sensorLabels[10][10];
extern char labPlusVal[10][10];
extern int labels[10];

extern char encoderVal[50];
extern char savedValue[50];

extern char currentValue[10][50];
extern char marginValue[10][30];
extern char savedPlus[10][50];
extern char currentPlus[10][50];
extern char currentText[40];
extern int setCurrent;

extern char airText[20];
extern char oilText[20];
extern int errorMode;

extern int margin;
extern int currentMargin[10];
extern int highThr;
extern int savedHighThr[10];

extern float divisor;
extern int substract;

extern int screwdHigh;
extern int screwdLow;
extern int dustHigh;
extern int dustLow;
extern int encoder;

extern int program;

extern void readSensors();
extern void readSensParams(int *pageFirstLoad);
extern void readCurrParams(int *pageFirstLoad);
extern void checkError();
extern void checkSelectP0();
extern void checkSelectP1();
extern void logicTree();
extern void initVars();
