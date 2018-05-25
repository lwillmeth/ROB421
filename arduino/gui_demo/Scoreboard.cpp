// Scoreboard.cpp
#include "Arduino.h"
#include "FastLED.h"

#define NUM_LEDS 36
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
    {8,  0,1,2,3,4,5,14,15},
    {18, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17},
    {13, 0,1,2,3,4,5,11,12,13,14,15,16,17}
  };

  bool smallDigitB[10][NUM_LEDS/2]{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1},
    {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1}
  };

  void shiftDigit(int pin, int start_LED, int R, int G, int B, int current_digit, int target_digit){
    // Morphs from current digit to new digit, with randomized colors/patterns in between
    
    // Randomize mid points for each LED in the digit
    int midpoint[18][3];
    // LEDs use RGB values
    for(int i=0; i<18; i++){
      for(int j=0; j<3; j++){
        midpoint[i][j] = random(256);
      }
    }

    // Iterate each LED from current position to midpoint in exactly 20 steps
    // There are 18 LEDs per digit
    for(int i=1; i<=20; i++){     // i = each step
      for(int d=0; d<18; d++){    // d = each LED
        leds[d+start_LED].setRGB(
            R + i*(midpoint[d][0] - R)/20,
            G + i*(midpoint[d][1] - G)/20,
            B + i*(midpoint[d][2] - B)/20
        );
      }
      // Update LEDs and pause briefly between transitions
      FastLED.show();
      delay(25);
    }

    // At this point the digit should be random colors/shapes, bring it back to target_digit
    for(int i=1; i<=20; i++){     // i = each step
      for(int d=0; d<18; d++){    // d = each LED

        if(smallDigitB[target_digit][d]){
          // This LED should be used in target_digit
          leds[d+start_LED].setRGB(
              R + i*(midpoint[d][0] - R)/20,
              G + i*(midpoint[d][1] - G)/20,
              B + i*(midpoint[d][2] - B)/20
          );
        }else{
          // This LED should be black in target_digit
          leds[d+start_LED].setRGB(
              midpoint[d][0] - i*midpoint[d][0]/20,
              midpoint[d][1] - i*midpoint[d][1]/20,
              midpoint[d][2] - i*midpoint[d][2]/20
          );
        }
      }
      // Update LEDs and pause briefly between transitions
      FastLED.show();
      delay(25);
    }
  }

  public:
  int blueScore;
  int redScore;
  int num_leds;
  
  Scoreboard(int pin, int num_leds){
    for(int i=0; i <= NUM_LEDS; i++){
      leds[i] = smallDigitB[i]?CRGB::Red:CRGB::Black;
    }

    num_leds = num_leds;
    blueScore = 0;
    redScore = 0;
    updateRed();
    updateBlue();
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  }

  void updateRed(){
    for(int i=0; i <= NUM_LEDS/2; i++){
      leds[i] = smallDigitB[i]?CRGB::Red:CRGB::Black;
    }
    FastLED.show();
  }

  void updateBlue(){
    for(int i=NUM_LEDS/2; i < NUM_LEDS; i++){
      leds[i] = smallDigitB[i]?CRGB::Blue:CRGB::Black;
    }
    FastLED.show();
  }
  
};

