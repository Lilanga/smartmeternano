#include <Arduino.h>
#include "EmonLib.h"
#include "WiFi.h"
#include <driver/adc.h>
#include "config/config.dist.h"
#include "config/enums.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

#include "tasks/updateDisplay.h"
#include "tasks/fetch-time-from-ntp.h"
#include "tasks/mqtt-aws.h"
#include "tasks/wifi-connection.h"
#include "tasks/wifi-update-signalstrength.h"
#include "tasks/measure-electricity.h"
#include "tasks/mqtt-home-assistant.h"
#include "tasks/monitor-battery-levels.h"

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
TFT_eSPI display(SCREEN_WIDTH, SCREEN_HEIGHT); // Invoke custom library
DisplayValues gDisplayValues;
EnergyMonitor emon1;

// Place to store local measurements before sending them off to AWS
unsigned short measurements[LOCAL_MEASUREMENTS];
unsigned char measureIndex = 0;


void SPIFFSInit(){
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    serial_println(F("[SPIFFS] initialisation failed!"));
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println(F("[SPIFFS] Initialisation done.!"));
}

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  if ( y >= display.height() ) return 0;
  display.pushImage(x, y, w, h, bitmap);
  return 1;
}

void DisplayInit(){
    display.init();
    // display.setRotation(1);
    display.fillScreen(TFT_BLACK);
    display.setTextSize(2);
    // display.setTextColor(TFT_GREEN);
    display.setCursor(0, 0);
    display.setTextDatum(MC_DATUM);
    display.setTextSize(1);
    
    display.setRotation(4);
    // display.setTextSize(1);
    
    display.setTextColor(TFT_WHITE);
    display.setTextWrap(false);

    TJpgDec.setJpgScale(1);
    TJpgDec.setSwapBytes(true);
    TJpgDec.setCallback(tft_output);
}

void setup()
{
  #if DEBUG == true
    Serial.begin(115200);
  #endif 

  // Setup the ADC
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
  analogReadResolution(ADC_BITS);
  pinMode(ADC_INPUT, INPUT);
  
  // Initialize storeage
  SPIFFSInit();

  // Initialize the display
    DisplayInit();

  // Initialize emon library
  emon1.current(ADC_INPUT, 30);

  // ----------------------------------------------------------------
  // TASK: Connect to WiFi & keep the connection alive.
  // ----------------------------------------------------------------
  xTaskCreatePinnedToCore(
    keepWiFiAlive,
    "keepWiFiAlive",  // Task name
    5000,            // Stack size (bytes)
    NULL,             // Parameter
    1,                // Task priority
    NULL,             // Task handle
    ARDUINO_RUNNING_CORE
  );

  // ----------------------------------------------------------------
  // TASK: Connect to AWS & keep the connection alive.
  // ----------------------------------------------------------------
  #if AWS_ENABLED == true
    xTaskCreate(
      keepAWSConnectionAlive,
      "MQTT-AWS",      // Task name
      5000,            // Stack size (bytes)
      NULL,             // Parameter
      5,                // Task priority
      NULL              // Task handle
    );
  #endif

  // ----------------------------------------------------------------
  // TASK: Update the display every second
  //       This is pinned to the same core as Arduino
  //       because it would otherwise corrupt the OLED
  // ----------------------------------------------------------------
  xTaskCreatePinnedToCore(
    updateDisplay,
    "UpdateDisplay",  // Task name
    10000,            // Stack size (bytes)
    NULL,             // Parameter
    3,                // Task priority
    NULL,             // Task handle
    ARDUINO_RUNNING_CORE
  );

  // ----------------------------------------------------------------
  // Task: measure electricity consumption ;)
  // ----------------------------------------------------------------
  xTaskCreate(
    measureElectricity,
    "Measure electricity",  // Task name
    5000,                  // Stack size (bytes)
    NULL,                   // Parameter
    4,                      // Task priority
    NULL                    // Task handle
  );

  // ----------------------------------------------------------------
  // TASK: update time from NTP server.
  // ----------------------------------------------------------------
  #if NTP_TIME_SYNC_ENABLED == true
    xTaskCreate(
      fetchTimeFromNTP,
      "Update NTP time",
      5000,            // Stack size (bytes)
      NULL,             // Parameter
      1,                // Task priority
      NULL              // Task handle
    );
  #endif

  // ----------------------------------------------------------------
  // TASK: update battery status.
  // ----------------------------------------------------------------
  #if TASK_MONITOR_BATTERY_ENABLED == true
    xTaskCreate(
      retrieveBatteryLevels,
      "Update battery status",
      2048,            // Stack size (bytes)
      NULL,             // Parameter
      1,                // Task priority
      NULL              // Task handle
    );
  #endif

  // ----------------------------------------------------------------
  // TASK: update WiFi signal strength
  // ----------------------------------------------------------------
  xTaskCreate(
    updateWiFiSignalStrength,
    "Update WiFi strength",
    1000,             // Stack size (bytes)
    NULL,             // Parameter
    2,                // Task priority
    NULL              // Task handle
  );

  #if HA_ENABLED == true
    xTaskCreate(
      HADiscovery,
      "MQTT-HA Discovery",  // Task name
      5000,                // Stack size (bytes)
      NULL,                 // Parameter
      5,                    // Task priority
      NULL                  // Task handle
    );

    xTaskCreate(
      keepHAConnectionAlive,
      "MQTT-HA Connect",
      5000,
      NULL,
      4,
      NULL
    );
  #endif
}

void loop()
{
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}