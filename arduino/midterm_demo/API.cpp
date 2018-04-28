// API.cpp
#include "Arduino.h"

class API
{
  #define numChars 32
  char receivedChars[numChars];
  char tempChars[numChars];
  char apiFunction[numChars] = {0};
  boolean newData = false;
  int arg1;
  int arg2;

  public:
//  API(void) { }
  /*
   * Check serial connection for API calls
   */
  boolean listen() {
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
      // Only parse the latest API call
      if(newData){
        strcpy(tempChars, receivedChars);
        parseData();
      }
      return newData;
  }
  
  /*
   * Parse string from an API call, validate as command, and execute
   */
  void parseData() {
      char * strtokIndx; // this is used by strtok() as an index
  
      strtokIndx = strtok(tempChars,",");   // get the first part - the string
      strcpy(apiFunction, strtokIndx);      // copy it to messageFromPC
   
      strtokIndx = strtok(NULL, ",");       // this continues where the previous call left off
      arg1 = atoi(strtokIndx);              // convert this part to an integer
  
      strtokIndx = strtok(NULL, ",");
      arg2 = atoi(strtokIndx);
  }
 
  /*
   * Display the contents of an API call
   */
  void showAPICall() {
      Serial.print("Function call: ");
      Serial.print(apiFunction);
      Serial.print(", arg1: ");
      Serial.print(arg1);
      Serial.print(", arg2: ");
      Serial.println(arg2);
  }
};
