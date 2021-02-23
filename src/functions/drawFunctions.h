#ifndef DRAW_FUNCTIONS
#define DRAW_FUNCTIONS

#include <WiFi.h>
#include <TFT_eSPI.h>
#include "../config/enums.h"
#include "../config/config.dist.h"

extern TFT_eSPI display;
extern DisplayValues gDisplayValues;
extern unsigned char measureIndex;
char *batteryImages[] = {"/battery_01.jpg", "/battery_02.jpg", "/battery_03.jpg", "/battery_04.jpg", "/battery_05.jpg"};


void drawTime(){
  // display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(gDisplayValues.time);
}

void drawSignalStrength(){
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  const byte X = SCREEN_WIDTH - 13;
  const byte X_SPACING = 2;

  // Draw the four base rectangles
  display.fillRect(X, 8-2, 1, 2, TFT_WHITE); // Bar 1
  display.fillRect(X + X_SPACING, 8-2, 1, 2, TFT_WHITE); // Bar 2
  display.fillRect(X + X_SPACING*2, 8-2, 1, 2, TFT_WHITE); // Bar 3
  display.fillRect(X + X_SPACING*3, 8-2, 1, 2, TFT_WHITE); // Bar 4

  // Draw bar 2
  if(gDisplayValues.wifi_strength > -70){
    display.fillRect(X+X_SPACING, 8-4, 1, 4, TFT_WHITE);
  }

  // Draw bar 3
  if(gDisplayValues.wifi_strength > -60){
    display.fillRect(X+X_SPACING*2, 8-6, 1, 6, TFT_WHITE);
  }

  // Draw bar 4
  if(gDisplayValues.wifi_strength >= -50){
    display.fillRect(X+X_SPACING*3, 8-8, 1, 8, TFT_WHITE);
  }
}

void drawMeasurementProgress(){
  const byte Y = SCREEN_HEIGHT - 20;
  display.drawRect(0, Y, measureIndex*4, 3, TFT_WHITE);
}

void drawingBatteryIcon(String filePath){
   TJpgDec.drawFsJpg(ICON_POS_X, 0, filePath);
}

void drawBatteryText(String text){
  display.setTextSize(1);
  display.setCursor(ICON_POS_X-(text.length()*6), 0);
  display.print(text);
}


void drawBatteryStatus(){

  if(gDisplayValues.battVolts >= MIN_USB_VOL){
    drawingBatteryIcon(batteryImages[4]);
    drawBatteryText("chrg");
  }else{
        int imgNum = 0;
        int batteryLevel = gDisplayValues.battChargeLevel;
        if(batteryLevel >=80){
          imgNum = 3;
        }else if(batteryLevel < 80 && batteryLevel >= 50 ){
          imgNum = 2;
        }else if(batteryLevel < 50 && batteryLevel >= 20 ){
          imgNum = 1;
        } 

        drawingBatteryIcon(batteryImages[imgNum]);
        drawBatteryText(String(batteryLevel) + "%");
        // vTaskDelay(1000);
    }
}

/**
 * The screen that is displayed when the ESP has just booted
 * and is connecting to WiFi & AWS.
 */
void drawBootscreen(){
  byte X = (SCREEN_WIDTH / 2) - 15;
  byte Y = SCREEN_HEIGHT / 2;
  byte WIDTH = 6;
  byte MAX_HEIGHT = 35;
  byte HEIGHT_STEP = 10;
  byte X_SPACING = 10;

  display.fillRect(X              , Y, WIDTH, MAX_HEIGHT - HEIGHT_STEP*3, TFT_WHITE);
  display.fillRect(X + X_SPACING  , Y - HEIGHT_STEP, WIDTH, MAX_HEIGHT - HEIGHT_STEP*2, TFT_WHITE);
  display.fillRect(X + X_SPACING*2, Y - HEIGHT_STEP*2, WIDTH, MAX_HEIGHT - HEIGHT_STEP, TFT_WHITE);
  display.fillRect(X + X_SPACING*3, Y - HEIGHT_STEP*3, WIDTH, MAX_HEIGHT, TFT_WHITE);

  display.setTextSize(1);
  display.setCursor(X-25, Y + MAX_HEIGHT / 2);
  display.print("Connecting");

  if(gDisplayValues.currentState == CONNECTING_WIFI){
    display.println(" WiFi");
  }

  if(gDisplayValues.currentState == CONNECTING_AWS){
    display.println(" AWS");
  }
}

/**
 * Draw the current amps & watts in the middle of the display.
 */
void drawAmpsWatts(){

  String watts = String(gDisplayValues.watt, 0);
  String amps = String(gDisplayValues.amps, 2);
  
  String lblWatts = "Watts";
  String lblAmps = "Amps";

  const int startY = SCREEN_HEIGHT / 4;

  // Calculate how wide (pixels) the text will be once rendered.
  // Each character = 6 pixels, with font size 2, that is 12 pixels.
  // -1 because of the spacing between letters (last one doesn't)
  int widthAmps = (amps.length() * 12) -1;
  int widthLblAmps = lblAmps.length() * 6 - 1;

  int widthWatts = watts.length() * 12 - 1;
  int widthLblWatts = lblWatts.length() * 6 -1;
  int alignmentFix = 3;

  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.setTextSize(3);
  display.setCursor((SCREEN_WIDTH - widthAmps) / alignmentFix, startY);
  display.print(amps);

  display.setTextSize(2);
  display.setCursor((SCREEN_WIDTH - widthLblAmps) / alignmentFix, startY + 30);
  display.print(lblAmps);

  display.setTextSize(3);
  display.setCursor((SCREEN_WIDTH - widthWatts) / alignmentFix, startY + 65);
  display.print(watts);

  display.setTextSize(2);
  display.setCursor((SCREEN_WIDTH - widthLblWatts) / alignmentFix, startY + 90);
  display.print(lblWatts);
}

#endif