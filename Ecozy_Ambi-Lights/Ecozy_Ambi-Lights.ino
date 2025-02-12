#include <Adafruit_NeoPixel.h>

#define RX_BUF_SIZE 2048

byte pixelPin = 4;
byte pixelCount = 128;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(pixelCount, pixelPin, NEO_GRB + NEO_KHZ800);

byte pCounter = 0;
byte subCounter = -1;

byte rColor = 0;
byte gColor = 0;
byte bColor = 0;

void setup()
{
    Serial.setRxBufferSize(RX_BUF_SIZE);
    Serial.begin(9600);
    pixels.begin();

    pixels.setBrightness((255 / 3) * 2);
}

// the loop function runs over and over again forever
void loop()
{
    // method_1();
    method_2();
    // delay(1);
    Serial.flush();
}

void method_1()
{
    if (Serial.available())
    {
        // inByte = Serial.read();
        byte i_code = Serial.read();
        byte r_code = Serial.read();
        byte g_code = Serial.read();
        byte b_code = Serial.read();

        pixels.setPixelColor(i_code, r_code, g_code, b_code);
        pixels.show();
    }
}

void method_2()
{
    if (Serial.available())
    {
        subCounter++;

        switch (subCounter)
        {
        case 0:
            rColor = Serial.read();
            break;
        case 1:
            gColor = Serial.read();
            break;
        case 2:
            bColor = Serial.read();
            break;
        case 3:
            pixels.setPixelColor(pCounter, rColor, gColor, bColor);
            pCounter += 1;
            subCounter = -1;

            if (pCounter >= pixelCount) {
                pixels.show();
                pCounter = 0;
            }
            break;
        }
    }

}