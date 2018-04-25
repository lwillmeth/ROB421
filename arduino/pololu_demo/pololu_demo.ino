#include "DualG2HighPowerMotorShield.h"

#define DEBUGGING     true
#define DEBUG_LED     13
#define SCORE_L_PIN   9
#define SCORE_R_PIN   8
#define BMONITOR_PIN  0

#define TIMER_DELAY 5000
#define PHOTO_THRESHOLD 500

// Uncomment the version corresponding with the version of your shield.
DualG2HighPowerMotorShield24v14 md;
// DualG2HighPowerMotorShield18v18 md;
// DualG2HighPowerMotorShield24v18 md;
// DualG2HighPowerMotorShield18v22 md;

int CAL_PHOTO_THRESHOLD;

void stopIfFault()
{
  if (md.getM1Fault())
  {
    md.disableDrivers();
	delay(1);
    Serial.println("M1 fault");
    while (1);
  }
  if (md.getM2Fault())
  {
    md.disableDrivers();
	delay(1);
    Serial.println("M2 fault");
    while (1);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Dual G2 High Power Motor Shield");
  md.init();
  md.calibrateCurrentOffsets();
  md.disableDrivers();

  pinMode(DEBUG_LED,    OUTPUT);  // debugging led
  pinMode(SCORE_L_PIN,  OUTPUT);  // scoreboard left digit
  pinMode(SCORE_R_PIN,  OUTPUT);  // scoreboard right digit
  pinMode(BMONITOR_PIN, INPUT);   // photoresistor

  delay(1000);
  long photo_sum = 0;
  for(int i=0; i<500; i++){
    photo_sum += analogRead(BMONITOR_PIN);
  }
  CAL_PHOTO_THRESHOLD = photo_sum / 500;
  CAL_PHOTO_THRESHOLD -= CAL_PHOTO_THRESHOLD/4;
  if(DEBUGGING){
    Serial.print("CAL_PHOTO_THRESHOLD = ");
    Serial.println(CAL_PHOTO_THRESHOLD); 
  }

  // Uncomment to flip a motor's direction:
  //md.flipM1(true);
  //md.flipM2(true);
}

void firingSequence() {
  md.enableDrivers();
  delay(1);  // The drivers require a maximum of 1ms to elapse when brought out of sleep mode.

  Serial.print("Ramping up motors..   ");
  // range is -400 (reverse) to 400 (forward)
  for (int i = 0; i <= 400; i++) {
    md.setM1Speed(i);
    md.setM2Speed(i);
    stopIfFault();
    delay(2);
  }
  Serial.println("OK.");

  delay(TIMER_DELAY);
  Serial.print("Ramping down motors.. ");
  for (int i =400; i >= 0; i--) {
    md.setM1Speed(i);
    md.setM2Speed(i);
    stopIfFault();
    delay(2);
  }
  Serial.println("OK.");

  Serial.println("Firing sequence complete.");
  md.disableDrivers();
}

void loop()
{
//  Serial.println( analogRead(BMONITOR_PIN) );
  // Check if the ball is ready to fire
  if(analogRead(BMONITOR_PIN) < CAL_PHOTO_THRESHOLD){
    Serial.print("Ball detected: ");
    Serial.println( analogRead(BMONITOR_PIN) );
    firingSequence();
  }
}
