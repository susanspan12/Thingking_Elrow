//****************************************************************************************************************************
//  Thingking Studio (Pty) Ltd
//  Product: Elrow x Absolut Cathedral
//****************************************************************************************************************************
// This code runs on the Teensy 3.6 mounted in the housing in the plinth.
// This controller must be connected to the USB cable provided during operation to receive comms from the touchscreen device.
// Please take note that this controller is powered through the USB connected to the touch screen device.

#include "FastLED.h"

//----#DEFINES--------------------------------------------------------
//****PIN DEFINES*****************************************************

#define R_1_PWM_PIN 35
#define G_1_PWM_PIN 36
#define B_1_PWM_PIN 37
#define R_2_PWM_PIN 38
#define G_2_PWM_PIN 14
#define B_2_PWM_PIN 16
#define R_3_PWM_PIN 20
#define G_3_PWM_PIN 21
#define B_3_PWM_PIN 22
#define R_4_PWM_PIN 23
#define G_4_PWM_PIN 2
#define B_4_PWM_PIN 3
#define R_5_PWM_PIN 4
#define G_5_PWM_PIN 5
#define B_5_PWM_PIN 6
#define R_6_PWM_PIN 7
#define G_6_PWM_PIN 8
#define B_6_PWM_PIN 9
#define DEBUG_BUTTON_PIN 34
#define RELAY_PIN 39

#define ANIMATION_TIME 10000            //TODO change to appropriate time
#define DEFAULT_ANIMATION_TIME 60000    //TODO change to appropriate time
#define RELAY_DELAY_TIME 3000           //TODO change to appropriate time
#define RELAY_START_TIME 2500           //TODO change to appropriate time

#define DEBUG_PRINTLN false

//****SERIAL COMMS*****************************************************

//Run animation 1 to 5
byte ANIMATION_BYTE_1 = '1';
byte ANIMATION_BYTE_2 = '2';
byte ANIMATION_BYTE_3 = '3';
byte ANIMATION_BYTE_4 = '4';
byte ANIMATION_BYTE_5 = '5';
byte YES_BYTE = 'Y';
byte NO_BYTE = 'N';

//----Variable definitions--------------------------------------------
//****Colour HSV******************************************************
byte hue = 0;
byte saturation = 255;
byte value = 100;
byte animation_byte = 'x';

//Time and counters
int delay_anim = 2;
int theme_hue_counter = 0;
int rainbow_counter = 0;
float animation_start_time = -1;
float default_animation_start_time = -1;
float relay_delay_time = -1;
float relay_start_time = -1;

//Flags
bool relay_start_flag = false;

//Absolut theme hues
int THEME_HUE_BLUE = 160;
int THEME_HUE_PURPLE = 190;
int THEME_HUE_PINK = 224;
int THEME_HUE_AQUA = 128;
int THEME_HUE_YELLOW = 35;

int theme_hues[] = {THEME_HUE_BLUE, THEME_HUE_PURPLE, THEME_HUE_PINK, THEME_HUE_AQUA, THEME_HUE_YELLOW};

int rgb_pin_matrix[6][3] = {     //6 frames each with 3 pins (r, g and b)
  {R_1_PWM_PIN, G_1_PWM_PIN, B_1_PWM_PIN},
  {R_2_PWM_PIN, G_2_PWM_PIN, B_2_PWM_PIN},
  {R_3_PWM_PIN, G_3_PWM_PIN, B_3_PWM_PIN},
  {R_4_PWM_PIN, G_4_PWM_PIN, B_4_PWM_PIN},
  {R_5_PWM_PIN, G_5_PWM_PIN, B_5_PWM_PIN},
  {R_6_PWM_PIN, G_6_PWM_PIN, B_6_PWM_PIN}
};

//**********Functions******************************************************************************
void debug_sequence();
void event_check();
void default_1();
void default_2();
void default_3();
void default_4();
void default_5();
void default_6();

void animation_1();
void animation_2();
void animation_3();
void animation_4();
void animation_5();
void clear_frames();

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 3; j++) {
      pinMode(rgb_pin_matrix[i][j], OUTPUT);
      delay(10);
    }
  }
  pinMode(DEBUG_BUTTON_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  delay(1000);
  //  debug_sequence();   //on startup, the debug sequence runs to see whether the order of the cables is correct and that each colour works as intended.
}

void loop() {
  clear_frames();
  default_1();
  clear_frames();
  default_2();
  clear_frames();
  default_3();
  clear_frames();
  default_4();
  clear_frames();
  default_5();
  clear_frames();
  default_6();
}

//-------Event check-----------------------------------------------------------------------------------
void event_check() {
  check_serial_incoming();
  if (digitalRead(DEBUG_BUTTON_PIN) == HIGH) {
    if (DEBUG_PRINTLN) {
      Serial.println("event check");
    }
    debug_sequence();
  }
}

//-------Serial comms---------------------------------------------------------------------------------
void check_serial_incoming() {
  if (Serial.available()) {
    animation_byte = Serial.read();
    if (animation_byte == ANIMATION_BYTE_1) {
      Serial.write('Y');
      animation_1();
    }
    else if (animation_byte == ANIMATION_BYTE_2) {
      Serial.write('Y');
      animation_2();
    }
    else if (animation_byte == ANIMATION_BYTE_3) {
      Serial.write('Y');
      animation_3();
    }
    else if (animation_byte == ANIMATION_BYTE_4) {
      Serial.write('Y');
      animation_4();
    }
    else if (animation_byte == ANIMATION_BYTE_5) {
      Serial.write('Y');
      animation_5();
    }
    else {
      Serial.write('N');
    }
  }
  while (Serial.available()) {  //clear buffer
    Serial.read();
  }
}

//-----------------Debugging------------------------------------------------------------------
void debug_sequence() {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 3; j++) {
      analogWrite(rgb_pin_matrix[i][j], 0);
      delay(1);
    }
  }
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 3; j++) {
      analogWrite(rgb_pin_matrix[i][j], 255);
      delay(1000);
      analogWrite(rgb_pin_matrix[i][j], 0);
    }
  }
}

//-----------------Show lights----------------------------------------------------------------
void showAnalogRGB( const CRGB& rgb, int frame) {
  analogWrite(rgb_pin_matrix[frame][0], rgb.r );
  analogWrite(rgb_pin_matrix[frame][1], rgb.g );
  analogWrite(rgb_pin_matrix[frame][2], rgb.b );
}

//--------------------------Animations-----------------------------------------------------------------------------------------------
//---------------------Default animations-------------------------------------------------------------------------------------------
void clear_frames() {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 3; j++) {
      analogWrite(rgb_pin_matrix[i][j], 0);
    }
  }
}

//--------------default 1-----------------------------------------------------------------------------------------------------------
void default_1() {      //scrolling rainbow
  //  int rainbow_colours[] = {0, 15, 50, 96, 160, 192, 224};
  int rainbow_colours[] = {10, 50, 94, 135, 180, 220};
  int colour_counter = 0;
  animation_start_time = millis();
  while (millis() - animation_start_time <= ANIMATION_TIME) {
    for (int f = 0; f < 6; f++) {
      showAnalogRGB(CHSV( rainbow_colours[colour_counter], 255, 255), f);
      event_check();
      delay(delay_anim);
      colour_counter++;
      if (colour_counter == 6) {
        colour_counter = 0;
      }
    }
    colour_counter++;
    if (colour_counter == 6) {
      colour_counter = 0;
    }
    event_check();
    delay(delay_anim * 50);
  }
}

//--------------default 2-----------------------------------------------------------------------------------------------------------
void default_2() {    //Green_blue swopping
  int animation_1_colours_midpoint = 123;
  default_animation_start_time = millis();
  while (millis() - default_animation_start_time <= DEFAULT_ANIMATION_TIME) {
    for (int i = 90; i <= 156; i++) {
      for (int f = 0; f < 6; f++) {
        if (f == 0 || f == 2 || f == 4) {
          hue = i;
          showAnalogRGB(CHSV(hue, 255, 255), f);
          event_check();
        }
        else {
          hue = animation_1_colours_midpoint - (i - animation_1_colours_midpoint);
          showAnalogRGB(CHSV(hue, 255, 255), f);
          event_check();
        }
      }
      delay(delay_anim);
    }
    delay(delay_anim);
    for (int r = 156; r >= 90; r--) {
      for (int f = 0; f < 6; f++) {
        if (f == 0 || f == 2 || f == 4) {
          hue = r;
          showAnalogRGB(CHSV(hue, 255, 255), f);
          event_check();
        }
        else {
          hue = animation_1_colours_midpoint + (animation_1_colours_midpoint - r);
          showAnalogRGB(CHSV(hue, 255, 255), f);
          event_check();
        }
        delay(delay_anim);
      }
      delay(delay_anim);
    }
  }
}

//--------------default 3-----------------------------------------------------------------------------------------------------------
void default_3() {      //scrolling yellow & purple stripes
  int rainbow_colours[] = {THEME_HUE_PURPLE, THEME_HUE_PURPLE, THEME_HUE_PURPLE, THEME_HUE_YELLOW, THEME_HUE_YELLOW, THEME_HUE_YELLOW};
  int colour_counter = 0;
  animation_start_time = millis();
  while (millis() - animation_start_time <= ANIMATION_TIME) {
    for (int f = 0; f < 6; f++) {
      showAnalogRGB(CHSV( rainbow_colours[colour_counter], 255, 255), f);
      event_check();
      delay(delay_anim);
      colour_counter++;
      if (colour_counter == 6) {
        colour_counter = 0;
      }
    }
    colour_counter++;
    if (colour_counter == 6) {
      colour_counter = 0;
    }
    event_check();
    delay(delay_anim * 50);
  }
}

//--------------default 4-----------------------------------------------------------------------------------------------------------
void default_4() {   //strobe lights = expressive
  //  hue = theme_hues[1];
  int random_hue = random(0, 4);
  int random_frame_1 = random(0, 6);
  int random_frame_2 = random(0, 6);
  int frame_pulse_timer = 0;
  default_animation_start_time = millis();
  while (millis() - default_animation_start_time <= DEFAULT_ANIMATION_TIME) {
    random_hue = random(0, 4);
    random_frame_1 = random(0, 6);
    random_frame_2 = random(0, 6);
    event_check();
    frame_pulse_timer = millis();
    while (millis() - frame_pulse_timer < 200) {
      showAnalogRGB(CHSV(theme_hues[random_hue], 255, 255), random_frame_1);
      showAnalogRGB(CHSV(theme_hues[random_hue], 255, 255), random_frame_2);
      event_check();
    }
    showAnalogRGB(CHSV(0, 0, 0), random_frame_1);
    showAnalogRGB(CHSV(0, 0, 0), random_frame_2);
    event_check();
  }
}

//------------------------------------------------------------------------------------------------------------------------------------
void default_5() {
  default_animation_start_time = millis();
  while (millis() - default_animation_start_time <= DEFAULT_ANIMATION_TIME) {
    for (int h = 0; h < 5; h++) {
      hue = theme_hues[h];
      for (int i = 30; i <= 255; i++) {
        for (int f = 0; f < 6; f++) {
          showAnalogRGB(CHSV( hue, 255, i), f);
          event_check();
        }
        delay(delay_anim * 2);
      }
      delay(delay_anim * 100);
      for (int r = 255; r >= 30; r--) {
        for (int f = 0; f < 6; f++) {
          showAnalogRGB(CHSV( hue, 255, r), f);
          event_check();
        }
        delay(delay_anim * 2);
      }
      delay(delay_anim * 10);
    }
  }
}

//--------------default 6-----------------------------------------------------------------------------------------------------------
void default_6() {    //purple,blue scrolling
  int animation_5_values[] = {255, 205, 155, 105, 55, 0};
  int value_counter = 0;
  animation_start_time = millis();
  while (millis() - animation_start_time <= ANIMATION_TIME) {
    for (int h = 0; h < 2; h++) {
      for (int f = 0; f < 6; f++) {
        showAnalogRGB(CHSV(theme_hues[h], 255, animation_5_values[value_counter]), f);
        event_check();
        delay(delay_anim * 2);
        value_counter++;
        if (value_counter == 6) {
          value_counter = 0;
        }
      }
      value_counter++;
      if (value_counter == 6) {
        value_counter = 0;
      }
      delay(delay_anim * 100);
    }
  }
}

//---------------------Triggered animations-------------------------------------------------------
//--------------------------Animation 1-----------------------------------------------------------
void animation_1() {    //pulsing blues and greens - human
  int animation_1_colours[] = {96, 108, 120, 130, 140, 150, 160};
  //  int animation_1_colours[] = {96, 100, 105, 110, 115, 120, 125};
  int random_colour = random(0, 6);
  int random_frame = random(0, 6);
  int frame_pulse_timer = 0;
  relay_start_flag = false;
  animation_start_time = millis();
  while (millis() - animation_start_time <= ANIMATION_TIME) {
    if ((millis() - animation_start_time >= RELAY_DELAY_TIME) && (millis() - animation_start_time < RELAY_DELAY_TIME + 10)) {
      relay_start_time = millis();
      relay_start_flag = true;
    }
    if ((millis() - relay_start_time <= RELAY_START_TIME) && (relay_start_flag == true)) {
      digitalWrite(RELAY_PIN, HIGH);
    }
    else {
      digitalWrite(RELAY_PIN, LOW);
      relay_start_flag = false;
    }
    random_colour = random(0, 6);
    random_frame = random(0, 6);
    frame_pulse_timer = millis();
    while (millis() - frame_pulse_timer < 200) {
      showAnalogRGB(CHSV(animation_1_colours[random_colour], 255, 255), random_frame);
    }
  }
}

//--------------------------Animation 2-----------------------------------------------------------
void animation_2() {   //strobe lights = expressive
  hue = theme_hues[1];
  int random_frame_1 = random(0, 6);
  int random_frame_2 = random(0, 6);
  int frame_pulse_timer = 0;
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 3; j++) {
      analogWrite(rgb_pin_matrix[i][j], 0);
    }
  }
  animation_start_time = millis();
  while (millis() - animation_start_time <= ANIMATION_TIME) {
    if ((millis() - animation_start_time >= RELAY_DELAY_TIME) && (millis() - animation_start_time < RELAY_DELAY_TIME + 10)) {
      relay_start_time = millis();
      relay_start_flag = true;
    }
    if ((millis() - relay_start_time <= RELAY_START_TIME) && (relay_start_flag == true)) {
      digitalWrite(RELAY_PIN, HIGH);
    }
    else {
      digitalWrite(RELAY_PIN, LOW);
      relay_start_flag = false;
    }
    random_frame_1 = random(0, 6);
    random_frame_2 = random(0, 6);
    frame_pulse_timer = millis();
    while (millis() - frame_pulse_timer < 200) {
      showAnalogRGB(CHSV(hue, 255, 255), random_frame_1);
      showAnalogRGB(CHSV(hue, 255, 255), random_frame_2);
    }
    showAnalogRGB(CHSV(hue, 255, 0), random_frame_1);
    showAnalogRGB(CHSV(hue, 255, 0), random_frame_2);
  }
}

//--------------------------Animation 3-----------------------------------------------------------
void animation_3() {    //Scrolling rainbow = equal
  int animation_3_colours[] = {0, 15, 50, 96, 160, 192};
  int colour_counter = 0;
  animation_start_time = millis();
  while (millis() - animation_start_time <= ANIMATION_TIME) {
    if ((millis() - animation_start_time >= RELAY_DELAY_TIME) && (millis() - animation_start_time < RELAY_DELAY_TIME + 55)) {
      relay_start_time = millis();
      relay_start_flag = true;
    }
    if ((millis() - relay_start_time <= RELAY_START_TIME) && (relay_start_flag == true)) {
      digitalWrite(RELAY_PIN, HIGH);
    }
    else {
      digitalWrite(RELAY_PIN, LOW);
      relay_start_flag = false;
    }
    for (int f = 0; f < 6; f++) {
      showAnalogRGB(CHSV( animation_3_colours[colour_counter], 255, 255), f);
      delay(delay_anim);
      colour_counter++;
      if (colour_counter == 6) {
        colour_counter = 0;
      }
    }
    colour_counter++;
    if (colour_counter == 6) {
      colour_counter = 0;
    }
    delay(delay_anim * 50);
  }
}

//--------------------------Animation 4-----------------------------------------------------------
void animation_4() {  //pulsing aqua - love
  hue = theme_hues[3];
  animation_start_time = millis();
  while (millis() - animation_start_time <= ANIMATION_TIME) {

    if ((millis() - relay_start_time <= RELAY_START_TIME) && (relay_start_flag == true)) {
      digitalWrite(RELAY_PIN, HIGH);
    }
    else {
      digitalWrite(RELAY_PIN, LOW);
      relay_start_flag = false;
    }
    for (int i = 30; i <= 255; i++) {
      for (int f = 0; f < 6; f++) {
        showAnalogRGB(CHSV( hue, 255, i), f);
      }
      delay(delay_anim * 2);
      if ((millis() - animation_start_time >= RELAY_DELAY_TIME) && (millis() - animation_start_time < RELAY_DELAY_TIME + 1000)) {
        relay_start_time = millis();
        relay_start_flag = true;
      }
      if ((millis() - relay_start_time <= RELAY_START_TIME) && (relay_start_flag == true)) {
        digitalWrite(RELAY_PIN, HIGH);
      }
      else {
        digitalWrite(RELAY_PIN, LOW);
        relay_start_flag = false;
      }
    }
    delay(delay_anim * 10);
    for (int r = 255; r >= 30; r--) {
      for (int f = 0; f < 6; f++) {
        showAnalogRGB(CHSV( hue, 255, r), f);
      }
      delay(delay_anim * 2);
      if ((millis() - animation_start_time >= RELAY_DELAY_TIME) && (millis() - animation_start_time < RELAY_DELAY_TIME + 1000)) {
        relay_start_time = millis();
        relay_start_flag = true;
      }
      if ((millis() - relay_start_time <= RELAY_START_TIME) && (relay_start_flag == true)) {
        digitalWrite(RELAY_PIN, HIGH);
      }
      else {
        digitalWrite(RELAY_PIN, LOW);
        relay_start_flag = false;
      }
    }
    delay(delay_anim * 10);
  }
}


//--------------------------Animation 5-----------------------------------------------------------
void animation_5() {    //Scrolling yellow = transparency
  hue = theme_hues[4];
  int animation_5_values[] = {255, 205, 155, 105, 55, 0};
  int value_counter = 0;
  animation_start_time = millis();
  while (millis() - animation_start_time <= ANIMATION_TIME) {
    if ((millis() - animation_start_time >= RELAY_DELAY_TIME) && (millis() - animation_start_time < RELAY_DELAY_TIME + 200)) {
      relay_start_time = millis();
      relay_start_flag = true;
    }
    if ((millis() - relay_start_time <= RELAY_START_TIME) && (relay_start_flag == true)) {
      digitalWrite(RELAY_PIN, HIGH);
    }
    else {
      digitalWrite(RELAY_PIN, LOW);
      relay_start_flag = false;
    }
    for (int f = 0; f < 6; f++) {
      showAnalogRGB(CHSV(35, 255, animation_5_values[value_counter]), f);
      delay(delay_anim * 2);
      value_counter++;
      if (value_counter == 6) {
        value_counter = 0;
      }
    }
    value_counter++;
    if (value_counter == 6) {
      value_counter = 0;
    }
    delay(delay_anim * 100);
  }
}
