#ifndef HEADER_MOTORS
  #define HEADER_MOTORS

  #include "Arduino.h"
  #include "DualG2HighPowerMotorShield.h"
  
  class PololuMotors {
    public:
      PololuMotors();
      void incSpeed();
      void decSpeed();
      void setMotorValues(int, int);
      void setRatio(int, int);
      void setSpeed(int);
      void firingSequence();
  
    private:
      int MotorSpeed;
      double LMotorPercent;
      double RMotorPercent;

      void writeToMotors();
      int validateSpeed( int );
      double validateRatio( double );
      DualG2HighPowerMotorShield24v14 md;
  };
  
  #endif
