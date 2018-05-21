#include "Motors.cpp"
#include "Scoreboard.cpp"
#include <Bridge.h>
#include <BlynkSimpleYun.h>

#define BLYNK_PRINT Serial
#define SCORE_L_PIN   9
#define SCORE_R_PIN   8

#define BMONITOR_PIN  A3
#define BPOWER_PIN    A4
#define BGND_PIN      A5

#define TURN_STEPS    900
#define TURN_PUL_PIN  5
#define TURN_DIR_PIN  3
#define TURN_ENA_PIN  13
#define TURN_DELAY    1000

// Blynk auth code
char auth[] = "ec4d250a7a664698b3eaefb3fdba2853";

// Calibration value for photoresistor
int CAL_PHOTO_THRESHOLD;

PololuMotors throwMotors;
Scoreboard scoreboard(2, 25);

/*
 * Turn the robot x degrees (pos is cw, neg is ccw)
 */
void turn(int numSteps)
{
  for (int i=0; i<abs(numSteps); i++){
    if(numSteps > 0){
      digitalWrite(TURN_DIR_PIN,LOW);
    }else{
      digitalWrite(TURN_DIR_PIN,HIGH);
    }

    digitalWrite(TURN_PUL_PIN,HIGH);
    delayMicroseconds(TURN_DELAY);
    digitalWrite(TURN_PUL_PIN,LOW);
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("ROB421 GUI Demo");

  Blynk.begin(auth);

  // Prepare the motors
  throwMotors.begin();

  // Scoreboard pins
  pinMode(SCORE_L_PIN,  OUTPUT);  // scoreboard left digit
  pinMode(SCORE_R_PIN,  OUTPUT);  // scoreboard right digit
  // Photoresistor pins
  pinMode(BMONITOR_PIN, INPUT);   // photoresistor
  pinMode(BPOWER_PIN,   OUTPUT);  // photoresistor power
  pinMode(BGND_PIN,     OUTPUT);  // photoresistor ground
  // Stepper pins
  pinMode(TURN_PUL_PIN, OUTPUT);
  pinMode(TURN_DIR_PIN, OUTPUT);
  pinMode(TURN_ENA_PIN, OUTPUT);
  
  digitalWrite(BPOWER_PIN, HIGH);
  digitalWrite(BGND_PIN,   LOW);

  delay(3000);
  long photo_sum = 0;
  for(int i=0; i<500; i++){
    photo_sum += analogRead(BMONITOR_PIN);
  }
  CAL_PHOTO_THRESHOLD = photo_sum / 500;
  CAL_PHOTO_THRESHOLD -= CAL_PHOTO_THRESHOLD/4;

  Serial.print("CAL_PHOTO_THRESHOLD = ");
  Serial.println(CAL_PHOTO_THRESHOLD);
}


void loop()
{
  Blynk.run();
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
}

/*
 * Get spin value from app
 */
BLYNK_WRITE(V2)
{
  throwMotors.spinVal = param.asInt(); 
  Serial.print("New spinVal = ");
  Serial.println(throwMotors.spinVal);
}

/*
 * Get max motor speed from app
 */
BLYNK_WRITE(V3)
{
  throwMotors.maxSpeed = param.asInt();
  Serial.print("New MotorSpeed = ");
  Serial.println(throwMotors.maxSpeed);
}

/*
 * Get yaw angle from app
 */
BLYNK_WRITE(V4)
{
  int move_steps = param.asInt();
  Serial.print("Adjust yaw by ");
  Serial.print(move_steps);
  Serial.println(" steps.");

  turn(move_steps);
}

/*
 * Get blue score from app
 */
BLYNK_WRITE(V6)
{
  scoreboard.blueScore += param.asInt();
  if( scoreboard.blueScore < 0 ) scoreboard.blueScore = 0;
  if( scoreboard.blueScore > 9 ) scoreboard.blueScore = 9;
  Serial.print("Blue score: ");
  Serial.println( scoreboard.blueScore );
  scoreboard.updateBlue();
}

/*
 * Triggers on 'FIRE' command from app
 */
BLYNK_WRITE(V9)
{
  throwMotors.firingSequence();
  Blynk.virtualWrite(V9, 0);
}

/*
 * Send analog ball monitor value to app
 */
BLYNK_READ(V0)
{
  if(analogRead(BMONITOR_PIN) < CAL_PHOTO_THRESHOLD){
    Serial.println("Ball detected!");
    Blynk.virtualWrite(V1, 1023);
  }
  Blynk.virtualWrite(V0, analogRead(BMONITOR_PIN));
}

/*
 * Send ball status values to app
 */
BLYNK_READ(V1)
{
  Serial.print("V1: ");
  Blynk.virtualWrite(V1, 1023);
//  throwMotors.firingSequence();
}

/*
 * Send motor ratios to app
 */
BLYNK_READ(V2)
{
    Blynk.virtualWrite(V2, throwMotors.spinVal);
}

/*
 * Send motor speed to app
 */
BLYNK_READ(V3)
{
    Blynk.virtualWrite(V3, throwMotors.maxSpeed);
}

/*
 * Send threshold to app
 */
BLYNK_READ(V8)
{
  Blynk.virtualWrite(V8, CAL_PHOTO_THRESHOLD);
}

