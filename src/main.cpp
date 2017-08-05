#include <Arduino.h>
// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

// #include <Adafruit_GFX.h>
// #include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
// #ifndef PSTR
//  #define PSTR // Make Arduino Due happy
// #endif

#define PIN 13
#define N_LEDS 12
uint32_t color;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// static void chase(uint32_t c) {
//   for(uint16_t i=0; i<strip.numPixels()+4; i++) {
//     strip.setPixelColor(i  , c); // Draw new pixel
//     strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
//     strip.show();
//     delay(20);
//   }
// }

void setup() {
  strip.begin();
}

void loop() {
  // color = strip.Color(random(0,254), random(0,254), random(0,254));
  // chase(color); // Red
  color = strip.Color(66, 170, 255);
  for (size_t i = 0; i < 13; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();

  //
  // int count = 256;
  // for (size_t i = 0; i < count; i++) {
  //   for (size_t j = 0; j < count; j++) {
  //     for (size_t k = 0; k < count; k++) {
  //       chase(strip.Color(i, j, k)); // Red
  //     }
  //   }
  // }

}
