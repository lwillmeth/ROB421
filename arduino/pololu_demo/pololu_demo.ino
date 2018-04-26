#include "DualG2HighPowerMotorShield.h"

#define DEBUGGING     true
#define DEBUG_LED     13
#define SCORE_L_PIN   9
#define SCORE_R_PIN   8
#define BMONITOR_PIN  0

#define TIMER_DELAY 5000
#define PHOTO_THRESHOLD 500

DualG2HighPowerMotorShield24v14 md;

// Calibration value for photoresistor
int CAL_PHOTO_THRESHOLD;

// Function parsing variables
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
char apiFunction[numChars] = {0};
int leftMotor = 0;
int rightMotor = 0;
boolean newData = false;

void setup()
{
  Serial.begin(115200);
  Serial.println("ROB421 Sensor Integration Demo");
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
}

/*
 * Cut power to the motors if the driver reports a fault
 */
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

/*
 * Check serial connection for API calls
 */
void checkAPICall() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            } else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        } else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

/*
 * Parse string from an API call, validate as command, and execute
 */
void parseData() {
    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");   // get the first part - the string
    strcpy(apiFunction, strtokIndx);      // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ",");       // this continues where the previous call left off
    leftMotor = atoi(strtokIndx);         // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    rightMotor = atoi(strtokIndx);
}

/*
 * Display the contents of an API call
 */
void showAPICall() {
    Serial.print("Function call: ");
    Serial.print(apiFunction);
    Serial.print(", Left motor: ");
    Serial.print(leftMotor);
    Serial.print(", Right motor: ");
    Serial.println(rightMotor);
}

/*
 * Firing sequence to throw a ball
 */
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

/*
 * Main program loop
 */
void loop()
{
  // Check for new API commands
  checkAPICall();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    parseData();
    showAPICall();
    newData = false;
  }

  // Check if the ball is ready to fire
  if(analogRead(BMONITOR_PIN) < CAL_PHOTO_THRESHOLD){
    Serial.print("Ball detected: ");
    Serial.println( analogRead(BMONITOR_PIN) );
    firingSequence();
  }
}
