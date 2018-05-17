// Motors.cpp
#include "Arduino.h"
#include "DualG2HighPowerMotorShield.h"
#include <ServoTimer2.h>

#define TIMER_DELAY 2000
#define MAX_SPEED 400
#define MIN_SPEED 0
#define THROW_SERVO_PIN 11

class PololuMotors
{
  int MotorSpeed;
  double LMotorPercent;
  double RMotorPercent;
  DualG2HighPowerMotorShield24v14 md;
  ServoTimer2 throwServo;

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
      throwServo.write(2300);
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
    writeToMotors(MotorSpeed);
  }

  /*
   * Update motor speeds using current parameters
   */
  void writeToMotors()
  {
    writeToMotors( MotorSpeed );
  }

  void writeToMotors(int mSpeed)
  {
    md.setM1Speed( mSpeed );
    md.setM2Speed( mSpeed );
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
      if(i % 10 == 0) { Serial.print('+'); }
      setSpeed(i);
      delay(2);
    }
    // Spin up the throwing motors
    delay(TIMER_DELAY);

    // Move the firing servo arm
    move_throw_servo();

    // Ball is gone, might as well wind down
    for (int i = goal; i >= 0; i--) {
      if(i % 10 == 0) { Serial.print('-'); }
      setSpeed(i);
      delay(2);
    }

    // Leave motors disabled when not intentionally using them
    md.disableDrivers();

    MotorSpeed = goal;
    Serial.println(" OK.");
  }

  /*
   * Move the throwing servo back and forth
   */
  void move_throw_servo()
  {
    Serial.print(">");
    throwServo.write(300);
    delay(1500);
    throwServo.write(2500);
    Serial.print("<");
  }
