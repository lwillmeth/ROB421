// Motors.cpp
#include "Arduino.h"
#include "DualG2HighPowerMotorShield.h"

#define TIMER_DELAY 5000
#define MAX_SPEED 400
#define MIN_SPEED 0

class PololuMotors 
{
  int MotorSpeed;
  double LMotorPercent;
  double RMotorPercent;
  DualG2HighPowerMotorShield24v14 md;

  public:
  /*
   * Constructor
   */
  PololuMotors( void )
  {
      MotorSpeed    = 0;
      LMotorPercent = 1;
      RMotorPercent = 1;
      md.init();
      md.calibrateCurrentOffsets();
      md.disableDrivers();
  }
  
  /*
   * Increase motor speed by 10%
   */
  void incSpeed() 
  {
    MotorSpeed = validateSpeed( MotorSpeed * 1.1 );
  }
  
  /*
   * Decrease motor speed by 10%
   */
  void decSpeed() 
  {
    MotorSpeed = validateSpeed( MotorSpeed * 0.9 );
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
    md.enableDrivers();
    delay(1);  // The drivers require a maximum of 1ms to elapse when brought out of sleep mode.
  
    // range is -400 (reverse) to 400 (forward)
    for (int i = 0; i <= 400; i++) {
      setSpeed(i);
      delay(2);
    }
  
    delay(TIMER_DELAY);
    for (int i =400; i >= 0; i--) {
      setSpeed(i);
      delay(2);
    }
    md.disableDrivers();
  }

};
