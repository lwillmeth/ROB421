//int PUL=7; //define Pulse pin
//int DIR=6; //define Direction pin
//int ENA=5; //define Enable Pin

int PUL=5; //define Pulse pin
int DIR=3; //define Direction pin
int ENA=13; //define Enable Pin
#define DELAY 500
#define STEPS 900
void setup() {
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);

}

void loop() {
  for (int i=0; i<STEPS; i++)    //Forward 5000 steps
  {
    digitalWrite(DIR,LOW);
    digitalWrite(ENA,HIGH);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(DELAY);
    digitalWrite(PUL,LOW);
    delayMicroseconds(DELAY);
  }
  for (int i=0; i<STEPS; i++)   //Backward 5000 steps
  {
    digitalWrite(DIR,HIGH);
    digitalWrite(ENA,HIGH);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(DELAY);
    digitalWrite(PUL,LOW);
    delayMicroseconds(DELAY);
  }
}
