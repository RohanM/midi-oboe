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

const int NUM_KEYS = 16;

const int BREATH_MIN = 180;
const int BREATH_MAX = 838;

const int LIP_MIN = 1400;
const int LIP_MAX = 1600;

const int TONGUE_MIN = 150;
const int TONGUE_MAX = 300;

const int MIDI_MIN = 0;
const int MIDI_MAX = 127;

const int MOD_WHEEL_CC = 1;
const int EXPRESSION_CC = 11;

const int MIDI_CHANNEL = 1;

const int KEY_THRESHOLD = 5000;


int currentNote, lastNote;
int breath, lastBreath, lip, lastLip, tongue, lastTongue;

bool keys[] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

const int NUM_FINGERINGS = 32;
const int FINGERING_IGNORE = 2;
const int FINGERING_NOTE = 16;

// 0 = must not be touched, 1 = must be touched, 2 = don't care
// 16 entries for the full layout, then the midi note value
const int fingerings[NUM_FINGERINGS][17] = {
  {0,  1, 2, 1, 1,  0, 0, 2, 1,  1, 1, 0, 1,  1, 2, 2,  46}, // Bb
  {0,  1, 2, 1, 1,  0, 0, 1, 0,  1, 1, 0, 1,  1, 2, 2,  47}, // B
  {0,  1, 2, 1, 1,  2, 0, 0, 0,  1, 1, 0, 1,  1, 0, 2,  48}, // C
  {0,  1, 2, 1, 1,  2, 2, 2, 2,  1, 1, 0, 1,  2, 1, 2,  49}, // C#
  {0,  1, 2, 1, 1,  2, 0, 2, 2,  1, 1, 0, 1,  2, 2, 0,  50}, // D
  {0,  1, 2, 1, 1,  2, 2, 2, 2,  1, 1, 0, 1,  2, 2, 1,  51}, // Eb
  {0,  1, 2, 1, 1,  2, 1, 2, 2,  1, 1, 0, 1,  2, 2, 2,  51}, // Eb
  {0,  1, 2, 1, 1,  2, 2, 2, 2,  1, 1, 0, 0,  2, 2, 2,  52}, // E
  {0,  1, 2, 1, 1,  2, 2, 2, 2,  1, 1, 1, 0,  2, 2, 2,  53}, // F
  {0,  1, 2, 1, 1,  2, 2, 2, 2,  1, 0, 0, 1,  2, 2, 2,  53}, // F
  {0,  1, 2, 1, 1,  2, 2, 2, 2,  1, 0, 0, 0,  2, 2, 2,  54}, // F#

  {0,  1, 2, 1, 1,  0, 2, 2, 2,  0, 0, 0, 0,  2, 2, 2,  55}, // G
  {0,  1, 2, 1, 1,  1, 2, 2, 2,  0, 0, 0, 0,  2, 2, 2,  56}, // Ab
  {0,  1, 2, 1, 0,  2, 2, 2, 2,  0, 0, 0, 0,  2, 2, 2,  57}, // A
  {0,  1, 2, 1, 0,  2, 2, 2, 2,  1, 0, 0, 0,  2, 2, 2,  58}, // Bb
  {0,  1, 2, 0, 0,  2, 2, 2, 2,  0, 0, 0, 0,  2, 2, 2,  59}, // B
  {0,  1, 2, 0, 0,  2, 2, 2, 2,  1, 0, 0, 0,  2, 2, 2,  60}, // C

  {0,  0, 1, 1, 1,  2, 2, 2, 2,  1, 1, 0, 1,  2, 1, 2,  61}, // C#
  {0,  0, 1, 1, 1,  2, 0, 2, 2,  1, 1, 0, 1,  2, 2, 0,  62}, // D
  {0,  0, 1, 1, 1,  2, 0, 2, 2,  1, 1, 0, 1,  2, 2, 1,  63}, // Eb
  {0,  0, 1, 1, 1,  2, 1, 2, 2,  1, 1, 0, 1,  2, 2, 2,  63}, // Eb

  {1,  1, 2, 1, 1,  2, 2, 2, 2,  1, 1, 0, 0,  2, 2, 2,  64}, // E
  {1,  1, 2, 1, 1,  2, 2, 2, 2,  1, 1, 1, 0,  2, 2, 2,  65}, // F
  {1,  1, 2, 1, 1,  2, 2, 2, 2,  1, 0, 0, 1,  2, 2, 2,  65}, // F
  {1,  1, 2, 1, 1,  2, 2, 2, 2,  1, 0, 0, 0,  2, 2, 2,  66}, // F#

  {1,  1, 2, 1, 1,  0, 2, 2, 2,  0, 0, 0, 0,  2, 2, 2,  67}, // G
  {1,  1, 2, 1, 1,  1, 2, 2, 2,  0, 0, 0, 0,  2, 2, 2,  68}, // Ab
  {1,  1, 2, 1, 0,  2, 2, 2, 2,  0, 0, 0, 0,  2, 2, 2,  69}, // A
  {1,  1, 2, 1, 0,  2, 2, 2, 2,  1, 0, 0, 0,  2, 2, 2,  70}, // Bb
  {1,  1, 2, 0, 0,  2, 2, 2, 2,  0, 0, 0, 0,  2, 2, 2,  71}, // B
  {1,  1, 2, 0, 0,  2, 2, 2, 2,  1, 0, 0, 0,  2, 2, 2,  72}, // C

  {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 36} // Fallback C
};

void setup() {
  pinMode(PIN_MUX_0, OUTPUT);
  pinMode(PIN_MUX_1, OUTPUT);
  pinMode(PIN_MUX_2, OUTPUT);
  pinMode(PIN_MUX_3, OUTPUT);

  pinMode(PIN_TONGUE_LED, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  lastBreath = breath;
  breath = analogRead(PIN_BREATH);
  breath = map(breath, BREATH_MIN, BREATH_MAX, MIDI_MIN, MIDI_MAX);
  breath = constrain(breath, MIDI_MIN, MIDI_MAX);

  lastLip = lip;
  lip = touchRead(PIN_LIP);
  lip = map(lip, LIP_MIN, LIP_MAX, MIDI_MIN, MIDI_MAX);
  lip = constrain(lip, MIDI_MIN, MIDI_MAX);

  lastTongue = tongue;
  tongue = analogRead(PIN_TONGUE);
  tongue = map(tongue, TONGUE_MIN, TONGUE_MAX, MIDI_MIN, MIDI_MAX);
  tongue = constrain(tongue, MIDI_MIN, MIDI_MAX);
  tongue = 127 - tongue;

  if (breath != lastBreath) {
    usbMIDI.sendAfterTouch(breath, MIDI_CHANNEL);

    // Start and stop the note
    if (breath == 0) {
      digitalWrite(PIN_TONGUE_LED, LOW);
      usbMIDI.sendControlChange(MOD_WHEEL_CC, 0, MIDI_CHANNEL);
      usbMIDI.sendControlChange(EXPRESSION_CC, 0, MIDI_CHANNEL);
      usbMIDI.sendNoteOff(currentNote, 0, MIDI_CHANNEL);

    } else if(breath != 0 && lastBreath == 0) {
      digitalWrite(PIN_TONGUE_LED, HIGH);
      usbMIDI.sendNoteOn(currentNote, 127, MIDI_CHANNEL);
    }
  }

  if (breath > 0) {
    if (lip != lastLip) {
      usbMIDI.sendControlChange(MOD_WHEEL_CC, lip, MIDI_CHANNEL);
    }
    if (tongue != lastTongue) {
      usbMIDI.sendControlChange(EXPRESSION_CC, tongue, MIDI_CHANNEL);
    }
  }


  lastNote = currentNote;

  for (int i = 0; i < NUM_KEYS; i++) {
    keys[i] = readTouch(i) > KEY_THRESHOLD;
  }

  for (int i = 0; i < NUM_FINGERINGS; i++) {
    bool found = true;
    for (int j = 0; j < NUM_KEYS; j++) {
      if (keys[j] != fingerings[i][j] && fingerings[i][j] != FINGERING_IGNORE) {
        found = false;
        break;
      }
    }
    if (found) {
      currentNote = fingerings[i][FINGERING_NOTE];
      break;
    }
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
