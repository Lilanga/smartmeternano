# Smart meter nano

Smart meter nano is a tiny smart meter solution to measure electricy power consumption in a building.

Device is designed to be installed in a DIN rail box and is powered by a 5V USB power supply or a 5V power supply from the DIN rail box. We are using Emonlib to measure the power consumption and a ESP32 micro controller to send the data to a MQTT broker. Here we are using AWS IoT Core as MQTT broker. The device is also capable of sending data to Home Assistant MQTT broker. Firmware is written utilizing FreeRTOS.

## Hardware

Here we are using Lilygo TTGO T-Display ESP32 micro controller. It has a 1.14 inch LCD display and a 5V USB power supply. It is also possible to power the device with a 5V power supply from the DIN rail box. We are utilizing the onboard LED to display measurements and connection status to MQTT brokers.

## Configuration

Configuration is done by editing the `config.h` file. You need to set the following parameters:

* `WIFI_NETWORK`: SSID of the WiFi network
* `WIFI_PASSWORD`: Password of the WiFi network
* `AWS_IOT_ENDPOINT`: AWS IoT Core endpoint address
* `AWS_IOT_TOPIC`: AWS IoT Core topic to publish the data

### Home Assistant

If you are using Home Assistant you can use the configuration definde in config.dist.h to enable Home Assistant mqtt support. You can turn off AWS IoT core data forwarding by setting `AWS_IOT_ENABLED` to `false`. You can also turn off Home Assistant mqtt support by setting `HOME_ASSISTANT_ENABLED` to `false`.

## Iot Core certificates

To communicate with AWS IoT Core you need to create a thing and download the certificates.
In the `certificates` folder you need to place the following certificate files:

* `amazonrootca1.pem`
* `certificate.pem.crt`
* `private.pem.key`
