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
uint32_t randomColor;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

static void chase(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
    strip.setPixelColor(i  , c); // Draw new pixel
    strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
    strip.show();
    delay(20);
  }
}

void setup() {
  strip.begin();
}

void loop() {
  randomColor = strip.Color(random(0,254), random(0,254), random(0,254));
  chase(randomColor); // Red
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




// #ifndef UNIT_TEST
// #include <Arduino.h>
// #endif

// #include <../lib/IRremoteESP8266/src/IRremoteESP8266.h>
// #include <../lib/IRremoteESP8266/src/IRsend.h>
//
// // Codes are in Global Cache format less the emitter ID and request ID.
// // These codes can be found in GC's Control Tower database.
//
// uint16_t Samsung_power_toggle[71] = {
//     38000, 1, 1, 170, 170, 20, 63, 20, 63, 20, 63, 20, 20, 20, 20, 20, 20, 20,
//     20, 20, 20, 20, 63, 20, 63, 20, 63, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
//     20, 20, 20, 63, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 63, 20,
//     20, 20, 63, 20, 63, 20, 63, 20, 63, 20, 63, 20, 63, 20, 1798};
//
// IRsend irsend(4);  // An IR LED is controlled by GPIO pin 4 (D2)
//
// void setup() {
//   irsend.begin();
//   Serial.begin(115200);
// }
//
// void loop() {
//   Serial.println("Toggling power");
//   irsend.sendGC(Samsung_power_toggle, 71);
//   delay(10000);
// }

/*
Example for different sending methods
   https://github.com/sui77/rc-switch/
*/
// #include <../lib/rc-switch/RCSwitch.h>
// RCSwitch mySwitch = RCSwitch();
//
//
// static const char* bin2tristate(const char* bin);
// static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength);
//
// void output(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol) {
//
//   if (decimal == 0) {
//     Serial.print("Unknown encoding.");
//   } else {
//     const char* b = dec2binWzerofill(decimal, length);
//     Serial.print("Decimal: ");
//     Serial.print(decimal);
//     Serial.print(" (");
//     Serial.print( length );
//     Serial.print("Bit) Binary: ");
//     Serial.print( b );
//     Serial.print(" Tri-State: ");
//     Serial.print( bin2tristate( b) );
//     Serial.print(" PulseLength: ");
//     Serial.print(delay);
//     Serial.print(" microseconds");
//     Serial.print(" Protocol: ");
//     Serial.println(protocol);
//   }
//
//   Serial.print("Raw data: ");
//   for (unsigned int i=0; i<= length*2; i++) {
//     Serial.print(raw[i]);
//     Serial.print(",");
//   }
//   Serial.println();
//   Serial.println();
// }
//
// static const char* bin2tristate(const char* bin) {
//   static char returnValue[50];
//   int pos = 0;
//   int pos2 = 0;
//   while (bin[pos]!='\0' && bin[pos+1]!='\0') {
//     if (bin[pos]=='0' && bin[pos+1]=='0') {
//       returnValue[pos2] = '0';
//     } else if (bin[pos]=='1' && bin[pos+1]=='1') {
//       returnValue[pos2] = '1';
//     } else if (bin[pos]=='0' && bin[pos+1]=='1') {
//       returnValue[pos2] = 'F';
//     } else {
//       return "not applicable";
//     }
//     pos = pos+2;
//     pos2++;
//   }
//   returnValue[pos2] = '\0';
//   return returnValue;
// }
//
// static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength) {
//   static char bin[64];
//   unsigned int i=0;
//
//   while (Dec > 0) {
//     bin[32+i++] = ((Dec & 1) > 0) ? '1' : '0';
//     Dec = Dec >> 1;
//   }
//
//   for (unsigned int j = 0; j< bitLength; j++) {
//     if (j >= bitLength - i) {
//       bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
//     } else {
//       bin[j] = '0';
//     }
//   }
//   bin[bitLength] = '\0';
//
//   return bin;
// }
//
//
//
// void setup() {
//   Serial.begin(9600);
//   mySwitch.enableReceive(14);  // Receiver on interrupt 0 => that is pin #2
// }
//
// void loop() {
//   if (mySwitch.available()) {
//     output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
//     mySwitch.resetAvailable();
//   }
// }

// // 263505
// // 263508
// RCSwitch mySwitch = RCSwitch();
//
// void setup() {
// Serial.begin(9600);
//
//   // Transmitter is connected to Arduino Pin #0
//   mySwitch.enableTransmit(2);  // Optional set pulse length.
//   // mySwitch.setPulseLength(320);
//
//   // Optional set protocol (default is 1, will work for most outlets)
//   // mySwitch.setProtocol(2);
//
//   // Optional set number of transmission repetitions.
//   // mySwitch.setRepeatTransmit(15);
// }
//
// void loop() {
// /* Same switch as above, but using decimal code */
//
// mySwitch.send(263505, 24);
// delay(2000);
// mySwitch.send(263508, 24);
// delay(2000);
//
// }
