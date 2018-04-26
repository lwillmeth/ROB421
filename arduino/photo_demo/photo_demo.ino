#define DEBUG         true
#define DEBUG_LED     13
#define SCORE_L_PIN   9
#define SCORE_R_PIN   8
#define MOTOR_TL_PIN  6
#define MOTOR_TR_PIN  5
#define BMONITOR_PIN  0

#define TIMER_DELAY 5000
#define PHOTO_THRESHOLD 500

unsigned long last_fired;
//  Valid states are:
//    0 = idle
//    1 = ball waiting
//    2 = shot in progress
int state = 0;
boolean msg_sent = false;

// ---------------

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];

// variables to hold the parsed data
char apiFunction[numChars] = {0};
int leftMotor = 0;
int rightMotor = 0;
boolean newData = false;

void recvWithStartEndMarkers() {
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

void parseData() {
    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");   // get the first part - the string
    strcpy(apiFunction, strtokIndx);      // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ",");       // this continues where the previous call left off
    leftMotor = atoi(strtokIndx);         // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    rightMotor = atoi(strtokIndx);
}

void showParsedData() {
    Serial.print("Function call: ");
    Serial.print(apiFunction);
    Serial.print(", Left motor: ");
    Serial.print(leftMotor);
    Serial.print(", Right motor: ");
    Serial.println(rightMotor);
}

// ---------------

void setup() {
  // Initialize I/O pins
  pinMode(DEBUG_LED,    OUTPUT);  // debugging led
  pinMode(SCORE_L_PIN,  OUTPUT);  // scoreboard left digit
  pinMode(SCORE_R_PIN,  OUTPUT);  // scoreboard right digit
  pinMode(MOTOR_TL_PIN, OUTPUT);  // left throwing motor
  pinMode(MOTOR_TR_PIN, OUTPUT);  // right throwing motor
  pinMode(BMONITOR_PIN, INPUT);   // photoresistor

  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("ROB421 Sensor Integration Demo");
}

void loop() {
  
  // Check for new API commands
  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    parseData();
    showParsedData();
    newData = false;
  }

  // --------------------------------
  // Short demo of running the motors
  // only when the ball is detected
  // --------------------------------

  // Check if the ball is ready to fire
  if( analogRead(BMONITOR_PIN) < PHOTO_THRESHOLD ){
    // Send back a message
    if(state != 1){
      Serial.println("Ball is ready to fire.");
      state = 1;
    }
    
    // Record the current time and firing state
    last_fired = millis();
    
    // Spin up the motors
    digitalWrite( MOTOR_TL_PIN, HIGH);
    digitalWrite( MOTOR_TR_PIN, HIGH);

  }else{
    // Send back a message
    if(state == 1){
      Serial.println("Ball out of firing position.");
      state = 2;
    }
        
    // Run for 5 seconds after photo resistor is uncovered
    if(last_fired+TIMER_DELAY < millis()){

      // Send back a message
      if(state == 2){
        Serial.println("Firing sequence complete.");
        state = 3;
      }
      
      digitalWrite( MOTOR_TL_PIN, LOW);
      digitalWrite( MOTOR_TR_PIN, LOW);
    }
  }
  
//  Serial.println( analogRead(BMONITOR_PIN) );

}
