/**
 * MIDI Oboe
 * Rohan Mitchell
 * Based heavily on the Kontinuum Lab Instrument Kit by Jeppe Tofthøj Rasmussen
 * https://github.com/KontinuumLab/KontinuumLAB_Instrument_Kit
 *
 * Licence: GPL v3
 */

const int PIN_SENSE = 0;
const int PIN_MUX_0 = 1;
const int PIN_MUX_1 = 2;
const int PIN_MUX_2 = 3;
const int PIN_MUX_3 = 4;

const int PIN_LIP = 15;
const int PIN_BREATH = 16;
const int PIN_TONGUE = 17;
const int PIN_TONGUE_LED = 14;

const int BREATH_MIN = 180;
const int BREATH_MAX = 838;

const int MIDI_MIN = 0;
const int MIDI_MAX = 127;

const int MIDI_CHANNEL = 1;

const int KEY_THRESHOLD = 5000;


void setup() {
  pinMode(PIN_MUX_0, OUTPUT);
  pinMode(PIN_MUX_1, OUTPUT);
  pinMode(PIN_MUX_2, OUTPUT);
  pinMode(PIN_MUX_3, OUTPUT);

  pinMode(PIN_TONGUE_LED, OUTPUT);

  Serial.begin(9600);
}

int currentNote, lastNote;
int breath, lastBreath, lip, tongue;

bool keys[] = {false, false, false, false};

void loop() {
  lastBreath = breath;
  breath = analogRead(PIN_BREATH);
  breath = map(breath, BREATH_MIN, BREATH_MAX, MIDI_MIN, MIDI_MAX);
  breath = constrain(breath, MIDI_MIN, MIDI_MAX);

  lip = touchRead(PIN_LIP); // 1250 - 1600
  tongue = analogRead(PIN_TONGUE); // 200 - 450

  if (breath != lastBreath) {
    usbMIDI.sendAfterTouch(breath, MIDI_CHANNEL);

    // Start and stop the note
    if (breath == 0) {
      digitalWrite(PIN_TONGUE_LED, LOW);
      usbMIDI.sendNoteOff(currentNote, 0, MIDI_CHANNEL);

    } else if(breath != 0 && lastBreath == 0) {
      digitalWrite(PIN_TONGUE_LED, HIGH);
      usbMIDI.sendNoteOn(currentNote, 127, MIDI_CHANNEL);
    }
  }

  lastNote = currentNote;

  for (int i = 0; i < 4; i++) {
    keys[i] = readTouch(i) > KEY_THRESHOLD;
  }

  if (keys[1] && keys[2] && keys[3]) {
    currentNote = 55; // G
  } else if (keys[1] && keys[2] && !keys[3]) {
    currentNote = 57; // A
  } else if (keys[1] && !keys[2] && !keys[3]) {
    currentNote = 59; // B
  } else if (!keys[1] && !keys[2] && !keys[3]) {
    currentNote = 61;
  } else {
    currentNote = 63;
  }

  if (breath > 0 && lastNote != currentNote && currentNote > 0) {
    usbMIDI.sendNoteOff(lastNote, 0, MIDI_CHANNEL);
    usbMIDI.sendNoteOn(currentNote, 127, MIDI_CHANNEL);
  }

  delay(10);
}


int readTouch(int number) {
  digitalWrite(PIN_MUX_0, bitRead(number, 0));
  digitalWrite(PIN_MUX_1, bitRead(number, 1));
  digitalWrite(PIN_MUX_2, bitRead(number, 2));
  digitalWrite(PIN_MUX_3, bitRead(number, 3));
  uint16_t value = touchRead(PIN_SENSE);

  return value;
}
