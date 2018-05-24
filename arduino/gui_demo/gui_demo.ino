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
#define TURN_DELAY    1500

// Blynk auth code
char auth[] = "90782f78485a448f9ab68a245af6ac4a";

// Default behavior is to auto throw if the ball is detected
bool autoThrow = true;

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

//  Blynk.begin(auth);
  Blynk.begin(auth, IPAddress(192,168,240,109), 8080);

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

  // Initialize photo resistor
  delay(500);
  calibrate_photo();
  Serial.print("CAL_PHOTO_THRESHOLD = ");
  Serial.println(CAL_PHOTO_THRESHOLD);
}

int calibrate_photo()
{
  long photo_sum = 0;
  for(int i=0; i<500; i++){
    photo_sum += analogRead(BMONITOR_PIN);
  }
  CAL_PHOTO_THRESHOLD = photo_sum / 500;
  CAL_PHOTO_THRESHOLD -= (CAL_PHOTO_THRESHOLD/10);
  return CAL_PHOTO_THRESHOLD;
}


void loop()
{
  Blynk.run();
  // Automatically throw, if appropriate to do so
  if(autoThrow && analogRead(BMONITOR_PIN) < CAL_PHOTO_THRESHOLD){
    throwMotors.firingSequence();
  }
  Blynk.virtualWrite(V0, analogRead(BMONITOR_PIN));
}

// Update values on the app, using arduino values
BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
}

/*
 * Get throwing mode (auto/manual) from app
 */
BLYNK_WRITE(V1)
{
  autoThrow = param.asInt();
  Serial.print("autoThrow = ");
  Serial.println(autoThrow);
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
 * Resets photoresistor from app
 */
BLYNK_WRITE(V5)
{
  Serial.print("Re-calibrating photoresistor...");
  calibrate_photo();
  Serial.println(CAL_PHOTO_THRESHOLD);
  Blynk.virtualWrite(V5, 0);
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
 * Get red score from app
 */
BLYNK_WRITE(V7)
{
  scoreboard.redScore += param.asInt();
  if( scoreboard.redScore < 0 ) scoreboard.redScore = 0;
  if( scoreboard.redScore > 9 ) scoreboard.redScore = 9;
  Serial.print("Red score: ");
  Serial.println( scoreboard.redScore );
  scoreboard.updateRed();
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
  Blynk.virtualWrite(V0, analogRead(BMONITOR_PIN));
}

/*
 * Send the throwing mode to app
 */
BLYNK_READ(V1)
{
  Blynk.virtualWrite(V1, autoThrow);
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

