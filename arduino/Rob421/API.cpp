// API.cpp
#include "Arduino.h"
#define APIMaxChars 32
  
struct apiCall {
  int fnNum;
  int arg1;
  int arg2;
  boolean isReady;
};

class API
{
  char receivedChars[APIMaxChars];
  char tempChars[APIMaxChars];
//  char apiFunction[APIMaxChars] = {0};
  boolean newData = false;
//  int arg1;
//  int arg2;
  apiCall lastCmd;

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
                  if (ndx >= APIMaxChars) {
                      ndx = APIMaxChars - 1;
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
        newData = false;
        return true;
      }
      return false;
  }
  
  /*
   * Parse string from an API call, validate as command, and execute
   */
  void parseData() {
      char * strtokIndx;                  // this is used by strtok() as an index

      strtokIndx = strtok(tempChars,",");
      lastCmd.fnNum = atoi(strtokIndx);
   
      strtokIndx = strtok(NULL, ",");
      lastCmd.arg1 = atoi(strtokIndx);
  
      strtokIndx = strtok(NULL, ",");
      lastCmd.arg2 = atoi(strtokIndx);
      lastCmd.isReady = true;
  }
 
  /*
   * Display the contents of an API call
   */
  void showAPICall() {
      Serial.print("Function call: ");
      Serial.print(lastCmd.fnNum);
      Serial.print(", arg1: ");
      Serial.print(lastCmd.arg1);
      Serial.print(", arg2: ");
      Serial.println(lastCmd.arg2);
  }

  apiCall getAPICall(){
    return lastCmd;
  }
};
