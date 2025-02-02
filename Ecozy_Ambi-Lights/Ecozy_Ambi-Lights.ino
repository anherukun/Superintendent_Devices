#include <Adafruit_NeoPixel.h>

byte pixelPin = 4;
byte pixelCount = 64;

int inByte;

String inputString = "";        // A string to hold incoming data
boolean stringComplete = false; // Whether the string is complete

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(pixelCount, pixelPin, NEO_GRB + NEO_KHZ800);

void setup()
{
    Serial.begin(115200);
    pixels.begin();
    // Serial.onEvent(Serial_OnEvent,)
}

// the loop function runs over and over again forever
void loop()
{
    if (stringComplete == true)
    {
        Serial.println(inputString);

        delay(250);

        stringComplete = false;
        inputString = "";
    }

    while (Serial.available())
    {
        // inByte = Serial.read();
        byte i_code = Serial.read();
        byte r_code = Serial.read();
        byte g_code = Serial.read();
        byte b_code = Serial.read();

        // pixels.setPixelColor(0, r_code, g_code, b_code);
        pixels.setPixelColor(i_code, r_code, g_code, b_code);
        // pixels.show();

        // char inChar = (char)inByte;

        // if (inChar == '\n')
        //     stringComplete = true;
        // else
        //     inputString += inChar;

        // delay(100);
    }

    // pixels.clear();
    pixels.show();
}