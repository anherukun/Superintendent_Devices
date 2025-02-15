#include <Adafruit_NeoPixel.h>

#define RX_BUF_SIZE 512

byte pixelPin = 4;
byte pixelCount = 32;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(pixelCount, pixelPin, NEO_GRB + NEO_KHZ800);

byte i_count = 0;

void setup()
{
    Serial.setRxBufferSize(RX_BUF_SIZE);
    Serial.begin(10000);
    pixels.begin();

    pixels.setBrightness((255 / 3) * 2);
    pixels.clear();
    pixels.show();
}

// the loop function runs over and over again forever
void loop()
{
    method_1();
}

void method_1()
{
    byte i_code = 0;

    if (Serial.available())
    {
        i_code = Serial.read();
        byte r_code = Serial.read();
        byte g_code = Serial.read();
        byte b_code = Serial.read();

        pixels.setPixelColor(i_code, r_code, g_code, b_code);
    }

    if (i_code == pixelCount - 1){
        pixels.show();
        Serial.flush();
    }
}