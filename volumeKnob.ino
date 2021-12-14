#include <HID-Project.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
#include <OneButton.h>
#include <EEPROM.h>

#define PIN       7 //Datapin for the LEDs
#define NUMPIXELS 8 //Anzahl der LEDs

//Color Options
byte color[25][3] = {{255,0,0},{255,64,0},{255,128,0},{255,191,0},{255,255,0},{191,255,0},{128,255,0},{64,255,0},{0,255,0},{0,255,64},{0,255,128},{0,255,191},{0,255,255},{0,191,255},
{0,128,255},{0,64,255},{0,0,255},{64,0,255},{128,0,255},{191,0,255},{255,0,255},{255,0,191},{255,0,128},{255,0,64},{255,0,0}};
int row = 0;

//aktColor
byte aktColor[8][3];

//Rotary Encoder
const int CLK = 6;//Datapin Encoder         
const int DT = 5; //Datepin Encoder
const int SW = 2; //Button des Encoders

int oldpos = 0;   //Puffer for Encoder
int pos = 0;    //the selected Led
int  brightness = 255; //Helligkeit der LEDs
long myTimeout = 5000;    //timer when the lights should turn off
long altePosition = -999;   //position for the rotary encoder
bool timer1 = false;      //State for turning left //I use this variables to check that you have to move two incerements to get one move in the volume 
bool timer2 = false;      //State for turning right

//Settings-menu
int menu = 0;
int selected = 0;

//Settings
int sec = 0;
uint8_t wait = 20;
int NumbPixel = 0; //Max Pixel = 7 und Min Pixel = 1
int mode = 0; //Modi: 0 = Normal Only Color with Truning, 1 = A constant rotating Pixel, 2 = colorWipes, 3 = rainbow, 4 = rainbowCycle, 5 = theaterChaseRainbow


OneButton EncoderSwitch(SW,true);
Encoder meinEncoder(DT,CLK);  
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup(){
  Serial.begin(9600);
  Serial.println("Consumer setup...");
  Consumer.begin();

  //Rotary Encoder with Switch
  Serial.println("Rotary Encoder setup...");
  pinMode(SW, INPUT);
  EncoderSwitch.attachClick(clickt);
  EncoderSwitch.attachLongPressStop(doubleclick);
  //attachInterrupt(digitalPinToInterrupt(SW), checkTicks, CHANGE);

  //LEDs
  pixels.begin();
  row = EEPROM.read(0);
 
}

void loop(){ 
  if(menu == 0){    //Volume-Controll
    int i = TurnEncoder(8);
    if(i != -1){
      SetLED(oldpos,0,0,0);
      SetLED(i,color[row][0],color[row][1],color[row][2]);
      brightness = 255;
      if(i>oldpos && i != 7 && oldpos != 0){
        //Serial.print(i);
        //Serial.print(">");
        //Serial.print(oldpos);        
        //Serial.println("VolumeDown");
        Consumer.write(MEDIA_VOL_DOWN);
      }else if(i<oldpos && i != 0 && oldpos != 7){
        //Serial.print(i);
        //Serial.print("<");
        //Serial.print(oldpos);  
        //Serial.println("VolumeUp");
        Consumer.write(MEDIA_VOL_UP);
      }
      oldpos = i;
    }
  } else if (menu == 1) {   //Color-Selection
    int i = TurnEncoder(25);
    if(i != -1){
      row = i;
      lightAll(color[row][0],color[row][1],color[row][2]);
    }
  } else if (menu == 2) {
    int i = TurnEncoder(2);
    if(i != -1){
      mode = i;
    }
  }
  
  showPixels();
  EncoderSwitch.tick();
  delay(10); 
}

void showPixels(){
  for(int i = 0; i < NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(aktColor[i][0], aktColor[i][1], aktColor[i][2]));
  }
  if(menu == 0){
    if(brightness <= 1){
      ResetLEDs();
    }
    for(int i = 0; i < NUMPIXELS; i++){
      if(aktColor[i][0] != 0 || aktColor[i][1] != 0 || aktColor[i][2] != 0){
        brightness = brightness - 1;
        i = NUMPIXELS;
      }
    }
  }
  pixels.setBrightness(brightness);
  pixels.show();
}

void checkTicks()
{
  EncoderSwitch.tick();
}

void clickt() {
  Serial.println("Click");
  if(menu == 0){
    //Consumer.write(MEDIA_VOL_MUTE);
    Serial.println("Mute");
    brightness = 255;
    lightAll(color[row][0],color[row][1],color[row][2]);
    delay(700); 
  } else if(menu == 1){
    Serial.println("leave with selected color");
    if(row != EEPROM.read(0)){
      EEPROM.write(0,row);
    }
    menu = 0;
    pos = 0;
    oldpos = 0;
    altePosition = -999;
    delay(100);   
  }
}

void doubleclick() {
  menu++;
  pos = 0;
  brightness = 255;
  ResetLEDs();
  if(menu == 1){
    lightAll(color[row][0],color[row][1],color[row][2]);
  }
  Serial.print(menu);
  Serial.println("LongClick");
}

int TurnEncoder(int len) {
  long neuePosition = meinEncoder.read();   //read the position of the encoder
  if (neuePosition != altePosition){    
    if(neuePosition < altePosition){      //if the wheel turns left
      if(timer1){
        pos--;
        if(pos < 0){
          pos = len -1;
        }        
        timer1 = false;
        timer2 = false;
      }else {
        timer1 = true;
      }
    }else {   
       if(timer2){
        pos++;
        if(pos == len){
          pos = 0;
        }
        timer2 = false;
        timer1 = false;
       }else{
        timer2 = true;
       }
    }
    altePosition = neuePosition;
    return pos;
  }
  return -1;
}

void SetLED(int i, int r, int g, int b){
  aktColor[i][0] = r;
  aktColor[i][1] = g;
  aktColor[i][2] = b;
}

void ResetLEDs(){
  for(int i = 0;i< NUMPIXELS; i++){
    SetLED(i,0,0,0);
  }
  pixels.clear();
}

void lightAll(int r, int g, int b) {
  for(int i = 0; i < NUMPIXELS; i++){
    SetLED(i,r,g,b);
  }
}
