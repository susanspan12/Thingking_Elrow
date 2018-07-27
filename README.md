"# Thingking_Elrow" 
--------------------

Python_code:
-------------------------------------------------------
Requirements: 
Python 2.7, 
Pyserial


Teensy_code:
-------------------------------------------------------

Requirements: 
FastLED library

Bottle_APA102_1: 
Code for the first Teensy 3.2 controlling the addressable LEDS in the bottle structure.
The board is marked "1" and must be connected to the tablet via USB which enables theme animations to be triggered
PLEASE NOTE: This controller needs to be powered externally with the 5V supply before it can be programmed. 

Bottle_APA102_2:
Code for the secong Teensy 3.2 controlling the addressable LEDS in the bottle structure.
The board is marked "2". The first Teensy relays the themed animation trigger to the second one. 
PLEASE NOTE: This controller needs to be powered externally with the 5V supply before it can be programmed. 

RGB_Frames:
Code for the Teensy 3.6 controlling the RGB LEDs mounted in the main cathedral structure.
This controller must be connected to the tablet via USB for power and comms. 

