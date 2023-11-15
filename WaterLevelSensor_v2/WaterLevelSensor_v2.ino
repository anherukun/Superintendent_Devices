/*
  Keyboard Message test

  For the Arduino Leonardo and Micro.

  Sends a text string when a button is pressed.

  The circuit:
  - pushbutton attached from pin 0 to ground
  - 10 kilohm resistor attached from pin 0 to +5V

  created 24 Oct 2011
  modified 27 Mar 2012
  by Tom Igoe
  modified 11 Nov 2013
  by Scott Fitzgerald

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/KeyboardMessage
*/
#if ARDUINO_USB_CDC_ON_BOOT
// #warning This sketch should be used when USB is in OTG mode
// #include "HID.h"
// #include "Keyboard.h"
#include <Arduino.h>
#include "WifiService.h"
#include "TimeService.h"

const char *SSID = "XIAO_WAT_32C";
const char *PASSWORD = "";

void setup()
{
    Serial.begin(115200);
    StartWifiService(SSID, PASSWORD);
    StartTimeService();
}

void loop()
{
    // if (WiFiManager.isConnected())
    // {
    //     Serial.print("Wifi Connected: ");
    //     Serial.print(WiFi.getHostname());
    //     Serial.print(" | ");
    //     Serial.println(WiFi.localIP());
    //     delay(1000);
    // }
    delay(1000);
    // printTime();
    Serial.println("Time: " + GetTime());
}
#endif