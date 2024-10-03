#include <USB-MIDI.h>

#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();

uint16_t lasttouched = 0;
uint16_t currtouched = 0;

USBMIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }

  Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1)
      ;
  }
  Serial.println("MPR121 found!");

  MIDI.begin(4);
}

void loop() {
  currtouched = cap.touched();    //check the pins states
  for (uint8_t i = 0; i < 12; i++) {    // for each twelve pins
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {        // if the pin was untouched and is now being touched

      Serial.print(i);
      Serial.println(" touched");
      MIDI.sendNoteOn(i, 127, 1);   // sends which pin is touched as a midi note
    }
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i))) {        // if it was touched and it is now not touched

      Serial.print(i);
      Serial.println(" released");
      MIDI.sendNoteOff(i, 0, 1);    // stops the note
    }
  }

  lasttouched = currtouched;    //each pin state is now the previous state
}
