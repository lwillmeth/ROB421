#define DEBUG         true
#define DEBUG_LED     13
#define SCORE_L_PIN   9
#define SCORE_R_PIN   8
#define MOTOR_TL_PIN  6
#define MOTOR_TR_PIN  5
#define BMONITOR_PIN  0

#define TIMER_DELAY 5000

unsigned long time_start;
boolean winding_up = false;

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
  // Short demo of running the motors only when the ball is detected

  if( winding_up ){
    
  }

  // Check if the ball is ready to fire
  if( analogRead(BMONITOR_PIN) < 500 ){
    // Record the current time and firing state
    time_start = millis()
    firing = true;
    
    // Spin up the motors
    digitalWrite( MOTOR_TL_PIN, HIGH);
    digitalWrite( MOTOR_TR_PIN, HIGH);
  }else{
    
    digitalWrite( MOTOR_TL_PIN, LOW);
    digitalWrite( MOTOR_TR_PIN, LOW);
  }
  
  Serial.println( analogRead(BMONITOR_PIN) );

}
