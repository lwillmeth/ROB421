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
  int blueScore;
  int redScore;
  
  Scoreboard(int pin, int num_leds){
    blueScore = 0;
    redScore = 0;
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  }

  void updateBlue(){
    for(int i=0; i<NUM_LEDS; i++){
      leds[i] = CRGB::Black;
    }
    for(int i=1; i <= smallDigit[blueScore][0]; i++){
      leds[smallDigit[blueScore][i]] = CRGB::Blue;
    }
    FastLED.show();
  }

  
  void demo() {
    for(int digit=0; digit<10; digit++){
      for(int i=1; i <= smallDigit[digit][0]; i++){
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

