#include <Adafruit_NeoPixel.h>

#define PIN 1
#define NUMPIXELS 11
#define MULTIPLY 3
#define DELAYVAL 250

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
uint32_t rgbcolors[NUMPIXELS * MULTIPLY];

void setup()
{
  pixels.begin();
  pixels.clear();

  for (int i = 0; i < NUMPIXELS * MULTIPLY; i++)
  {
    int hue = map(i, 0, NUMPIXELS * MULTIPLY, 0, 65536);
    rgbcolors[i] = pixels.ColorHSV(hue);
  }

  // for (int i = 0; i < NUMPIXELS; i++)
  // {
  //   pixels.setPixelColor(i, rgbcolors[i]);
  // }
  // pixels.show();
}

void loop()
{
  pixels.setPixelColor((int)random(0, NUMPIXELS), rgbcolors[(uint32_t)random(0, NUMPIXELS * MULTIPLY) - 1]);
  delay(DELAYVAL);
  pixels.show();
  // for (int step = 0; step < MULTIPLY; step++)
  // {
  //   for (int i = 0; i < NUMPIXELS; i++)
  //   {
  //     pixels.setPixelColor(i, rgbcolors[i * step]);
  //     delay(DELAYVAL);
  //     pixels.show();
  //   }
  // }

  // for(int i = 0; i < NUMPIXELS * 2; i++)
  // {
  //   int hue = map(i, 0, NUMPIXELS * 2, 0, 65536);
  //   uint32_t rgbcolor = pixels.ColorHSV(hue);
  //   if (i > NUMPIXELS) {
  //     pixels.setPixelColor(i - NUMPIXELS, rgbcolor);
  //
  //   } else {
  //     pixels.setPixelColor(i, rgbcolor);
  //
  //   }
  //   delay(DELAYVAL);
  //   pixels.show();
  // }
}
