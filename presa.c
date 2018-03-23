#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
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
  sensorsValue[4] = readVariableValue("InputValue_2_i04");
  sensorsValue[5] = readVariableValue("InputValue_3_i04");
  sensorsValue[6] = readVariableValue("InputValue_4_i04");
  sensorsValue[7] = readVariableValue("InputValue_2_i05");
  sensorsValue[8] = readVariableValue("InputValue_3_i05");
  sensorsValue[9] = readVariableValue("InputValue_4_i05");
 
  for(i = 0; i < 10; ++i)
  {
    if(sensorsValue[i] > savedHighThr[i] && encodeRange(screwdHighThr, screwdLowThr, 1) && sens_sel[i] )
    {
      sensors[i] = 1;
    }
    else
    {
      sensors[i] = 0;
    }

    if(sensorsValue[i] < savedLowThr[i] && encodeRange(dustLowThr, dustHighThr, 0) && dust_sel[i])
    {
      sensorsDust[i] = 1;
    }
    else
    {
      sensorsDust[i] = 0;
    }
  }
}

void readSensParams(int *pageFirstLoad)
{
  int i;
  int j;
  if(*pageFirstLoad)
  { 
    for(i = 0; i < 10; i++)
    {
      printf("fileBuff[%d]:%s\n", i, fileBuff[i]);
      #ifdef RPI
      fp_sens[i] = fopen(fileBuff[i], "r");
      if(fp_sens[i] == NULL)
      {
        printf("File: %d - FILE NOT OPENED\n", i);
      }
      #endif
      #ifdef LUKA
      fp_sens[i] = fopen(fileBuff[i], "r");
      #endif

      for(j = 0; j < 2; ++j)
      {
        getline(&line, &len, fp_sens[i]);
  
        if(j==0)
        {
          margin = atoi(line);
        }
        else if(j==1)
        {
          savedHighThr[i]=atoi(line);
        }
      }
      currentMargin[i] = margin;
    }
    *pageFirstLoad = 0;
  }
}

void readDustParams(int *pageFirstLoad)
{
  int i;
  int j;
  if(*pageFirstLoad)
  { 
    for(i = 0; i < 10; i++)
    {
      printf("fileBuffDust[%d]:%s\n", i, fileBuffDust[i]);
      #ifdef RPI
      fp_dust[i] = fopen(fileBuffDust[i], "r");
      if(fp_dust[i] == NULL)
      {
        printf("File: %d - FILE NOT OPENED\n", i);
      }
      #endif
      #ifdef LUKA
      fp_dust[i] = fopen(fileBuffDust[i], "r");
      #endif

      for(j = 0; j < 2; ++j)
      {
        getline(&line, &len, fp_dust[i]);
  
        if(j==0)
        {
          dustMargin = atoi(line);
        }
        else if(j==1)
        {
          savedLowThr[i]=atoi(line);
        }
      }
      currentDustMargin[i] = dustMargin;
    }
    *pageFirstLoad = 0;
  }
}

void readCurrParams()
{
  int i;
  #ifdef RPI
  fp_curr = fopen("/home/pi/PRESA/data/param_curr.txt", "r");
  #endif
  #ifdef LUKA
  fp_curr = fopen("/home/luka/PRESA/data/param_curr.txt", "r");
  #endif

  for(i = 0; i < 1; ++i)
  { 
    getline(&line, &len, fp_curr);
    if(i==0)
    {
      setCurrent=atoi(line);
    }
  }
}

void readThrParams(int *pageFirstLoad)
{
  int i;
  #ifdef RPI
  fp_thr = fopen("/home/pi/PRESA/data/param_thr.txt", "r");
  #endif
  #ifdef LUKA
  fp_thr = fopen("/home/luka/PRESA/data/param_thr.txt", "r");
  #endif

  if(*pageFirstLoad) 
  {
    for(i = 0; i < 4; ++i)
    { 
      getline(&line, &len, fp_thr);
      
      if(i==0)
      {
        screwdLowThr=atoi(line);
      }
      else if(i==1)
      {
        screwdHighThr=atoi(line);
      }
      else if(i==2)
      {
        dustLowThr=atoi(line);
      }
      else if(i==3)
      {
        dustHighThr=atoi(line);
      }
    }
    *pageFirstLoad = 0;    
  }
}

void readCountParams()
{
  int i;
  #ifdef RPI
  fp_press_count = fopen("/home/pi/PRESA/data/press_count.txt", "r");
  #endif
  #ifdef LUKA
  fp_press_count = fopen("/home/luka/PRESA/data/press_count.txt", "r");
  #endif

  for(i = 0; i < 4; ++i)
  { 
    getline(&line, &len, fp_press_count);
    if(i==0)
    {
      press_count=atoi(line);
    }
  }
  fclose(fp_press_count);
}

void readCountAllParams()
{
  int i;
  #ifdef RPI
  fp_press_count_all = fopen("/home/pi/PRESA/data/press_count_all.txt", "r");
  #endif
  #ifdef LUKA
  fp_press_count_all = fopen("/home/luka/PRESA/data/press_count_all.txt", "r");
  #endif
  for(i = 0; i < 4; ++i)
  { 
    getline(&line, &len, fp_press_count_all);
    if(i==0)
    {
      press_count_all=atoi(line);
    }
  }
  fclose(fp_press_count);
}

void readSensSelectParams(int *pageFirstLoad)
{
  int i;
  if(*pageFirstLoad)
  { 
    #ifdef RPI
    fp_sens_sel = fopen("/home/pi/PRESA/data/param_sens_sel.txt", "r");
    #endif
    #ifdef LUKA
    fp_sens_sel = fopen("/home/luka/PRESA/data/param_sens_sel.txt", "r");
    #endif

    for(i = 0; i < 10; ++i)
    {
      getline(&line, &len, fp_sens_sel);
  
      if(i==0)
      {
        sens_sel[0] = atoi(line);
      }
      else if(i==1)
      {
        sens_sel[1] = atoi(line);
      }
      else if(i==2)
      {
        sens_sel[2] = atoi(line);
      }
      else if(i==3)
      {
        sens_sel[3] = atoi(line);
      }
      else if(i==4)
      {
        sens_sel[4] = atoi(line);
      }
      else if(i==5)
      {
        sens_sel[5] = atoi(line);
      }
      else if(i==6)
      {
        sens_sel[6] = atoi(line);
      }
      else if(i==7)
      {
        sens_sel[7] = atoi(line);
      }
      else if(i==8)
      {
        sens_sel[8] = atoi(line);
      }
      else if(i==9)
      {
        sens_sel[9] = atoi(line);
      }
    }
  }
  *pageFirstLoad = 0;
}

void readDustSelectParams(int *pageFirstLoad)
{
  int i;
  if(*pageFirstLoad)
  { 
    #ifdef RPI
    fp_dust_sel = fopen("/home/pi/PRESA/data/param_dust_sel.txt", "r");
    #endif
    #ifdef LUKA
    fp_dust_sel = fopen("/home/luka/PRESA/data/param_dust_sel.txt", "r");
    #endif

    for(i = 0; i < 10; ++i)
    {
      getline(&line, &len, fp_dust_sel);
  
      if(i==0)
      {
        dust_sel[0] = atoi(line);
      }
      else if(i==1)
      {
        dust_sel[1] = atoi(line);
      }
      else if(i==2)
      {
        dust_sel[2] = atoi(line);
      }
      else if(i==3)
      {
        dust_sel[3] = atoi(line);
      }
      else if(i==4)
      {
        dust_sel[4] = atoi(line);
      }
      else if(i==5)
      {
        dust_sel[5] = atoi(line);
      }
      else if(i==6)
      {
        dust_sel[6] = atoi(line);
      }
      else if(i==7)
      {
        dust_sel[7] = atoi(line);
      }
      else if(i==8)
      {
        dust_sel[8] = atoi(line);
      }
      else if(i==9)
      {
        dust_sel[9] = atoi(line);
      }
    }
  }
  *pageFirstLoad = 0;
}


void checkError()
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
 
void checkSelectP0()
{
  if(readVariableValue("I_1"))
  {
    if(left_button_selected || right_button_selected)
    {
      page = 1;
      sbarText = 5;
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
      writeVariableValue("OutputValue_1_i05", setCurrent); 
    } 
  }
}

void checkSelectP1()
{
    if(selected[0] == 1)
    {
      if(encodeRange(160, 340, 0) == 0  && encodeRange(350, 10, 1) == 0 && checkDoublePress()/* &&checkOil() && checkAir() && checkForceField()*/)
      {
         sbarText = 0;
         page = 7;
      }
    }
  
    else if(selected[1] == 1)
    {
      if(checkDoublePress()/* &&checkOil() && checkAir() && checkForceField()*/)
      {
        sbarText = 1;
        page = 8;
      }
    }

    else if(selected[2] == 1)
    {
      if(encodeRange(160, 340, 0) == 0 && checkDoublePress()/* &&checkOil() && checkAir() && checkForceField()*/)
      {
        sbarText = 2;
        page = 2;
      }
    }
    else if(selected[3] == 1)
    {
      if(encodeRange(160, 340, 0) == 0 && checkDoublePress()/* &&checkOil() && checkAir() && checkForceField()*/)
      {
        page = 2;
        sbarText = 3;
      }
    }
    else
    {
      sbarText = 5;
    }
  
}

void checkStopCycle()
{
  if(readVariableValue("I_3"))
  {
    page = 1;
    sbarText = 5;
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
      checkStopMotor();
      break; 
    
    case 2:
      /*if(readVariableValue("I_6") == 1)    vklopljen reÅim senzorjev
      {*/
      checkError();
      /*}*/

      if(sbarText == 2)
      {
        oneCycle();
      } 
      else if(sbarText == 3)
      {
        checkPress();
        checkStopCycle();
      }   
      break;
    
    case 7:
      oneCycle();
      break;  
    
    case 8:
      checkStopCycle();
      checkFeeder();
      break;
  }
}


void initVars()
{
  int i;
  int count_lab = 1;
  
  for(i = 0; i < 10; ++i)
  {
     #ifdef RPI
     sprintf(fileBuff[i], "/home/pi/PRESA/data/sensor_%d_param.txt", i);
     sprintf(fileBuffRm[i],"rm %s", fileBuff[i]);
     #endif
    
     #ifdef LUKA
     sprintf(fileBuff[i], "/home/luka/PRESA/data/sensor_%d_param.txt", i);
     sprintf(fileBuffRm[i],"rm %s", fileBuff[i]);
     #endif
     
     #ifdef RPI
     sprintf(fileBuffDust[i], "/home/pi/PRESA/data/dust_%d_param.txt", i);
     sprintf(fileBuffRmDust[i],"rm %s", fileBuffDust[i]);
     #endif
    
     #ifdef LUKA
     sprintf(fileBuffDust[i], "/home/luka/PRESA/data/dust_%d_param.txt", i);
     sprintf(fileBuffRmDust[i],"rm %s", fileBuffDust[i]);
     #endif
   
     sprintf(sensorLabels[i], "S%d", count_lab);
     count_lab++;

     sens_sel[i] = 1;
     dust_sel[i] = 1;
  }
  readCurrParams();
  readCountParams();
  readCountAllParams();
}

void timer(int measure)
{
  struct timespec start, stop;
  double accum;
    
  clock_gettime(CLOCK_REALTIME, &start);    
  while(accum < measure)
  {
    clock_gettime(CLOCK_REALTIME, &stop);

    accum = ( stop.tv_sec - start.tv_sec )
     + ( stop.tv_nsec - start.tv_nsec )
     / BILLION;
  }
  /*
  if(accum >= measure)
  {
    return 1;
  }
  else
  {
    return 0;
  } 
  */
}

int encodeRange (int a, int b, int mode)
{
  if(mode==0)
  {
    if(encoder >= a && encoder <= b)
    {
      return 1; 
    }
    else
    {
      return 0;
    }
  }
  else
  {
    if(encoder >= a || encoder <= b)
    {
      return 1; 
    }
    else
    {
      return 0;
    }
  }
}

void checkStressSensor()
{
  if(readVariableValue("I_7"))
  {
    writeVariableValue("O_3", 0);
  }
}

int checkDoublePress()
{
  if(readVariableValue("I_2"))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int checkOil()
{
  if(readVariableValue("I_4"))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int checkAir()
{
  if(readVariableValue("I_5"))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int checkForceField()
{
  if(readVariableValue("I_6"))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void oneCycle()
{
  writeVariableValue("O3", 1);
  if(encodeRange(355, 5, 1))
  {
    writeVariableValue("O3", 0);
    sbarText = 5;
    page = 1;
  }
}

void checkStopMotor()
{
  if(readVariableValue("I_3"))
  {
    page = 0;
    sbarText = 4; 
  }
}

void checkFeeder()
{
  if(readVariableValue("I_10"))
  {
    writeVariableValue("O_10", 1);
  }
  else
  {
    writeVariableValue("O_10", 0);
  }
 
  if(readVariableValue("I_11"))
  {
    writeVariableValue("O_11", 1);
  }
  else
  {
    writeVariableValue("O_10", 0);
  }
}


void feeder()
{
  if(readVariableValue("I_12"))
  {
    if(encodeRange(50,100, 0))
    {
      writeVariableValue("O_7", 1);
    }
    else
    {
      writeVariableValue("O_7", 0);
    }
  }
}

void checkPress()
{
  if(encodeRange(355, 5, 1))
  {
    if(!counted)
    {
      press_count++;
      press_count_all++;
      counted = 1;
    }
  }
  if(encodeRange(300, 354, 0))
  {
    counted = 0;
  }
}
/* fire signal when in position between 160 and 340. else off */
