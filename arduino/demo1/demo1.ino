#include "FastLED.h"

#define NUM_LEDS 25
#define DATA_PIN 7

// Define the array of leds
CRGB leds[NUM_LEDS];

int simpleNumbers[][26]{
  {22, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21},
  {8,  0,1,2,3,4,5,6,7},
  {17, 0,1,2,3,8,9,10,11,12,13,14,19,20,21,22,23,24},
  {17, 0,1,2,3,4,5,6,7,8,9,10,19,20,21,22,23,24},
  {15, 0,1,2,3,4,5,6,7,15,16,17,18,22,23,24},
  {17, 4,5,6,7,8,9,10,15,16,17,18,19,20,21,22,23,24},
  {21, 4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24},
  {11, 0,1,2,3,4,5,6,7,19,20,21},
  {25, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24},
  {18, 0,1,2,3,4,5,6,7,15,16,17,18,19,20,21,22,23,24}
};

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
//    for(int dot = 0; dot < NUM_LEDS; dot++) { 
//        leds[dot] = CRGB::Red;
//        FastLED.show();
//        // clear this led for the next time around the loop
//        leds[dot] = CRGB::Black;
//        delay(100);
//    }
  for(int digit=0; digit<10; digit++){
    Serial.println(digit);
    for(int i=1; i <= simpleNumbers[digit][0]; i++){
      Serial.print("\t");
      Serial.println(simpleNumbers[digit][i]);
      leds[simpleNumbers[digit][i]] = CRGB::Red;
    }
    FastLED.show();
    for(int i=0; i<NUM_LEDS; i++){
      leds[i] = CRGB::Black;
    }
    delay(250);
  }
}
