const int PIN_SENSE = 0;
const int PIN_MUX_0 = 1;
const int PIN_MUX_1 = 2;
const int PIN_MUX_2 = 3;
const int PIN_MUX_3 = 4;

const int MIDI_CHANNEL = 1;
const int KEY_THRESHOLD = 5000;

void setup() {
  pinMode(PIN_MUX_0, OUTPUT);
  pinMode(PIN_MUX_1, OUTPUT);
  pinMode(PIN_MUX_2, OUTPUT);
  pinMode(PIN_MUX_3, OUTPUT);

  Serial.begin(9600);
}

int currentNote, lastNote;
bool keys[] = {false, false, false, false};

void loop() {
  //Serial.println("---");
  //Serial.println(readTouch(0));
  //Serial.println(readTouch(1));
  //Serial.println(readTouch(2));
  //Serial.println(readTouch(3));

  lastNote = currentNote;

  for(int i=0; i<4; i++) {
    keys[i] = readTouch(i) > KEY_THRESHOLD;
  }

  if (keys[1] && keys[2] && keys[3]) {
    currentNote = 55; // G
  } else if (keys[1] && keys[2] && !keys[3]) {
    currentNote = 57; // A
  } else if (keys[1] && !keys[2] && !keys[3]) {
    currentNote = 59; // B
  } else if (!keys[1] && !keys[2] && !keys[3]) {
    currentNote = 0; // Note off
  }

  if (lastNote != currentNote) {
    usbMIDI.sendNoteOff(lastNote, 0, MIDI_CHANNEL);

    if (currentNote > 0) {
      usbMIDI.sendNoteOn(currentNote, 127, MIDI_CHANNEL);
    }
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
