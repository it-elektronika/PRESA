extern FILE *fp_sens;
extern FILE *fp_curr;
extern char *line;
extern size_t len;

extern int status;
extern int regime;

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
extern char currentValue[50];
extern char marginValue[50];
extern char savedPlus[50];
extern char currentPlus[50];
extern char currentText[40];
extern int setCurrent;

extern int margin;
extern int currentMargin;
extern int highThr;
extern int savedHighThr;

extern float divisor;
extern int substract;

extern int screwdHigh;
extern int screwdLow;
extern int dustHigh;
extern int dustLow;
extern int encoder;

extern int program;

extern void readSensors();
extern void readParams(int *pageFirstLoad);
