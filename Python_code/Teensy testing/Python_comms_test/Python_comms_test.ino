int led = 13;
bool flag_serial = false;

const byte ANIMATION_BYTE_1 = '1';
const byte ANIMATION_BYTE_2 = '2';
const byte ANIMATION_BYTE_3 = '3';
const byte ANIMATION_BYTE_4 = '4';
const byte ANIMATION_BYTE_5 = '5';
const byte YES_BYTE = 'Y';
const byte NO_BYTE = 'N';

void check_serial_incoming();

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  check_serial_incoming();
  if (flag_serial) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000);               // wait for a second
      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
      delay(1000);
    }
  }
  delay(200);
}

void check_serial_incoming() {
  if (Serial.available()) {

    byte animation_byte = Serial.read();
    //    animation_byte = Serial.read();
    if (animation_byte == ANIMATION_BYTE_1) {
      flag_serial = true;
      Serial.write(YES_BYTE);
    }
    else if (animation_byte == ANIMATION_BYTE_2) {
      flag_serial = true;
      Serial.write(YES_BYTE);
    }
    else if (animation_byte == ANIMATION_BYTE_3) {
      flag_serial = true;
      Serial.write(YES_BYTE);
    }
    else if (animation_byte == ANIMATION_BYTE_4) {
      flag_serial = true;
      Serial.write(YES_BYTE);

    }
    else if (animation_byte == ANIMATION_BYTE_5) {
      flag_serial = true;
      Serial.write(YES_BYTE);
    }
    else {
      flag_serial = false;
      Serial.write(NO_BYTE);
    }
    while (Serial.available()) {  //clear buffer
      Serial.read();
    }
  }
  else {
    flag_serial = false;
  }
}
