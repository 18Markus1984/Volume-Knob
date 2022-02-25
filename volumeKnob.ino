#include <HID-Project.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
#include <OneButton.h>
#include <EEPROM.h>

#define PIN       7 //Datapin for the LEDs
#define NUMPIXELS 9 //number of LEDs in the row

//Color Options
byte color[25][3] = {{255,0,0},{255,64,0},{255,128,0},{255,191,0},{255,255,0},{191,255,0},{128,255,0},{64,255,0},{0,255,0},{0,255,64},{0,255,128},{0,255,191},{0,255,255},{0,191,255},
{0,128,255},{0,64,255},{0,0,255},{64,0,255},{128,0,255},{191,0,255},{255,0,255},{255,0,191},{255,0,128},{255,0,64},{255,0,0}};  //Array with the different color-values would be better to do this with a function
int row = 0;  //pointer, which color is selected ##das is kein Pointer

//Rotary Encoder
const int CLK = 6;//Clockpin Encoder         
const int DT = 5; //Datepin Encoder
const int SW = 2; //Button of the Encoders
int pos = 0;    //the variable for the Encoder-function to give back ## ?
long oldPosition = -999;   //position for the rotary encoder
bool turnLeft = false;      //State for turning left //I use this variables to check that you have to move two incerements to get one move in the volume 
bool turnRight = false;      //State for turning right

//variable for the first mode
int newPos = 0;   //the selected LED that should light up
int oldpos = 0;   //the last lit up LED, You need this to turn the LED off after you jump to the next one ##Name nicht eindeutig, oldPosition-oldpos

//Rainbow-Mode
uint16_t r, m;

//Florian-Mode
byte florian[3][3] = {{0,255,0},{255,255,0},{255,0,0}}; //green yellow red for the traffic light mode
int volumeInPercent = 0;

//Settings-menu
int menu = 0; //0 = Sound-Mode, 1 = color selection, 2 = modi-selection
int mode = 1; //1-7 = Rotating-Pixel, 9-15 = rgb-wheel fade, 17-23 = rgb-wheel with the moving colors, 25-31 = percent red, yellow, green for different noise levels, 33-39 = simular to the first mode but you have 3 LEDs and the pixels stay on
int brightness = 255; //of LEDs from 0-255

//Generall Objects
OneButton EncoderSwitch(SW,true);   //Libary that adds functions for LongPress, DoubleClick etc.
Encoder myEncoder(DT,CLK);      
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup();
void loop();
void volumeControl();
void colorSelection();
void modeSelection();
void showPixels();
void onePixelFade();
void rgbWheelFade();
void rgbWheelMoving();
void noiseLevels();
void pixelFadeStay();
void clicked();
void longPress();
int TurnEncoder(int len);
uint32_t Wheel(byte WheelPos);
void resetLEDs();
void lightAll(int r, int g, int b);

void setup(){
  Serial.begin(9600);
  Serial.println("Consumer setup...");
  Consumer.begin();

  //Rotary Encoder with Switch
  Serial.println("Rotary Encoder setup...");
  pinMode(SW, INPUT);
  EncoderSwitch.attachClick(clicked);
  EncoderSwitch.attachLongPressStop(longPress);

  //LEDs
  pixels.begin();

  //Read saved settings
  row = EEPROM.read(0);   
  mode = EEPROM.read(1);

  for(int i = 0; i<50;i++){   //for a special mode doesn't work till now ##geht hier ein direktes auf 0 setzten nicht ?
    Consumer.write(MEDIA_VOL_DOWN);
  }
  Consumer.write(MEDIA_VOL_UP);
  Consumer.write(MEDIA_VOL_UP);
  volumeInPercent = 4;
}

void loop(){ 
  if(menu == 0){  
    volumeControl();
  } 
  else if (menu == 1) {   
    colorSelection();
  } 
  else if (menu == 2) {  
    modeSelection();
  }
  showPixels();
  EncoderSwitch.tick();   //the libary reads the encoder value and gives back an int ##dat gibt garnischt zurück. Also zumindest speicherst du es nicht ab
  delay(10); 
}

void volumeControl(){
  int i = TurnEncoder(NUMPIXELS);
    if(i != -1){
      newPos = i; 
      brightness = 255;              
      if(newPos > oldpos && newPos != NUMPIXELS-1 && oldpos != 0 || newPos == 0 && oldpos == NUMPIXELS-1){ //solves the issue from 0 to 7 and 7 to 0     
        Consumer.write(MEDIA_VOL_UP);   
        volumeInPercent -= 2;
        if(volumeInPercent < 0){
          volumeInPercent = 0;
        }
      }else if(newPos < oldpos && newPos != 0 && oldpos != NUMPIXELS-1 || newPos == NUMPIXELS-1 && oldpos == 0){
        Consumer.write(MEDIA_VOL_DOWN);
        volumeInPercent += 2;
        if(volumeInPercent>100) {
          volumeInPercent = 100;
        }
      } 
    }
}

void colorSelection(){
  int i = TurnEncoder(25);    //turns the Encoder with the length of the array
    if(i != -1){
      row = i;
      lightAll(color[row][0],color[row][1],color[row][2]);
    }
}

void modeSelection(){
  int i = TurnEncoder(NUMPIXELS*5);  //6 Positions for every mode with a blank spot between the modes
    if(i != -1){
      mode = i;
      brightness = 255;
    }
}
/*
void showPixels(){    //function for the different Modis you could select
    if((menu == 0 || menu == 2 )&& mode > 0 && mode < 8){   //The first mode with only one pixel lit up that fades the color after some time
      onePixelFade();
    }
    else if((menu == 0 || menu == 2)&& mode > 8 && mode < 16){   //the rgb-wheel fade
      rgbWheelFade();
    }
    else if((menu == 0 || menu == 2) && mode > 16 && mode < 24){//the rgb-wheel with the moving colors
      rgbWheelMoving();
    } 
    else if((menu == 0 || menu == 2) && mode > 24 && mode < 32){  //the mode for the percent red, yellow, green for different noise levels
        noiseLevels();
    }
    else if((menu == 0 || menu == 2) && mode > 32 && mode < 40){ //simular to the first mode but you have 3 LEDs and the pixels stay on.
      pixelFadeStay();
    }
    if(mode == 8 || mode == 16 || mode == 24 || mode == 32 || mode == 40 || mode == 0 && menu == 2){  //The blank spots in the mode-menu where no animations should be shown
      resetLEDs();
    }
  pixels.setBrightness(brightness);
  pixels.show();    //Only time I update the pixels
}
*/

void showPixels(){    //function for the different Modis you could select
    if((menu == 0 || menu == 2 )&& mode > 0 && mode < NUMPIXELS){   //The first mode with only one pixel lit up that fades the color after some time
      onePixelFade();
    }
    else if((menu == 0 || menu == 2)&& mode > NUMPIXELS && mode < NUMPIXELS*2){   //the rgb-wheel fade
      rgbWheelFade();
    }
    else if((menu == 0 || menu == 2) && mode > NUMPIXELS*2 && mode < NUMPIXELS*3){//the rgb-wheel with the moving colors
      rgbWheelMoving();
    } 
    else if((menu == 0 || menu == 2) && mode > NUMPIXELS*3 && mode < NUMPIXELS*4){  //the mode for the percent red, yellow, green for different noise levels
        noiseLevels();
    }
    else if((menu == 0 || menu == 2) && mode > NUMPIXELS*4 && mode < NUMPIXELS*5){ //simular to the first mode but you have 3 LEDs and the pixels stay on.
      pixelFadeStay();
    }
    if(mode == NUMPIXELS || mode == NUMPIXELS*2 || mode == NUMPIXELS*3 || mode == NUMPIXELS*4 || mode == NUMPIXELS*5 || mode == 0 && menu == 2){  //The blank spots in the mode-menu where no animations should be shown
      resetLEDs();
    }
  pixels.setBrightness(brightness);
  pixels.show();    //Only time I update the pixels
}

void onePixelFade(){
  int value = 0;
  if(menu == 0){
        value = newPos;
      } 
      else if(menu == 2){
        value = mode;
      }
      if(value != -1 && (menu == 2 || menu == 0)){
          pixels.setPixelColor(oldpos, pixels.Color(0,0,0));
          pixels.setPixelColor(value, pixels.Color(color[row][0],color[row][1],color[row][2]));
          oldpos = value;
      }
      if(brightness <= 1){
        resetLEDs();
      }
      for(int i = 0; i < NUMPIXELS; i++){
        if(pixels.getPixelColor(i) != pixels.Color(0,0,0)){
          brightness = brightness - 1;
          i = NUMPIXELS;
        }
      }
}

void rgbWheelFade(){
  for(r = 0; r < NUMPIXELS; r++) {
        pixels.setPixelColor(r, Wheel((r+m) & 255));
      }
      m++;
      if(m >= 256) {
        m = 0;
      }
      oldpos = newPos;
}

void rgbWheelMoving(){
  for(r = 0; r < NUMPIXELS; r++) {
        pixels.setPixelColor(r,Wheel(((r * 256 / pixels.numPixels()) + m) & 255));
      }
      m++;
      if(m >= 256*5) {
        m = 0;
      }
      oldpos = newPos;
}

void noiseLevels(){
  int i, value = 0;
  if(menu == 0){
          value = newPos;
        } else if(menu == 2){
          value = mode - NUMPIXELS*3;
        }
        Serial.println(value);
        if(value != -1 && (menu == 2 || menu == 0)){
          if(volumeInPercent >= 0 && volumeInPercent <= 33){
            i = 0;
          }else if (volumeInPercent >= 34 && volumeInPercent <= 66){
            i = 1;
          }else if (volumeInPercent >= 67 && volumeInPercent <= 100){
            i = 2;
          }
          Serial.println(volumeInPercent);
          if(value == NUMPIXELS-1 && oldpos == 0){
            pixels.clear();
          }else if(value == 0 && oldpos == NUMPIXELS){
            pixels.fill(pixels.Color(florian[i][0],florian[i][1],florian[i][2]));
          }else if(value > oldpos){
            pixels.setPixelColor(oldpos, pixels.Color(0,0,0));
          }else{
            pixels.setPixelColor(value, pixels.Color(florian[i][0],florian[i][1],florian[i][2]));
          }
          oldpos = value;
        }
}

void pixelFadeStay(){
  int puffer = 0, value = 0;
  if(menu == 0){
        value = newPos;
      } 
      else if(menu == 2){
        value = mode - NUMPIXELS*4;
      }
      if(value != -1 && (menu == 2 || menu == 0)){
          pixels.fill(pixels.Color(0,0,0));
          for(int i = 0; i<3;i++) {
            puffer = value+i;
            if(puffer > NUMPIXELS-1){
              puffer = i-1;
            }
            pixels.setPixelColor(puffer, pixels.Color(color[row][0],color[row][1],color[row][2]));
          }
          oldpos = value;
      }
}

void clicked() {   //Select Button
  if(menu == 0){
    Consumer.write(MEDIA_VOL_MUTE);
    brightness = 255;
    pixels.fill(pixels.Color(color[row][0],color[row][1],color[row][2]));
    delay(700); 
  } 
  else if(menu == 1){   //leave the color-selection menu and select the color 
    Serial.println("leave with selected color");
    EEPROM.update(0,row);
    menu = 0;
    pos = 0;
    newPos = 0;
    oldpos = 0;
    oldPosition = -999;
    delay(100);    
  }
  else if(menu == 2){    //leave the mode-menu and select the mode
    Serial.print("leave with selected mode: ");
    Serial.println(mode);
    EEPROM.update(1,mode);
    pixels.clear();
    menu = 0;
    r = 0;
    m = 0; 
    pos = 0;
    newPos = 0;
    oldpos = 0;
    oldPosition = -999;
    delay(100);   
  }
}

void longPress() {    //Menu button
  menu++;   //jump into the next menu up
  pos = 0;
  newPos = 0;
  oldpos = 0;
  brightness = 255;
  resetLEDs();
  if(menu == 1){
    lightAll(color[row][0],color[row][1],color[row][2]);
  } 
  else if(menu == 2){
    row = EEPROM.read(0); 
    delay(100);  
  }
  if(menu >= 3){
    menu = 0;
    mode = EEPROM.read(1);
    r = 0;
    m = 0; 
    pos = 0;
    newPos = 0;
    oldpos = 0;
    oldPosition = -999;
  }
  Serial.print(menu);
  Serial.println("LongClick");
}

int TurnEncoder(int len) {      //The function that reads the data from the encoder relative to the given size. You can put in the len as the max number of points for one rotation
  long newPosition = myEncoder.read();   //read the position of the encoder
  if (newPosition != oldPosition){    //compare it to the old position
    if(newPosition > oldPosition){      //if the wheel turns left
      if(turnLeft){       //turnLeft/Right are for reducing the number of points by half so you have to pass two points to get one, because the encoder roation points are sometimes pretty small
        pos++;
        if(pos == len){
          pos = 0;
        }        
        turnLeft = false;
        turnRight = false;
      }
      else {
        turnLeft = true;
      }
    }
    else {   
       if(turnRight){
        pos--;
        if(pos < 0){
          pos = len - 1;
        }
        turnRight = false;
        turnLeft = false;
       }else{
        turnRight = true;
       }
    }
    oldPosition = newPosition; 
    return pos;
  }
  return -1;
}

uint32_t Wheel(byte WheelPos) {   //From the Adafruit Libyary a function for a rgb circle
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

void resetLEDs(){
  pixels.clear();
}

void lightAll(int r, int g, int b) {
  pixels.fill(pixels.Color(r,g,b));
}
