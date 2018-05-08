// Motors.cpp
#include "Arduino.h"
#include "DualG2HighPowerMotorShield.h"

#define TIMER_DELAY 5000
#define MAX_SPEED 400
#define MIN_SPEED 0
#define THROW_SERVO_PIN 10
#include <Servo.h>

class PololuMotors 
{
  int MotorSpeed;
  double LMotorPercent;
  double RMotorPercent;
  DualG2HighPowerMotorShield24v14 md;
  Servo throwServo;

  public:
  /*
   * Constructor
   */
  void begin( void )
  {
      MotorSpeed    = 0;
      LMotorPercent = 1;
      RMotorPercent = 1;
      md.init();
      md.calibrateCurrentOffsets();
      md.disableDrivers();

      throwServo.attach(THROW_SERVO_PIN);
      throwServo.write(90); // Set to midpoint
  }

  /*
   * Display current motor info
   */
   void display(){
    Serial.print("Total speed: ");
    Serial.println(MotorSpeed);
    
    Serial.print("Motor ratios: ");
    Serial.print(LMotorPercent);
    Serial.print(" : ");
    Serial.println(RMotorPercent);

    Serial.print("Motor speeds: ");
    Serial.print(MotorSpeed * LMotorPercent);
    Serial.print(" : ");
    Serial.println(MotorSpeed * RMotorPercent);
    Serial.println();
   }
  
  /*
   * Increase motor speed by 10%
   */
  void incSpeed() 
  {
    Serial.print("increasing to ");
    MotorSpeed = validateSpeed( MotorSpeed + MAX_SPEED*0.1 );
    Serial.print(MotorSpeed);
  }
  
  /*
   * Decrease motor speed by 10%
   */
  void decSpeed() 
  {
    Serial.print("decreasing to ");
    MotorSpeed = validateSpeed( MotorSpeed - MAX_SPEED*0.1 );
    Serial.print(MotorSpeed);
  }
  
  /*
   * Set both motor speeds to absolute values (unsafe)
   */
  void setMotorValues( int left, int right ) 
  {
    md.setM1Speed( left  );
    md.setM2Speed( right );
  }
  
  /*
   * Update both motor speed ratios as a percent 0-1
   */
  void setRatio( int left, int right ) 
  {
    LMotorPercent = validateRatio( (double) left / 100 );
    RMotorPercent = validateRatio( (double) right / 100 );
    writeToMotors();
  }
  
  /*
   * Update the total motor speed, then set both motors using their current ratios
   */
  void setSpeed( int newSpeed ) 
  {
    MotorSpeed = validateSpeed( newSpeed );
    writeToMotors();
  }
  
  /*
   * Update motor speeds using current parameters
   */
  void writeToMotors()
  {
    md.setM1Speed( MotorSpeed * LMotorPercent );
    md.setM2Speed( MotorSpeed * RMotorPercent );
  }
  
  /*
   * Ensure motor speeds remain within 0 (0%) and 400 (100%)
   */
  int validateSpeed( int newSpeed ) 
  {
    if( newSpeed > MAX_SPEED ){
      newSpeed = MAX_SPEED;
    } else if( newSpeed < MIN_SPEED ) {
      newSpeed = MIN_SPEED;
    }
    return newSpeed;
  }
  
  /*
   * Ensure motor ratios stay between 0 (0%) and 1 (100%)
   */
  double validateRatio( double newRatio ) 
  {
    if( newRatio > 1 ){
      newRatio = 1;
    } else if ( newRatio < 0 ) {
      newRatio = 1;
    }
    return newRatio;
  }
  
  /*
   * Firing sequence to throw a ball
   */
  void firingSequence() 
  {
    Serial.print("Throwing at ");
    Serial.print(MotorSpeed);
    
    md.enableDrivers();
    delay(1);  // The drivers require a maximum of 1ms to elapse when brought out of sleep mode.

    int goal = MotorSpeed;
  
    // range is -400 (reverse) to 400 (forward)
    for (int i = 0; i <= goal; i++) {
      if(i % 10 == 0) { Serial.print('.'); }
      setSpeed(i);
      delay(2);
    }
  
    delay(TIMER_DELAY);
    for (int i = goal; i >= 0; i--) {
      if(i % 10 == 0) { Serial.print('.'); }
      setSpeed(i);
      delay(2);
    }
    md.disableDrivers();
    
    MotorSpeed = goal;
    Serial.print(" OK.");
  }
  
  /*
   * Move the throwing servo back and forth
   */
  void move_throw_servo()
  {
    // Slowly swing servo into throwing position
    for(int i=90; i<=45; i--){
      throwServo.write(i);
      delay(10);
    }
    // Return to resting position
    throwServo.write(90);
  }

};
