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

//Rotary Encoder
const int CLK = 6;//Datapin Encoder         
const int DT = 5; //Datepin Encoder
const int SW = 2; //Button des Encoders


int newpos = 0;
int oldpos = 0;   //Puffer for Encoder
int pos = 0;    //the selected Led
int  brightness = 255; //Helligkeit der LEDs
long myTimeout = 5000;    //timer when the lights should turn off
long altePosition = -999;   //position for the rotary encoder
bool timer1 = false;      //State for turning left //I use this variables to check that you have to move two incerements to get one move in the volume 
bool timer2 = false;      //State for turning right

//Rainbow-Mode
uint16_t r, m;

//Felix-Mode
byte florian[3][3] = {{0,255,0},{255,255,0},{255,0,0}};
int volumeInProzent = 0;
bool calibrated = false;

//Settings-menu
int menu = 0;
int mode = 1; //Modi: 0 = Normal Only Color with Truning, 1 = A constant rotating Pixel, 2 = colorWipes, 3 = rainbow, 4 = rainbowCycle, 5 = theaterChaseRainbow


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
  EncoderSwitch.attachLongPressStop(longPress);
  //attachInterrupt(digitalPinToInterrupt(SW), checkTicks, CHANGE);

  //LEDs
  pixels.begin();
  row = EEPROM.read(0);

  for(int i = 0; i<50;i++){
    Consumer.write(MEDIA_VOL_DOWN);
  }
  Consumer.write(MEDIA_VOL_UP);
  Consumer.write(MEDIA_VOL_UP);
  volumeInProzent = 4;
}

void loop(){ 
  if(menu == 0){    //Volume-Control
    int i = TurnEncoder(8);
    if(i != -1){
      newpos = i;
      //pixels.setPixelColor(oldpos, pixels.Color(0,0,0));
      //pixels.setPixelColor(i, pixels.Color(color[row][0],color[row][1],color[row][2]));      
      brightness = 255;              
      if(newpos > oldpos && newpos != 7 && oldpos != 0 || newpos == 0 && oldpos == 7){     
        //Serial.println("VolumeDown");
        Consumer.write(MEDIA_VOL_DOWN);
        volumeInProzent -= 2;
        if(volumeInProzent < 0){
          volumeInProzent = 0;
        }
      }else if(newpos < oldpos && newpos != 0 && oldpos != 7 || newpos == 7 && oldpos == 0){
        //Serial.println("VolumeUp");
        Consumer.write(MEDIA_VOL_UP);
        volumeInProzent += 2;
        if(volumeInProzent>100) {
          volumeInProzent = 100;
        }
      } 
    }
  } else if (menu == 1) {   //Color-Selection
    int i = TurnEncoder(25);
    if(i != -1){
      row = i;
      lightAll(color[row][0],color[row][1],color[row][2]);
    }
  } else if (menu == 2) {   //Mode-Selection
    int i = TurnEncoder(31);
    if(i != -1){
      mode = i;
      brightness = 255;
    }
  }
  showPixels();
  EncoderSwitch.tick();
  delay(10); 
}

void showPixels(){
  if((menu == 0 || menu == 2 )&& mode > 0 && mode < 8){
    int value = 0;
    if(menu == 0){
      value = newpos;
    } else if(menu == 2){
      value = mode;
    }
    if(value != -1 && (menu == 2 || menu == 0)){
        pixels.setPixelColor(oldpos, pixels.Color(0,0,0));
        pixels.setPixelColor(value, pixels.Color(color[row][0],color[row][1],color[row][2]));
        oldpos = value;
    }
    if(brightness <= 1){
      ResetLEDs();
    }
    for(int i = 0; i < NUMPIXELS; i++){
      if(pixels.getPixelColor(i) != pixels.Color(0,0,0)){
        brightness = brightness - 1;
        i = NUMPIXELS;
      }
    }
  }else if((menu == 0 || menu == 2)&& mode > 8 && mode < 16){
     for(r = 0; r < NUMPIXELS; r++) {
      pixels.setPixelColor(r, Wheel((r+m) & 255));
    }
    m++;
    if(m >= 256) {
      m = 0;
    }
    oldpos = newpos;
  }else if((menu == 0 || menu == 2) && mode > 16 && mode < 24){
     for(r = 0; r < NUMPIXELS; r++) {
      pixels.setPixelColor(r,Wheel(((r * 256 / pixels.numPixels()) + m) & 255));
    }
    m++;
    if(m >= 256*5) {
      m = 0;
    }
    oldpos = newpos;
  } else if((menu == 0 || menu == 2) && mode > 24 && mode < 32){
      int value = 0;
      if(menu == 0){
        value = newpos;
      } else if(menu == 2){
        value = mode - 24;
      }
      Serial.println(value);
      if(value != -1 && (menu == 2 || menu == 0)){
        int i = 0;
        if(volumeInProzent >= 0 && volumeInProzent <= 33){
          i = 0;
        }else if (volumeInProzent >= 34 && volumeInProzent <= 66){
          i = 1;
        }else if (volumeInProzent >= 67 && volumeInProzent <= 100){
          i = 2;
        }
        Serial.println(volumeInProzent);
        if(value == 7 && oldpos == 0){
          pixels.clear();
        }else if(value == 0 && oldpos == 7){
          pixels.fill(pixels.Color(florian[i][0],florian[i][1],florian[i][2]));
        }else if(value > oldpos){
          pixels.setPixelColor(oldpos, pixels.Color(0,0,0));
        }else{
          pixels.setPixelColor(value, pixels.Color(florian[i][0],florian[i][1],florian[i][2]));
        }
        oldpos = value;
      }
  }
  
  if(mode == 8 || mode == 16 || mode == 24 || mode == 32 || mode == 0 && menu == 2){
    ResetLEDs();
  }
  
  pixels.setBrightness(brightness);
  pixels.show();
}

void clickt() {
  //Serial.println("Click");
  if(menu == 0){
    //Consumer.write(MEDIA_VOL_MUTE);
    //Serial.println("Mute");
    brightness = 255;
    pixels.fill(pixels.Color(color[row][0],color[row][1],color[row][2]));
    delay(700); 
  } else if(menu == 1){
    Serial.println("leave with selected color");
    EEPROM.update(0,row);
    menu = 0;
    pos = 0;
    newpos = 0;
    oldpos = 0;
    altePosition = -999;
    delay(100);   
  } else if(menu == 2){
    Serial.print("leave with selected mode: ");
    Serial.println(mode);
    menu = 0;
    r = 0;
    m = 0; 
    pos = 0;
    newpos = 0;
    oldpos = 0;
    altePosition = -999;
    delay(100);   
  }
}

void longPress() {
  menu++;
  pos = 0;
  newpos = 0;
  oldpos = 0;
  brightness = 255;
  ResetLEDs();
  if(menu == 1){
    pos = row;
    Serial.println(pos);
    lightAll(color[pos][0],color[pos][1],color[pos][2]);
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
          pos = len - 1;
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

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void ResetLEDs(){
  pixels.clear();
}

void lightAll(int r, int g, int b) {
  pixels.fill(pixels.Color(r,g,b));
}
