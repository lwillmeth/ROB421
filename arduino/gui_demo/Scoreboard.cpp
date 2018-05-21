// Scoreboard.cpp
#include "Arduino.h"
#include "FastLED.h"

#define NUM_LEDS 25
#define DATA_PIN 2

class Scoreboard{
  CRGB leds[NUM_LEDS];
  
  int smallDigit[10][19]{
    {16, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
    {6,  0,1,2,3,4,5},
    {12, 0,1,2,6,7,8,9,10,14,15,16,17},
    {12, 0,1,2,3,4,5,6,7,14,15,16,17},
    {10, 0,1,2,3,4,5,11,12,13,16,17},
    {12, 3,4,5,6,7,11,12,13,14,15,16,17},
    {15, 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17},
    {8, 0,1,2,3,4,5,14,15},
    {18, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17},
    {13, 0,1,2,3,4,5,11,12,13,14,15,16,17}
  };

  public:
  Scoreboard(int pin, int num_leds){
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  }
  
  void demo() {
//    digitalWrite(LED_BUILTIN, HIGH);
    for(int digit=0; digit<10; digit++){
//      Serial.println(digit);
      for(int i=1; i <= smallDigit[digit][0]; i++){
//        Serial.print("\t");
//        Serial.println(smallDigit[digit][i]);
        leds[smallDigit[digit][i]] = CRGB::Red;
      }
      FastLED.show();
      for(int i=0; i<NUM_LEDS; i++){
        leds[i] = CRGB::Black;
      }
      delay(500);
//      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  
};

