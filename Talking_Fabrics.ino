#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            6
#define NUMPIXELS      144
 

const int xBee01 = 2;
const int xBee02 = 3;
const int xBee03 = 4;
int xBee01_state;
int xBee02_state;
int xBee03_state;
const int magnet = 5;
const int ledStripPin = 6;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
 
void setup()
{
  Serial.begin(9600);
  pinMode(xBee01, INPUT);
  pinMode(xBee02, INPUT);
  pinMode(xBee03, INPUT);
  pinMode(ledStripPin, OUTPUT);
  pinMode(magnet, OUTPUT);
  //Magnets should be on by default
  digitalWrite(magnet, HIGH);
  strip.begin();
}
 
void loop()
{
  //Pin 2 fans
  xBee01_state = digitalRead(xBee01);
  //Pin 3 magnets
  xBee02_state = digitalRead(xBee02);
  //Pin 4 leds
  xBee03_state = digitalRead(xBee03);
  
  Serial.println("Magnet pins state is:");
  Serial.println(xBee02_state);
  Serial.println("Fans pin state is: ");
  Serial.println(xBee01_state);
  delay(100);
  
  //xBee01 controls the fans, just supplies a signal to a mosfet 
  if (xBee03_state == HIGH){
    Serial.println("Fans on");
  }  
  
  //xBee02 controls the magnets, actually turns 
  //them off if detects a signal from the xBee02 glove
  if (xBee03_state == HIGH) {
    Serial.println("Magnet Off");
    digitalWrite(magnet, LOW);
  }
  
  //xBee03 controls the LEDs, this one works right now, turns them on if signal detected
  //No signal detected, turns all the LEDs on the strip off
  if (xBee03_state == HIGH) {
    Serial.println("LED ON");
    for (int i = 0; i < NUMPIXELS; i++)
    {
      strip.setPixelColor(i, 255);
      strip.show();
      delay(10);
    }
  } else {
    for (int i = 0; i < NUMPIXELS; i++) {
      strip.setPixelColor(i, 0);
      strip.show();
    }
  }
}



//These two functions are for the xBee03 controlling the Neopixel LEDs
//they both control potential patterns that can be used by the Neopixel strip.
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
