#include <TFT_eSPI.h>

#ifndef CONFIG
#define CONFIG

extern TFT_eSPI display;

/**
 * Set this to false to disable Serial logging
 */
#define DEBUG true

/**
 * The name of this device (as defined in the AWS IOT console).
 * Also used to set the hostname on the network
 */
#define DEVICE_NAME "power-pod-nano"

/**
 * ADC input pin that is used to read out the CT sensor
 */
#define ADC_INPUT 36

/**
 * The voltage of your home, used to calculate the wattage.
 * Try setting this as accurately as possible.
 */
#define HOME_VOLTAGE 230.0

/**
 * WiFi credentials
 */
#define WIFI_NETWORK "Your WiFi SSID"
#define WIFI_PASSWORD "Your WiFi password"

/**
 * Timeout for the WiFi connection. When this is reached,
 * the ESP goes into deep sleep for 30seconds to try and
 * recover.
 */
#define WIFI_TIMEOUT 30000 // 20 seconds

/**
 * How long should we wait after a failed WiFi connection
 * before trying to set one up again.
 */
#define WIFI_RECOVER_TIME_MS 30000 // 20 seconds

/**
 * Dimensions of the OLED display attached to the ESP
 */
#define SCREEN_WIDTH 135
#define SCREEN_HEIGHT 240

/**
 * Force Emonlib to assume a 3.3V supply to the CT sensor
 */
#define emonTxV3 1


/**
 * Local measurements
 */
#define LOCAL_MEASUREMENTS 30


/**
 * The MQTT endpoint of the service we should connect to and receive messages
 * from.
 */
#define AWS_ENABLED true
#define AWS_IOT_ENDPOINT "*** YOUR AWS IOT ENDPOINT ***"
#define AWS_IOT_TOPIC "*** YOUR AWS IOT TOPIC ***"

#define MQTT_CONNECT_DELAY 200
#define MQTT_CONNECT_TIMEOUT 20000 // 20 seconds

/**
 * Battery level monitoring
 */
#define TASK_MONITOR_BATTERY_ENABLED true
#define BATT_UPDATE_INTERVAL_MS 60000
#define MIN_USB_VOL 4.9

#define ICON_WIDTH 36
#define ICON_HEIGHT 18
#define STATUS_HEIGHT_BAR ICON_HEIGHT
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define ICON_POS_X (display.width() - ICON_WIDTH)-5

/**
 * Syncing time with an NTP server
 */
#define NTP_TIME_SYNC_ENABLED true
#define NTP_SERVER "pool.ntp.org"
// +8 UTC time zone
#define NTP_OFFSET_SECONDS 3600 * 8
#define NTP_UPDATE_INTERVAL_MS 60000

/**
 * Wether or not you want to enable Home Assistant integration
 */
#define HA_ENABLED false
#define HA_ADDRESS "*** YOUR HOME ASSISTANT IP ADDRESSS ***"
#define HA_PORT 8883
#define HA_USER "*** MQTT USER ***"
#define HA_PASSWORD "*** MQTT PASSWORD ***"

// Check which core Arduino is running on. This is done because updating the 
// display only works from the Arduino core.
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#endif