#ifndef TASK_UPDATE_DISPLAY
#define TASK_UPDATE_DISPLAY

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "functions/drawFunctions.h"
#include "../config/config.dist.h"

extern TFT_eSPI display;
extern DisplayValues gDisplayValues;

/**
 * Metafunction that takes care of drawing all the different
 * parts of the display (or not if it's turned off).
 */
void updateDisplay(void * parameter){
  for (;;){
    serial_println(F("[LCD] Updating..."));
    // display.clearDisplay();
    display.fillScreen(TFT_BLACK);

    if(gDisplayValues.currentState == CONNECTING_WIFI || 
        gDisplayValues.currentState == CONNECTING_AWS){
      drawBootscreen();
    }

    if(gDisplayValues.currentState == CONNECTED){
      display.fillScreen(TFT_BLACK);
      gDisplayValues.currentState = UP;
    }
    
    if(gDisplayValues.currentState == UP){
      drawTime();
      drawSignalStrength();
      drawAmpsWatts();
      drawMeasurementProgress();
      drawBatteryStatus();
    }

    // display.display();

    // Sleep for 2 seconds, then update display again!
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

#endif
