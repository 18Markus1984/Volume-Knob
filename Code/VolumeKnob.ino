//#include <HID-Project.h>
#include <CapacitiveSensor.h>
#include <AS5600.h>

CapacitiveSensor Sensor = CapacitiveSensor(4, 2);
AS5600 encoder;

const int LONG_PRESS_TIME  = 4000; // 4000 milliseconds
const int SHORT_PRESS_TIME = 500; // 500 milliseconds
int lastState = 0;  // the previous state from the input pin
int currentState;     // the current reading from the input pin
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;

long val;
int pos;
long revolutions = 0;   // number of revolutions the encoder has made
double position = 0;    // the calculated value the encoder is at
double output;          // raw value from AS5600
long lastOutput;        // last output from AS5600

int turnRight = 0;      
int turnLeft = 0;
int turnDistance = 10;

void setup() {
  Serial.begin(9600);
  //Consumer.begin();
  output = encoder.getPosition();
  lastOutput = output;
  position = output;
}

void loop() {
  ReadButtonStatus();
  ReadRotaryEncoderStatus();
}

void ReadButtonStatus() {
  currentState = Sensor.capacitiveSensor(30);
  
  if(lastState < 1000 && currentState >= 1000){           // button is pressed
    pressedTime = millis();
  }else if(lastState >= 1000 && currentState < 1000) {        // button is released
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

    if(pressDuration < SHORT_PRESS_TIME ){            //short press less than 0.5 sec detected
      Serial.println("A short press is detected");
      //Consumer.write(MEDIA_VOL_MUTE);
      delay(500);
    }else if(pressDuration > LONG_PRESS_TIME) {       //long press after 5 sec detected
      Serial.println("A long press is detected");
      delay(500);
    }
  }
  // save the the last state
  lastState = currentState;
}

void ReadRotaryEncoderStatus() {
  output = encoder.getPosition();           // get the raw value of the encoder                      
  if ((lastOutput - output) > 2047 )        // check if a full rotation has been made
    revolutions++;
  if ((lastOutput - output) < -2047 )
    revolutions--;
    
  if(output != lastOutput && output+1 != lastOutput && output+2 != lastOutput && output-1 != lastOutput && output-2 != lastOutput ){    // check for toleances -+ 2
    if(output > lastOutput){
      if(turnRight == turnDistance){        // check for amount of steps
        Serial.println("VolumeUp");
        //Consumer.write(MEDIA_VOL_UP);
        turnRight = 0;
      }else {
        turnRight++;
        turnLeft = 0;
      }
    }
    else{
      if(turnLeft == turnDistance){
        Serial.println("VolumeDown");
        //Consumer.write(MEDIA_VOL_DOWN);
        turnLeft = 0;
      }else{
        turnLeft++;
        turnRight = 0;
      }
    }
  }
  position = revolutions * 4096 + output;   // calculate the position the the encoder is at based off of the number of revolutions
  lastOutput = output;                      // save the last raw value for the next loop 
}
