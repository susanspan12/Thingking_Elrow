//****************************************************************************************************************************
//  Thingking Studio (Pty) Ltd
//  Product: Elrow x Absolut Cathedral
//****************************************************************************************************************************
// This code runs on the Teensy 3.2 marked "1" mounted in the in the bottle's electronics housing.
// This controller must be connected to the USB cable provided during operation to receive comms from the touchscreen computer.
// Please take note that the controller needs to be externally powered by the 5V supply before it can be programmed. 

#include "FastLED.h"

//----#DEFINES--------------------------------------------------------
#define NUM_LEDS 145 

//****PIN DEFINES*****************************************************
#define SPI_DATA_PIN 11
#define SPI_CLK_PIN 13
#define DEBUG_BUTTON 14

//*****ANIMATIONS*****************************************************
#define ANIMATION_TIME 30000
#define DEFAULT_TIME 60000

#define FORWARD 0
#define BACKWARD 1
#define MEDIUM 50

#define DEBUG_MODE false

//----Variable definitions--------------------------------------------------------

//****SERIAL COMMS*****************************************************

//Run animation 1 to 5 
byte ANIMATION_BYTE_1 = '1';
byte ANIMATION_BYTE_2 = '2';
byte ANIMATION_BYTE_3 = '3';
byte ANIMATION_BYTE_4 = '4';
byte ANIMATION_BYTE_5 = '5';
byte YES_BYTE = 'Y';
byte NO_BYTE = 'N';

//****Colour HSV******************************************************
byte hue = 0;
byte saturation = 255;
byte value = 100;
byte animation_byte = 'x';

boolean direction = FORWARD;    //for animations

//****LED strip*******************************************************

int THEME_HUE_BLUE = 0;
int THEME_HUE_PURPLE = 230;
int THEME_HUE_PINK = 190;
int THEME_HUE_AQUA = 35;
int THEME_HUE_YELLOW = 140;

int theme_hues[] = {THEME_HUE_BLUE, THEME_HUE_PURPLE, THEME_HUE_PINK, THEME_HUE_AQUA, THEME_HUE_YELLOW};

int chsv_orange = 150;

int random_index_led_1 = -1;
int random_index_led_2 = -1;
float animation_timer = -1;
float default_timer = -1;

CRGB leds[NUM_LEDS];

//-------------------------------Function prototypes-----------------------------------------------------------------------------
void rainbow(int cycles, int speed);
void stripes(CRGB c1, CRGB c2, int width);
void colorWipe(CHSV c, int speed_);
void allColor();
void someColour(CHSV c, int start_led, int end_led);
void disolve(CHSV c, int speed_);
void moving_strip(CHSV c, int speed_, int width_);
void default_animation_1();
void default_animation_2();
void default_animation_3();
void default_animation_4();
void default_animation_5();
void default_animation_6();
void default_animation_7();

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(2000);  //Long delay for boot stability
  FastLED.addLeds<APA102, SPI_DATA_PIN, SPI_CLK_PIN, RGB, DATA_RATE_MHZ(10)>(leds, NUM_LEDS);
}

void loop() {
  default_animation_1();    //Rainbow scrolling
  default_animation_2();    //Random lines of colour appearing
  default_animation_3();    //Colour wipe using theme hues
  default_animation_4();    //moving strip of light, changing colour according to theme hues
  default_animation_5();    //Sparkle/flashes
  default_animation_6();    //Rainbow colours fade in and out
  default_animation_7();    //Sparkle/flashes
  default_animation_4();
  default_animation_2();
}

//--------------------Serial comms------------------------------------------------------
void check_serial_incoming() {
  if (Serial.available()) {
    animation_byte = Serial.read();
    if (animation_byte == ANIMATION_BYTE_1) {
      Serial.write(YES_BYTE);
      Serial1.write(animation_byte);
      animation_1();
    }
    else if (animation_byte == ANIMATION_BYTE_2) {
      Serial.write(YES_BYTE);
      Serial1.write(animation_byte);
      animation_2();
    }
    else if (animation_byte == ANIMATION_BYTE_3) {
      Serial.write(YES_BYTE);
      Serial1.write(animation_byte);
      animation_3();
    }
    else if (animation_byte == ANIMATION_BYTE_4) {
      Serial.write(YES_BYTE);
      Serial1.write(animation_byte);
      animation_4();

    }
    else if (animation_byte == ANIMATION_BYTE_5) {
      Serial.write(YES_BYTE);
      Serial1.write(animation_byte);
      animation_5();
    }
    else {
      Serial.write(NO_BYTE);
    }
  }
  while (Serial.available()) {  //clear buffer
    Serial.read();
  }
}

//----------------------Animations-------------------------------------
//----------------------Default 1-------------------------------------
void default_animation_1() {
  colorWipe(CHSV (0, 0, 0), 1);
  default_timer = millis();
  while (millis() - default_timer < DEFAULT_TIME) {
    if (DEBUG_MODE) {
      Serial.println('default 1');
    }
    rainbow(10, 8);
  }
}

//----------------------Default 2-------------------------------------
void default_animation_2() {
  colorWipe(CHSV (0, 0, 0), 1);
  default_timer = millis();
  while (millis() - default_timer < DEFAULT_TIME) {
    if (DEBUG_MODE) {
      Serial.println('default 7');
    }
    //    while (abs(random_index_led_1 - random_index_led_2) < 5) {
    random_index_led_1 = random(NUM_LEDS);
    random_index_led_2 = random(NUM_LEDS);
    //    }
    someColour(CHSV (random(0, 255), 255, 255), min(random_index_led_1, random_index_led_2), max(random_index_led_1, random_index_led_2));
    delay(300);
  }
}

//----------------------Default 3-------------------------------------
void default_animation_3() {
  colorWipe(CHSV (0, 0, 0), 1);
  default_timer = millis();
  while (millis() - default_timer < DEFAULT_TIME) {
    if (DEBUG_MODE) {
      Serial.println('default 3');
    }
    for (int i = 0; i < 5; i++) {
      colorWipe(CHSV (theme_hues[i], 255, 255), 5);
    }
  }
}

//----------------------Default 4-------------------------------------
void default_animation_4() {
  colorWipe(CHSV (0, 0, 0), 1);
  default_timer = millis();
  while (millis() - default_timer < DEFAULT_TIME) {
    if (DEBUG_MODE) {
      Serial.println('default 6');
    }
    for (int i = 0; i < 5; i++) {
      moving_strip(CHSV (theme_hues[i], 255, 255), 10, 50);
    }
  }
}

//----------------------Default 5-------------------------------------
void default_animation_5() {
  colorWipe(CHSV (0, 0, 0), 1);
  default_timer = millis();
  while (millis() - default_timer < DEFAULT_TIME) {
    if (DEBUG_MODE) {
      Serial.println('default 2');
    }
    lightning(CHSV (THEME_HUE_AQUA, 200, 255), 20, 50, MEDIUM / 1.5);
  }
}

//----------------------Default 6-------------------------------------
void default_animation_6() {
  int i = 0;
  colorWipe(CHSV (0, 0, 0), 1);
  allColor(CHSV (THEME_HUE_BLUE, 255, 255));
  default_timer = millis();
  while (millis() - default_timer < DEFAULT_TIME) {
    if (DEBUG_MODE) {
      Serial.println(i);
    }
    i += 20;
    disolve(CHSV (i, 255, 255), 5);
    disolve(CHSV (0, 0, 0), 5);
    if (i >= 255) {
      i = 0;
    }
  }
}

//----------------------Default 7-------------------------------------
void default_animation_7() {
  colorWipe(CHSV (0, 0, 0), 1);
  default_timer = millis();
  while (millis() - default_timer < DEFAULT_TIME) {
    if (DEBUG_MODE) {
      Serial.println('default 8');
    }
    lightning(CHSV(THEME_HUE_PURPLE, 255, 255), 20, 50, MEDIUM / 1.5);
    lightning(CHSV(THEME_HUE_BLUE, 255, 255), 20, 50, MEDIUM / 1.5);
  }
}

//----------------------Triggered Animations-------------------------------------
//----------------------Triggered Animation 1-------------------------------------
void animation_1() {
  animation_timer = millis();
  allColor(CHSV (THEME_HUE_BLUE, 255, 255));
  while (millis() - animation_timer < ANIMATION_TIME) {
    disolve(CHSV (chsv_orange, 255, 255), 10);
    disolve(CHSV (THEME_HUE_BLUE, 255, 255), 10);
  }
  colorWipe(CHSV (0, 0, 0), 1);
}

//----------------------Triggered Animation 2-------------------------------------
void animation_2() {
  colorWipe(CHSV (0, 0, 0), 1);
  animation_timer = millis();
  while (millis() - animation_timer < ANIMATION_TIME) {
    lightning(CHSV(THEME_HUE_PURPLE, 255, 255), 20, 50, MEDIUM);
  }
  colorWipe(CHSV (0, 0, 0), 1);
}

//----------------------Triggered Animation 3-------------------------------------
void animation_3() {
  animation_timer = millis();
  while (millis() - animation_timer < ANIMATION_TIME) {
    allColor(CHSV (THEME_HUE_YELLOW, 255, 255));
    delay(100);
    allColor(CHSV (THEME_HUE_BLUE, 255, 255));
    delay(100);
    allColor(CHSV (THEME_HUE_PURPLE, 255, 255));
    delay(100);
    allColor(CHSV (THEME_HUE_PINK, 255, 255));
    delay(100);
    allColor(CHSV (THEME_HUE_AQUA, 255, 255));
    delay(100);
  }
  colorWipe(CHSV (0, 0, 0), 1);
}

//----------------------Triggered Animation 4-------------------------------------
void animation_4() {
  animation_timer = millis();
  while (millis() - animation_timer < ANIMATION_TIME) {
    moving_strip(CHSV (THEME_HUE_AQUA, 255, 255), 2, 50);
  }
  colorWipe(CHSV (0, 0, 0), 1);
}

//----------------------Triggered Animation 5-------------------------------------
void animation_5() {
  animation_timer = millis();
  allColor(CHSV (THEME_HUE_YELLOW, 255, 255));
  while (millis() - animation_timer < ANIMATION_TIME) {
    disolve(CHSV (0, 0, 0), 6);
    disolve(CHSV (THEME_HUE_YELLOW, 255, 255), 6);
    colorWipe(CHSV (0, 0, 0), 3);
    colorWipe(CHSV (THEME_HUE_YELLOW, 255, 255), 3);
  }
  colorWipe(CHSV (0, 0, 0), 1);
}

//----------------------Animation functions-------------------------------------
void lightning(CRGB c, int simultaneous, int cycles, int speed) {
  int flashes[simultaneous];
  for (int i = 0; i < cycles; i++) {
    for (int j = 0; j < simultaneous; j++) {
      int idx = random(NUM_LEDS);
      flashes[j] = idx;
      leds[idx] = c;
    }
    FastLED.show();
    check_serial_incoming();
    delay(speed);
    for (int s = 0; s < simultaneous; s++) {
      leds[flashes[s]] = CRGB::Black;
    }
    delay(speed);
  }
}

void allColor(CHSV c) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = c;
  }
  FastLED.show();
  check_serial_incoming();
}

void someColour(CHSV c, int start_led, int end_led) {
  for (int i = start_led; i < end_led; i++) {
    leds[i] = c;
  }
  FastLED.show();
  check_serial_incoming();
}

// Random disolve colors
void disolve(CHSV c, int speed_) {
  int simultaneous = 5;
  int cycles = 80;
  for (int i = 0; i < cycles; i++) {
    for (int j = 0; j < simultaneous; j++) {
      int idx = random(NUM_LEDS);
      leds[idx] = c;
    }
    FastLED.show();
    check_serial_incoming();
    delay(speed_);
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = c;
  }
  FastLED.show();
  check_serial_incoming();
}

// Rainbow colors that slowly cycle across LEDs
void rainbow(int cycles, int speed) { // TODO direction
  if (cycles == 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = Wheel(((i * 256 / NUM_LEDS)) & 255);
    }
    FastLED.show();
    check_serial_incoming();
  }
  else {
    for (int j = 0; j < 256 * cycles; j++) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      }
      FastLED.show();
      check_serial_incoming();
      delay(speed);
    }
  }
}

// Sliding bar across LEDs
void moving_strip(CHSV c, int speed_, int width_) {
  //  CRGB c(147, 103, 0);
  // First slide the leds in one direction
  for (int i = 0; i <= NUM_LEDS - width_; i++) {
    for (int j = 0; j < width_; j++) {
      leds[i + j] = c;
    }
    FastLED.show();
    check_serial_incoming();
    // now that we've shown the leds, reset to black for next loop
    for (int j = 0; j < 5; j++) {
      leds[i + j] = CRGB::Black;
    }
    delay(speed_);
  }
  delay(50);
  // Now go in the other direction.
  for (int i = NUM_LEDS - width_; i >= 0; i--) {
    for (int j = 0; j < width_; j++) {
      leds[i + j] = c;
    }
    FastLED.show();
    check_serial_incoming();
    for (int j = 0; j < width_; j++) {
      leds[i + j] = CRGB::Black;
    }
    delay(speed_);
  }
  delay(50);
}

void colorWipe(CHSV c, int speed_) {
  bool direction_ = 1;
  for (int i = 0; i < NUM_LEDS; i++) {
    if (direction == FORWARD) {
      leds[i] = c;
    }
    else {
      leds[NUM_LEDS - 1 - i] = c;
    }
    FastLED.show();
    check_serial_incoming();
    delay(speed_);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
CRGB Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
