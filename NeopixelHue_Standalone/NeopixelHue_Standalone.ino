#include <Adafruit_NeoPixel.h>

#define PIN 1
#define NUMPIXELS 11
#define DELAYVAL 100 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  pixels.clear();

  for(int i = 0; i < NUMPIXELS; i++) 
    { 
      if (i == NUMPIXELS -1) {
        
      }
      int hue = map(i, 0, NUMPIXELS, 0, 65536);
      uint32_t rgbcolor = pixels.ColorHSV(hue);

      pixels.setPixelColor(i, rgbcolor);
    }
    pixels.show();
}

void loop() {
  for(int i = 0; i < NUMPIXELS * 2; i++) 
  { 
    int hue = map(i, 0, NUMPIXELS * 2, 0, 65536);
    uint32_t rgbcolor = pixels.ColorHSV(hue);
    if (i > NUMPIXELS) {
      pixels.setPixelColor(i - NUMPIXELS, rgbcolor);

    } else {
      pixels.setPixelColor(i, rgbcolor);

    }
    delay(DELAYVAL);
    pixels.show();
  }
}
