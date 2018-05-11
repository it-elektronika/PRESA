#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "presa.h"
#include "kunbus.h"
#include "graphics_sdl.h"


void readSensors()  /* BRANJE SENZORJEV IN PREVERJANJE NEDOVOLJENJIH VREDNOSTI */
{  
  int i;
  
  encoder = round((readVariableValue("InputValue_3_i05")-substract)/divisor);
 
  /*encoder = readVariableValue("InputValue_3_i05");*/
  /*encoder = 100;*/
  /* IF encoder in right position read values */
  
  sensorsValue[0] = readVariableValue("InputValue_1");      /*  1 */
  sensorsValue[1] = readVariableValue("InputValue_2_i04");      /*  2 */
  sensorsValue[2] = readVariableValue("InputValue_2");      /*  3 */
  sensorsValue[3] = readVariableValue("InputValue_3_i04");      /*  4 */
  sensorsValue[4] = readVariableValue("InputValue_3");  /*  5 */
  sensorsValue[5] = readVariableValue("InputValue_4_i04");  /*  6 */
  sensorsValue[6] = readVariableValue("InputValue_4");  /*  7 */
  sensorsValue[7] = readVariableValue("InputValue_1_i05");  /*  8 */
  sensorsValue[8] = readVariableValue("InputValue_1_i04");  /*  9 */
  sensorsValue[9] = readVariableValue("InputValue_2_i05");  /* 10 */
 
  for(i = 0; i < 10; ++i)
  {
    if(sensorsValue[i] > savedHighThr[i] && encodeRange(screwdLowThr, screwdHighThr, 0) && sens_sel[i] )
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

void readSensParams(int *pageFirstLoad) /* BRANJE SHRANJENIH PARAMETROV DOVOLJENIH VREDNOSTI */
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

void readDustParams(int *pageFirstLoad) /* BRANJE SHRANJENIH PARAMETROV DOVOLJENIH VREDNOSTI - smet na senzorju */
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

void readCurrParams()  /* BRANJE SHRANJENIH PARAMETROV TOKA MOTORJA */
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
  fclose(fp_curr);
}

void readThrParams(int *pageFirstLoad) /* BRANJE SHRANJENIH PARAMETROV O RAZPONU DELOVANJA SENZORJEV */
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

void readThrParamsIntro()
{
  int i;
  #ifdef RPI
  fp_thr = fopen("/home/pi/PRESA/data/param_thr.txt", "r");
  #endif
  #ifdef LUKA
  fp_thr = fopen("/home/luka/PRESA/data/param_thr.txt", "r");
  #endif

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
}

void readCountParams() /* BRANJE SHRANJENIH PODATKOV O STEVILU KOSOV */
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

void readCountAllParams() /* BRANJE SHRANJENIH PODATKOV O STEVILU KOSOV -skupaj*/
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

void readSensSelectParams(int *pageFirstLoad) /* BRANJE SHRANJENIH PARAMETROV O IZBRANIH SENZORJIH*/
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

void readDustSelectParams(int *pageFirstLoad) /* BRANJE SHRANJENIH PARAMETROV O IZBRANIH SENZOJIH -smet na senzorju */
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

void checkError() /* PREGLEDOVANJE NAPAKE ZARADI NEDOVOLJENIH VREDNOSTI SENZORJEV */
{
  int i;
  if(readVariableValue("I_5"))
  {
  
    for(i = 0; i < 4; i++)
    {
      if(sensors[i] == 1)
      {
        errorMode = 1;
        page = 4;
        sbarText = 7;
        
        writeVariableValue("O_9", 1);
        timer(0.2);
        writeVariableValue("O_9", 0);
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
        
        writeVariableValue("O_9", 1);
        timer(0.2);
        writeVariableValue("O_9", 0);
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

void checkSelectP1() /* PREVERJANJE IZBIRE V MENIJU - izbira nacina delovanja */
{
    if(selected[0] == 1)
    {
      if(/*encodeRange(160, 340, 0) == 0  && encodeRange(350, 10, 1) == 0 &&*/ confirmSelection() && checkOil() && checkAir() && checkMotorDriver())
      {
         sbarText = 0;
         page = 7;
      }
    }
  
    else if(selected[1] == 1)
    {
      if(confirmSelection() && checkOil() && checkAir())
      {
        sbarText = 1;
        page = 8;
      }
    }

    else if(selected[2] == 1)
    {
      
      if(encodeRange(160, 330, 0) == 0 && confirmSelection() && checkOil() && checkAir() && checkMotorDriver())
      {
        sbarText = 2;
        page = 2;
      }
    }
    else if(selected[3] == 1)
    {
      if(encodeRange(160, 330, 0) == 0 && confirmSelection() &&checkOil() && checkAir() && checkMotorDriver())
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

void initVars() /* INICIALIZACIJA SPREMENLJIVK */
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
  readThrParamsIntro();
}

void timer(float measure) /* CASOVNI ZAMIK */
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
}

int encodeRange (int a, int b, int mode) /* PREVERJANJE ALI JE VREDNOST ENCODERJA V IZBRANEM RAZPONU */
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

void checkStopCycle() /* STOP TIPKA */
{
  if(readVariableValue("I_4") || stop_step == 1)
  {
    if(stop_step == 0 || stop_step == 1)  
    {
      stop_step = 1;
      stopClutch();
    }
    if(stop_step == 2)
    {
      if(sbarText == 3 && readVariableValue("I_6"))
      {
        writeVariableValue("O_9", 1);
        timer(0.2);
        writeVariableValue("O_9", 0);
      }
      writeVariableValue("O_7", 0);
      writeVariableValue("O_8", 0);
      writeVariableValue("O_10", 0);
      page = 1;
      sbarText = 5;
      stop_step = 0;
      oneCycleStop = 0;
      
      fp_press_count = fopen("/home/pi/PRESA/data/press_count.txt", "w");
      fprintf(fp_press_count, "%lu\n", press_count);
      fclose(fp_press_count);
  
      fp_press_count_all = fopen("/home/pi/PRESA/data/press_count_all.txt", "w");
      fprintf(fp_press_count_all, "%lu\n", press_count_all);
      fclose(fp_press_count_all);
    }
  }
}
 
void checkStartMotor() /* VKLOP MOTORJA */
{
  if(left_button_selected && readVariableValue("I_9")) /* IZBRANA LEVA SMER IN PRITISNJENA TIPKA ZA VKLOP MOTORJA */
  {
    writeVariableValue("O_2", 1);  /* VKLOP MOTORJA V LEVO */ 
    page = 1;
    sbarText = 5;
  } 
  else if(right_button_selected && readVariableValue("I_9")) /* IZBRANA DESNA SMER IN PRITISNJENA TIPKA ZA VKLOP MOTORJA */
  {
    writeVariableValue("O_3", 1);  /* VKLOP MOTORJA V DESNO */    
    page = 1;
    sbarText = 5;
  } 
}

void checkStopMotor() /* IZKLOP MOTORJA */
{
  /* int i;*/
  if(left_button_selected && readVariableValue("I_10")) /* IZBRANA LEVA SMER IN PRITISNJENA TIPKA ZA IZKLOP MOTORJA */
  {
    writeVariableValue("O_2", 0);  /* IZKLOP MOTORJA V LEVO */    
    page = 0;
    sbarText = 22;
    
    renderBackground();
    renderStatusBar();
    renderContent();
    SDL_RenderPresent(renderer);
    writeVariableValue("O_1", 1);
    timer(20);
         
    writeVariableValue("O_1", 0);
    sbarText = 4;

  } 
  else if(right_button_selected && readVariableValue("I_10")) /* IZBRANA DESNA SMER IN PRITISNJENA TIPKA ZA IZKLOP MOTORJA */
  {
    writeVariableValue("O_3", 0);  /* IZKLOP MOTORJA V DESNO */    
    page = 0;
    sbarText = 22;
    
    renderBackground();
    renderStatusBar();
    renderContent();
    SDL_RenderPresent(renderer);
    writeVariableValue("O_1", 1);
    timer(20);
         
    writeVariableValue("O_1", 0);
    sbarText = 4;
  }
}

void startClutch() /* VKLOP SKLOPKE */
{
  writeVariableValue("O_6", 1);
}

void stopClutch() /* IZKLOP SKLOPKE */
{
  if(stop_step == 1)
  {
    if(sbarText == 0 || sbarText == 2 || sbarText == 3)
    {
      if(encodeRange(stop_angle-20, stop_angle, 0))
      {
        writeVariableValue("O_6", 0);
        stop_step = 2;
      }
      else if(oneCycleStop == 1)
      {
        stop_step = 2;
        writeVariableValue("O_6", 0);
      }
      else if(procedure == 0)
      {
        stop_step = 2;
      }
    }
    else
    {
      writeVariableValue("O_6", 0);
      stop_step = 2;
    }
  }
  else
  {
    if(sbarText != 0 || sbarText != 2 || sbarText != 3) 
    {
      writeVariableValue("O_6", 0);
    }
  }
}


void oneCycle()  /* EN DELOVNI CIKEL PRESE */
{
  if(right_button_selected)
  {
    if(encodeRange(stop_angle-20, stop_angle, 0))
    {
      stopClutch();
      oneCycleStop = 1;
      procedure = 0;
      /*
      page = 1;
      sbarText = 5;*/
    }
  }
  else if(left_button_selected)
  {
    if(encoder <= stop_angle+20 && encoder >= stop_angle)
    {
      stopClutch();
      oneCycleStop = 1;
      procedure = 0;
      /*
      page = 1;
      sbarText = 5;*/
    }
  }
}

void feeder() /* PODAJALNIK IZVIJACEV */
{
  if(readVariableValue("I_6"))
  {
    if(encodeRange(340, 1, 1) && feeder_step == 0)
    {
      writeVariableValue("O_7", 1);
      feeder_step = 1;
    }  
    if(readVariableValue("I_7") && feeder_step == 1)
    {
      writeVariableValue("O_7", 0);
      writeVariableValue("O_8", 1);
      feeder_step = 2;
    }
    if(readVariableValue("I_8") && feeder_step == 2)
    {
      writeVariableValue("O_8", 0);
      feeder_step = 0;
    }
  }
}

void blowAir() /* IZPIHOVANJE ZRAKA */
{
  if(encodeRange(300, 55, 1))
  {
    writeVariableValue("O_10", 1);
  }
  else if(encodeRange(56, 299, 0))
  {
    writeVariableValue("O_10", 0);
  }
}

void screwdriverSpring()
{
  if(encodeRange(110, 300, 0))
  {
    writeVariableValue("O_11", 1);
  }
  else if(encodeRange(301, 109, 1))
  {
    writeVariableValue("O_11", 0);
  }
}

void forceFieldBridge()
{
  if(encodeRange(80, 240, 0))
  {
    writeVariableValue("O_5", 1);
  }
  else if(encodeRange(241, 79, 1))
  {
    writeVariableValue("O_5", 0);
  }
}

void checkPress() /* STETJE KOSOV */
{
  if(encodeRange(330, 10, 1))
  {
    if(!counted)
    {
      press_count++;
      press_count_all++;
      counted = 1;
    }
  }
  if(encodeRange(150, 300, 0))
  {
    counted = 0;
  }
}

void checkSelection() /* PREVERJANJE IZBIRE ZARADI VKLOPA RELEJEV */
{
  if(selected[0] == 1 || selected[2] == 1 || selected[3] == 1)
  {
    writeVariableValue("O_4", 1);
    writeVariableValue("O_5", 0);
  }
  else if(selected[1] == 1)
  {
    writeVariableValue("O_4", 0);
    writeVariableValue("O_5", 1);
  }
  else if(selected[0] == 0 || selected[1] == 0 || selected[2] == 0 || selected[3] == 0)
  {
    writeVariableValue("O_4", 0);
    writeVariableValue("O_5", 0);
  }
}

int confirmSelection() /* POTRJEVANJE IZBIRE - start cikla */
{
  if(readVariableValue("I_3"))
  {
    writeVariableValue("O_12", 1);
    timer(2);
    writeVariableValue("O_12", 0);
    return 1;
  }
  return 0;
}



int checkDoublePress() /* DVOROCNI VKLOP */
{
  if(readVariableValue("I_1") == 0 && readVariableValue("I_2"))
  {
    procedure = 1;
    startClutch();
    return 1;
  }
  else
  {
    return 0;
  }
}

void checkStressSensor() /* PREVERJANJE PREOBREMENITVE PRESE */
{
  if(readVariableValue("RTDValue_2_i05") < 1000)
  {
    sbarText = 11;
    stopClutch();
    backgroundColor = 2;
    writeVariableValue("O_3", 0);
    writeVariableValue("O_2", 0);
  }
}

int checkOilError() /* PREVERJANJE NAPAKE OLJA */
{
  if(readVariableValue("RTDValue_2_i04") < 1000)
  { 
    writeVariableValue("O_9", 1);
    timer(0.2);
    writeVariableValue("O_9", 0);
    sbarText = 18;
    backgroundColor = 2;
    stopClutch();
    return 1;
  }
  else
  {
    return 0;
  }
}

int checkOil() /* PREVERJANJE OLJA */
{
  if(readVariableValue("RTDValue_2_i04") < 1000)
  {
    sbarText = 18;
    return 0;
  }
  else
  {
    return 1;
  }

}

int checkAirError() /* PREVERJANJE NAPAKE ZRAKA */
{
  if(readVariableValue("RTDValue_1_i05") < 1000)
  {
    writeVariableValue("O_9", 1);
    timer(0.2);
    writeVariableValue("O_9", 0);
    sbarText = 19;
    backgroundColor = 2;
    stopClutch();
    return 1;
  }
  else
  {
    return 0;
  }
}

int checkAir() /* PREVERJANJE ZRAKA */
{
  if(readVariableValue("RTDValue_1_i05") < 1000)
  {
    sbarText = 19;
    return 0;
  }
  else
  {
    return 1;
  }
}

int checkForceField() /* VARNOSTNA ZAVESA */
{
  if(readVariableValue("I_11"))
  {
    return 1;
  }
  else if(delay_stop == 1 && encodeRange(241, 79, 1))
  {
    writeVariableValue("O_9", 1);
    timer(0.2);
    writeVariableValue("O_9", 0);   
    stopClutch();
    sbarText = 13;
    backgroundColor = 2;
    hold_red = 1;
    delay_stop = 0;
    return 1;
  }
  else
  {
    if(encodeRange(80, 240, 0) == 0)
    {
      delay_stop = 1; 
      backgroundColor = 2;
      hold_red = 1;
      return 0;
    }
    else if(encodeRange(241, 79, 1))
    { 
      writeVariableValue("O_9", 1);
      timer(0.2);
      writeVariableValue("O_9", 0);
      stopClutch();
      sbarText = 13;
      backgroundColor = 2;
      hold_red = 1;
      return 0;
    }
    return 0;
  }
}

int checkZeroPosition()  /* NICELNA TOCKA */
{
  if(readVariableValue("I_12") == 0 && encodeRange(31, 331, 0))
  {
    writeVariableValue("O_9", 1);
    timer(0.2);
    writeVariableValue("O_9", 0);
    stopClutch();
    sbarText = 17;
    backgroundColor = 2;
    return 1;
  }
  return 0;
}

int checkFeederIntro()  /* horizontalni pomik */
{
  if(readVariableValue("I_14") == 0)
  {
    writeVariableValue("O_9", 1);
    timer(0.2);
    writeVariableValue("O_9", 0);
    stopClutch();
    sbarText = 20;
    backgroundColor = 2;
    return 1;
  }
  return 0;
}

int checkFeederLift()  /* vertikalni TOCKA */
{
  if(readVariableValue("I_13") == 0 && encodeRange(350, 20, 1))
  { 
    writeVariableValue("O_9", 1);
    timer(0.2);
    writeVariableValue("O_9", 0);
    stopClutch();
    sbarText = 21;
    backgroundColor = 2;
    return 1;
  }
  return 0;
}


int checkStopTotal() /*STOP TOTAL - goba */
{
  if(readVariableValue("RTDValue_1") < 1000)
  {
    writeVariableValue("O_9", 1);
    timer(0.2);
    writeVariableValue("O_9", 0);
    stopClutch();
    sbarText = 14;
    backgroundColor = 2;
   
    return 1;
  }
  return 0;
}

int checkMotorDriverError() /* PREVERJANJE NAPAKE DRIVERJA MOTORJA */
{
  if(readVariableValue("RTDValue_2") < 1000)
  {
    writeVariableValue("O_9", 1);
    timer(0.2);
    writeVariableValue("O_9", 0);
    stopClutch();
    writeVariableValue("O_2", 0);
    writeVariableValue("O_3", 0);
    sbarText = 15;
    backgroundColor = 2;
   
    return 1;
  }
  return 0;
}

int checkMotorDriver() /* PREVERJANJE DRIVERJA MOTORJA */
{
  if(readVariableValue("RTDValue_2") < 1000)
  {
    stopClutch();
    writeVariableValue("O_2", 0);
    writeVariableValue("O_3", 0);
    sbarText = 15;
    backgroundColor = 2;
    return 0;
  }
  return 1;
}

int checkClutchError() /* PREVERJANJE NAPAKE SKLOPKE */
{
  if(readVariableValue("RTDValue_1_i04") < 1000)
  {
    writeVariableValue("O_9", 1);
    timer(0.2);
    writeVariableValue("O_9", 0);
    stopClutch();
    sbarText = 16;
    backgroundColor = 2;
    return 1;
  }
  return 0;
}

void logicTree() /* DELOVNA LOGIKA PO STRANEH (nekaj je tudi v graphics filetu) */
{
  switch(page)
  {
    case 0:
      if(readVariableValue("RTDValue_2") > 1000)
      {
        checkStartMotor();
        writeVariableValue("OutputValue_1_i05", setCurrent);
      }
      else
      {
        sbarText = 15;
        backgroundColor = 2;
      }
      break;

    case 1:
      sbarText = 5;
      checkSelectP1(); 
      checkStopMotor();
      checkSelection();
      if(selected[1])
      {
        screwdriverSpring();
      }
      procedure = 0;
      hold_red = 0;
      if(readVariableValue("RTDValue_2") < 1000)
      {
        page = 0;
        writeVariableValue("O_2", 0);
        writeVariableValue("O_3", 0);
      }
      break; 
    
    case 2:
      if(sbarText == 3 || sbarText == 2 )
      {
        checkDoublePress();
      }
      if(procedure)
      {
        if(hold_red != 1)
        {
          backgroundColor = 0;
        }
        checkError();
        checkForceField();
        checkMotorDriverError();
        checkClutchError();
        checkZeroPosition();
        checkOilError();
        checkAirError();
        checkStopTotal();
        checkFeederIntro(); 
        checkFeederLift(); 
        
        if(sbarText == 2)
        {
          oneCycle();
          checkPress();
          feeder();
          blowAir();
          screwdriverSpring();
          forceFieldBridge();
        } 
        else if(sbarText == 3)
        {
          checkPress();
          feeder();
          blowAir();
          screwdriverSpring();
          forceFieldBridge();
        }   
        checkStopCycle();
      }
      checkStopCycle();
      break;

    case 4:
      stopClutch();
      checkStopCycle();
      break;

    case 7:
      checkDoublePress();
      checkStopCycle();
      if(procedure)
      {
        encoder = round((readVariableValue("InputValue_3_i05")-substract)/divisor);
        checkForceField();
        oneCycle();
        checkStopCycle();
      }
      break;  
    
    case 8:
      if(procedure)
      {
        checkStopCycle();
      }
      checkStopCycle();
      break;
  }
}






