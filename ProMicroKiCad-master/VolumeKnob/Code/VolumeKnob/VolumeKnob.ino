#include <HID-Project.h>
#include <Encoder.h>

const int CLK = 6;       
const int DT = 5;
const int SW = 2;
int leds[] = {18,4,7,8,9,10,16,15};  //The led order
int k = 8;    //only for testing number of Leds
int pos = 0;    //the selected Led
long myTimer = 0;
long myTimeout = 5000;    //timer when the lights should turn off
bool timer1 = false;      //State for turning left
bool timer2 = false;      //State for turning right
long altePosition = -999;   //position for the rotary encoder

Encoder meinEncoder(DT,CLK);  

void setup(){
  Consumer.begin();
  Serial.begin(9600);   
  pinMode(SW, INPUT);   
  for(int i = 0; i < k; i++){     //All output pins for the Leds get set
    pinMode(leds[i], OUTPUT);
  }
}

void loop(){
  long neuePosition = meinEncoder.read();   //read the position of the encoder
  if (neuePosition != altePosition){    
    Serial.print(neuePosition);
    digitalWrite(leds[pos], LOW);
    if(neuePosition > altePosition){      //if the wheel turns left
        Consumer.write(MEDIA_VOL_UP);
        if(timer1){
          pos--;
          if(pos < 0){
            pos = 7;
          }        
          timer1 = false;
        }else {
          timer1 = true;
        }
    }else {   
        Consumer.write(MEDIA_VOL_DOWN);
        if(timer2){
          pos++;
          if(pos == 8){
            pos = 0;
          }
          timer2 = false;
        }else{
          timer2 = true;
        }
    }  
    digitalWrite(leds[pos], HIGH);
    myTimer = millis();
    altePosition = neuePosition;       
  }
  if (millis() > myTimeout + myTimer) {   //turns of the Led that was turned on off after the timer ends
    myTimer = millis();
    digitalWrite(leds[pos], LOW);
  }
  if(digitalRead(SW) == LOW){     //rotary encoder Button is pressed
    Serial.println("Mute"); 
    Consumer.write(MEDIA_VOL_MUTE);
    lightAll(700);
  }
  delay(10);
}

void circle(int sec){
  for(int i = 0; i < k; i++){
    digitalWrite(leds[i], HIGH);
    delay(sec);
    digitalWrite(leds[i], LOW);
    delay(sec);
  }
}

void circleFluend(int sec){
  for(int i = 0; i < k; i++){
    digitalWrite(leds[i], HIGH);
    if(i >= 1){
      digitalWrite(leds[i-1], LOW);
    }
    delay(sec);
  }
}

void lightAll(int sec){
  for(int i = 0; i < k; i++){
    digitalWrite(leds[i], HIGH);
  }
  delay(sec);
  for(int i = 0; i < k; i++){
    digitalWrite(leds[i], LOW);
  }
}
