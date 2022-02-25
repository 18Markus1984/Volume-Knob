#include <Adafruit_NeoPixel.h>
#define PIN       7
#define NUMPIXELS 8
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = -1; i<10; i++){
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    //Serial.println(i);
    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(1000); // Pause before next pass through loop
  
  }
}
