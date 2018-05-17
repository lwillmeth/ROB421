#include "FastLED.h"

#define NUM_LEDS 36
#define DATA_PIN 5

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

int smallDigit[][19]{
  {16, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {6,  0,1,2,3,4,5},
  {12, 0,1,2,6,7,8,9,10,14,15,16,17},
  {12, 0,1,2,3,4,5,6,7,14,15,16,17},
  {10, 0,1,2,3,4,5,11,12,13,16,17},
  {12, 3,4,5,6,7,11,12,13,14,15,16,17},
  {15, 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17},
  {8,  0,1,2,3,4,5,14,15},
  {18, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17},
  {13, 0,1,2,3,4,5,11,12,13,14,15,16,17}
};

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  delay(1000);
//  Serial.println("Starting up..");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);

  // Left digit
  for(int left=0; left<10; left++){
    Serial.println(left);
    for(int i=1; i <= smallDigit[left][0]; i++){
      leds[18+smallDigit[left][i]] = CRGB::Red;
    }
    FastLED.show();

    // Right digit
    for(int right=0; right<10; right++){
      Serial.print("\t -> ");
      Serial.println(right);
      for(int i=1; i <= smallDigit[right][0]; i++){
        leds[smallDigit[right][i]] = CRGB::Blue;
      }
      FastLED.show();
      
      // reset the left digit
      for(int i=0; i<NUM_LEDS/2; i++){
        leds[i] = CRGB::Black;
      }
      delay(500);
    }

    // reset the right digit
    for(int i=18; i<NUM_LEDS; i++){
      leds[i] = CRGB::Black;
    }
  }

  delay(3000);
  digitalWrite(LED_BUILTIN, LOW);
}
