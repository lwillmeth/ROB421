#include "Motors.cpp"
#include "API.cpp"
//#include "Scoreboard.cpp"

#define DEBUGGING     true
#define DEBUG_LED     13
#define SCORE_L_PIN   9
#define SCORE_R_PIN   8
#define BMONITOR_PIN  A3
#define BPOWER_PIN    A4
#define BGND_PIN      A5
#define PHOTO_THRESHOLD 500

#define TURN_RATIO    2.5
#define TURN_PUL_PIN  7
#define TURN_DIR_PIN  6
#define TURN_ENA_PIN  5
#define TURN_DELAY    500

// Calibration value for photoresistor
int CAL_PHOTO_THRESHOLD;
boolean newData;

PololuMotors throwMotors;
API apiHandle;
//Scoreboard scoreboard(2, 25);
apiCall waitingAPICmd;
static const apiCall noAPICmd;


/*
 * Turn the robot x degrees (pos is cw, neg is ccw)
 */
void turn(int deg)
{
  int steps = deg * TURN_RATIO;
  Serial.print("inside, steps = ");
  Serial.println(steps);
  
  for (int i=0; i<steps; i++){
    if(deg > 0)
      digitalWrite(TURN_DIR_PIN,LOW);
    else
      digitalWrite(TURN_DIR_PIN,HIGH);
    digitalWrite(TURN_ENA_PIN,HIGH);
    digitalWrite(TURN_PUL_PIN,HIGH);
    delayMicroseconds(TURN_DELAY);
    digitalWrite(TURN_PUL_PIN,LOW);
    delayMicroseconds(TURN_DELAY);
  }
  
}

void setup()
{
  Serial.begin(115200);
  Serial.println("ROB421 Sensor Integration Demo");

  // Prepare the motors
  throwMotors.begin();

  pinMode(DEBUG_LED,    OUTPUT);  // debugging led
  pinMode(SCORE_L_PIN,  OUTPUT);  // scoreboard left digit
  pinMode(SCORE_R_PIN,  OUTPUT);  // scoreboard right digit
  pinMode(BMONITOR_PIN, INPUT);   // photoresistor
  pinMode(BPOWER_PIN,   OUTPUT);  // photoresistor power
  pinMode(BGND_PIN,     OUTPUT);  // photoresistor ground

  digitalWrite(BPOWER_PIN, HIGH);
  digitalWrite(BGND_PIN,   LOW);

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
}


/*
 * Main program loop
 */
void loop()
{
  // Check for new API commands

  if(apiHandle.listen()) {
    waitingAPICmd = apiHandle.getAPICall();
    if(waitingAPICmd.isReady){
      waitingAPICmd.isReady = false;
//      apiHandle.showAPICall();

      switch(waitingAPICmd.fnNum){
        case -1:
          throwMotors.decSpeed();
          break;
        case 1:
          // Set both throw motors to absolute (0-400) values
          // <1 200 250>
          throwMotors.setMotorValues(waitingAPICmd.arg1, waitingAPICmd.arg2);
          break;
        case 2:
          // Set both throw motors to a percentage of the current max speed
          // <2 50 65>
          throwMotors.setRatio(waitingAPICmd.arg1, waitingAPICmd.arg2);
          break;
        case 3:
          // Set the max (100%) speed for both motors
          // <3 50 65>
          throwMotors.setSpeed(waitingAPICmd.arg1);
          break;
        case 4:
          // Step forward or backwards x steps
          // <4 2>
          Serial.print("Turning ");
          Serial.print(waitingAPICmd.arg1);
          Serial.print(" degrees ");
          if(waitingAPICmd.arg1 > 0)
            Serial.println("CW.");
          else
            Serial.println("CCW.");
          turn(waitingAPICmd.arg1);
          break;
        case 9:
          // Run the firing sequence
          throwMotors.firingSequence();
          break;
        default:
          // Set the max (100%) speed for both motors, IFF 10 <= x <= 100
          if( 10 <= waitingAPICmd.fnNum && waitingAPICmd.fnNum <= 100 ){
            // Set motor speed to a percentage (this is getting messy)
            throwMotors.setSpeed(4 * waitingAPICmd.fnNum);
          } else {
            Serial.print(waitingAPICmd.fnNum);
            Serial.println(" is not a valid command.");
          }
      }
      throwMotors.display();
    }
  }

  // Check if the ball is ready to fire
  if(analogRead(BMONITOR_PIN) < CAL_PHOTO_THRESHOLD){
    Serial.print("Ball detected: ");
    Serial.println( analogRead(BMONITOR_PIN) );
    throwMotors.firingSequence();
  }
}
