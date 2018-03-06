#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "presa.h"
#include "kunbus.h"
#include "graphics_sdl.h"

void readSensors()  /* handling struct for drawing grids */
{  
  int i;
  encoder = round((readVariableValue("InputValue_1_i04")-substract)/divisor);

  /* IF encoder in right position read values */
  sensorsValue[0] = readVariableValue("InputValue_1");
  sensorsValue[1] = readVariableValue("InputValue_2");
  sensorsValue[2] = readVariableValue("InputValue_3");
  sensorsValue[3] = readVariableValue("InputValue_4");
  sensorsValue[4] = readVariableValue("InputValue_5");
  sensorsValue[5] = readVariableValue("InputValue_6");
  sensorsValue[6] = readVariableValue("InputValue_7");
  sensorsValue[7] = readVariableValue("InputValue_8");
  sensorsValue[8] = readVariableValue("InputValue_9");
  sensorsValue[9] = readVariableValue("InputValue_10");
  
  for(i = 0; i < 10; ++i)
  {
    if(sensorsValue[i] > savedHighThr && (encoder >= screwdHigh || encoder <= screwdLow))
    {
      sensors[i] = 1;
    }
    else
    {
      sensors[i] = 0;
    }

    if(sensorsValue[i] < dustThr && (encoder <= dustHigh && encoder >= dustLow))
    {
      sensorsDust[i] = 1;
    }
    else
    {
      sensorsDust[i] = 0;
    }
  }
}

void readParams(int *pageFirstLoad)
{
  int i;
  
  #ifdef RPI
  fp_sens = fopen("/home/pi/PRESA/data/param_sens.txt", "r");
  fp_curr = fopen("/home/pi/PRESA/data/param_curr.txt", "r");
  #endif
  #ifdef LUKA
  fp_sens = fopen("/home/luka/PRESA/data/param_sens.txt", "r");
  fp_curr = fopen("/home/luka/PRESA/data/param_curr.txt", "r");
  #endif
  if(*pageFirstLoad) 
  {
    for(i = 0; i < 3; ++i)
    {
      getline(&line, &len, fp_sens);
  
      if(i==0)
      {
        margin = atoi(line);
      }
      else if(i==1)
      {
        savedHighThr=atoi(line);
      }
    }
    currentMargin = margin;
 
    for(i = 0; i < 1; ++i)
    { 
      getline(&line, &len, fp_curr);
      if(i==0)
      {
        setCurrent=atoi(line);
        writeVariableValue("OutputValue_1_i04", setCurrent); 
      }
    }
    *pageFirstLoad = 0;    
  }
}

void checkError()
{
  if(page == 2)
  {
    int i;

    for(i = 0; i < 4; i++)
    {
      if(sensors[i] == 1)
      {
        errorMode = 1;
        page = 4;
        sbarText = 7;
        break;
      }
      else
      {
        errorMode = 0;
      }
      if(sensorsDust[i]== 1)
      {
        errorMode = 2;
        page = 4;
        sbarText = 8;
        break;
      }
      else
      {
        errorMode = 0;
      }
    }
    printf("ERROR MODE: %d\n", errorMode);
  }
} 
 
void checkSelectP0()
{
  if(readVariableValue("I_1"))
  {
    if(left_button_selected || right_button_selected)
    {
      page = 1;
    } 
  }
}

void checkSelectP1()
{
  if(readVariableValue("I_2"))
  {
    if(selected[0] == 1)
    {
      sbarText = 0;
    }
    else if(selected[1] == 1)
    {
      sbarText = 1;
    }
    else if(selected[2] == 1)
    {
      sbarText = 2;
      page = 2;
    }
    else if(selected[3] == 1)
    {
      page = 2;
      sbarText = 3;
    }
    else
    {
      sbarText = 5;
    }
  }
}

void checkStopCycle()
{
  if(readVariableValue("I_3"))
  {
    page = 1;
  }
}

void logicTree()
{
  switch(page)
  {
    case 0:
      checkSelectP0();
      break;

    case 1:
      checkSelectP1(); 
      break; 
    
    case 2:
      checkStopCycle();
      break;
  }
}



/* fire signal when in position between 160 and 340. else off */
