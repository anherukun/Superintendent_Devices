#include <Adafruit_NeoPixel.h>

#define RX_BUF_SIZE 1024

byte pixelPin = 4;
byte pixelCount = 128;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(pixelCount, pixelPin, NEO_GRB + NEO_KHZ800);

void setup()
{
    Serial.setRxBufferSize(RX_BUF_SIZE);
    Serial.begin(9600);
    pixels.begin();

    // pixels.setBrightness(255 / 2);
}

// the loop function runs over and over again forever
void loop()
{
    method_1();
    delay(1);
    Serial.flush();
}

void method_1()
{
    if (Serial.available())
    {
        // inByte = Serial.read();
        int i_code = Serial.read();
        byte r_code = Serial.read();
        byte g_code = Serial.read();
        byte b_code = Serial.read();

        pixels.setPixelColor(i_code, r_code, g_code, b_code);
        pixels.show();
    }
}