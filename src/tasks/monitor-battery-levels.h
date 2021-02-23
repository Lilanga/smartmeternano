#ifndef TASK_MONITOR_BATTERY
#define TASK_MONITOR_BATTERY

#if TASK_MONITOR_BATTERY_ENABLED == true
    #include <Pangodream_18650_CL.h>
    #include "../config/enums.h"

    #define ADC_PIN 34
    #define CONV_FACTOR 1.8
    #define READS 20

    Pangodream_18650_CL Battery(ADC_PIN, CONV_FACTOR, READS);

    void retrieveBatteryLevels(void * parameter){
        for(;;){
            serial_println("[BATT] Updating...");
            gDisplayValues.battVolts = Battery.getBatteryVolts();
            gDisplayValues.battChargeLevel = Battery.getBatteryChargeLevel(); 
            serial_println("[BATT] Done");

            // Sleep for a minute before checking again
            vTaskDelay(BATT_UPDATE_INTERVAL_MS / portTICK_PERIOD_MS);
        }
    }
#endif
#endif
