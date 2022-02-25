#include <HID-Project.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
#include <OneButton.h>
#include <Thread.h>
#include <ThreadController.h>
//#include <EEPROM.h>

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

int pos = 0;    //the selected Led
int  brightness = 255; //Helligkeit der LEDs
long myTimer = 0;
long myTimeout = 5000;    //timer when the lights should turn off
bool timer1 = false;      //State for turning left
bool timer2 = false;      //State for turning right
long altePosition = -999;   //position for the rotary encoder

//Settings-menu
int menue = 0;
int selected = 0;

//Settings
int sec = 0;
uint8_t wait = 20;
int NumbPixel = 0; //Max Pixel = 7 und Min Pixel = 1
int modi = 0; //Modi: 0 = Normal Only Color with Truning, 1 = A constant rotating Pixel, 2 = colorWipes, 3 = rainbow, 4 = rainbowCycle, 5 = theaterChaseRainbow

//"Threads"
ThreadController controll = ThreadController();
Thread rainbowThread = Thread();
Thread rainbowCycleThread = Thread();
Thread colorWipeThread = Thread();

OneButton EncoderSwitch(SW,true);
Encoder meinEncoder(DT,CLK);  
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup(){
  Serial.begin(9600);
  Consumer.begin();

  //Rotary Encoder with Switch
  pinMode(SW, INPUT);
  EncoderSwitch.attachClick(clickt);
  EncoderSwitch.attachDoubleClick(doubleclick);

  attachInterrupt(digitalPinToInterrupt(SW), checkTicks, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(CLK), checkTicks, CHANGE);

  //LEDs
  pixels.begin();

  //"Threads" to run the LEDs while using the Rotary Encoder
  rainbowThread.onRun(rainbow);
  rainbowThread.setInterval(250);

  rainbowCycleThread.onRun(rainbowCycle);
  rainbowCycleThread.setInterval(500);

  colorWipeThread.onRun(colorWipe);
  colorWipeThread.setInterval(350);

  //Add the threads to the controller
  controll.add(&rainbowThread);
  controll.add(&rainbowCycleThread);
  controll.add(&colorWipeThread);
}

void loop(){
  
  if(menue == 0){
    ChangeVolume();
  }
  else if (menue == 1)  {
    ChangeMainColor();
  }
  else if (menue == 2) {
    ChangeModi();
  }
  
  EncoderSwitch.tick();
  if(modi != 0){
    controll.run();
  }
}

void checkTicks()
{
  EncoderSwitch.tick();
}

void clickt() {
  if(menue == 0){
    Serial.println("Mute"); 
    Consumer.write(MEDIA_VOL_MUTE);
    lightAllFade(1500);
  }
  if(menue == 1) {
    Serial.println("Switch to Normal-Modus");
    row = selected;
    menue = 0;
  }
  if(menue == 2){
    menue = 0;
  }
}

void doubleclick() {
  if(menue == 0){
    Serial.println("\nOpen Settings-Color");
    modi = 0;
    pixels.clear();
    menue = 1;
    selected = row;
  }else if(menue == 1) {
    pixels.clear();
    pixels.show();
    menue = 2;
    Serial.println("Switch to Modi-Menu");
  }else if(menue == 2) {
    Serial.print("Leave Settings");
    modi = 0;
    menue = 0;
  }
}

void ChangeVolume() {
  long neuePosition = meinEncoder.read();   //read the position of the encoder
  if (neuePosition != altePosition){    
    Serial.print(neuePosition);
    pixels.setPixelColor(pos, pixels.Color(color[row][0], color[row][1], color[row][2]));
    brightness = 255;
    pixels.setBrightness(255);
    pixels.show();   // Send the updated pixel colors to the hardware.
    if(neuePosition < altePosition){      //if the wheel turns left
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
    pixels.clear(); // Set all pixel colors to 'off'   
    myTimer = millis();
    altePosition = neuePosition;    
  }
  if (millis() > myTimeout + myTimer) {   //turns of the Led that was turned on off after the timer ends
    myTimer = millis();
    pixels.clear();
    pixels.show();
  }
}

void ChangeMainColor(){
  long neuePosition = meinEncoder.read();   //read the position of the encoder
  lightAll();
  if (neuePosition != altePosition){    
    //Serial.print(neuePosition);
    if(neuePosition < altePosition){      //if the wheel turns left
        if(timer1){
          selected--;
          if(selected < 0){
            selected = 24;
          }        
          timer1 = false;
        }else {
          timer1 = true;
        }
    }else {   
        if(timer2){
          selected++;
          if(selected == 25){
            selected = 0;
          }
          timer2 = false;
        }else{
          timer2 = true;
        }
    } 
    altePosition = neuePosition;    
  }
}

void ChangeModi(){
  long neuePosition = meinEncoder.read();   //read the position of the encoder
  Serial.print("Selected Modi: ");
  Serial.println(modi);
  if (neuePosition != altePosition){    
    Serial.print(neuePosition);
    if(neuePosition > altePosition){      //if the wheel turns left
        if(timer1){
          modi--;
          if(modi < 0){
            modi = 5;
          }
          else if (modi > 5){
            modi = 0;        
          }
          timer1 = false;
        }else {
          timer1 = true;
        }
    }else {   
        if(timer2){
          modi++;
          if(modi >= 6){
            selected = 0;
          }
          timer2 = false;
        }else{
          timer2 = true;
        }
    } 
    altePosition = neuePosition;    
  }
}

void lightAll(){ //Used in the Colorselection-menu
  pixels.clear();
  for(int i = 0; i < 9; i++) {
    pixels.setPixelColor(i, pixels.Color(color[selected][0], color[selected][1], color[selected][2]));
  }
  brightness = 255;
  pixels.setBrightness(255);
  pixels.show();
}

void lightAllFade(int sec) {
  for(int i = 0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(color[row][0], color[row][1], color[row][2]));
  }
  pixels.setBrightness(255);
  pixels.show();   // Send the updated pixel colors to the hardware.
  for(int i = 255; i>0; i--){
    pixels.setBrightness(i);
    pixels.show();
    delay(sec/255);
  }
  pixels.setBrightness(255);
  pixels.clear();
}

void rotatingPixel(){
  if(modi == 1) {
    
  }
}

void colorWipe() {
  if(modi == 2) {
    for(int k=0;k<8;k++){
      for(int i=0; i<8; i++) {
        int puffer = i+k;
        if(puffer > 7){
          puffer = 0 + k - 1;
        }
        Serial.println(puffer);
        pixels.setPixelColor(puffer, pixels.Color(color[row][0], color[row][1], color[row][2]));
        pixels.show();
        delay(300);
        if(modi != 2){
           k = 8;
        }
      }
      pixels.clear();
      pixels.show();
    }
  }
  pixels.clear();
}

void rainbow() {
  if(modi == 3){
    Serial.println("rainbow");
    uint16_t i, j;
    for(j=0; j<256; j++) {
        for(i=0; i<pixels.numPixels(); i++) {
          pixels.setPixelColor(i, Wheel((i+j) & 255));
          if(modi != 3){
           j = 256;
          }
        }
        pixels.show();
        delay(wait);
    }
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle() {
  if(modi == 4){
    Serial.println("rainbowCycles");
    uint16_t i, j;
    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< pixels.numPixels(); i++) {
          pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
          if(modi != 4){
            j = 256;
          }
        }
        if(modi != 4){
            j = 256;
        }
        pixels.show();
        delay(wait);
      
    }
    if(menue == 2){
      modi = 5;
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
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

//MultibuttonClick <-- Button Click Libary --> Zum Fabrewechseln(Erledigt), Modus wechseln
//Langsam Ausklingende Lichter
//Modi maybe in einer zweiten Datei
//
