const int pinSense = 0;
const int pinMux0 = 1;
const int pinMux1 = 2;
const int pinMux2 = 3;
const int pinMux3 = 4;

const int midiChannel = 1;
const int keyThreshold = 5000;

void setup() {
  pinMode(pinMux0, OUTPUT);
  pinMode(pinMux1, OUTPUT);
  pinMode(pinMux2, OUTPUT);
  pinMode(pinMux3, OUTPUT);

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
    keys[i] = readTouch(i) > keyThreshold;
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
    usbMIDI.sendNoteOff(lastNote, 0, midiChannel);

    if (currentNote > 0) {
      usbMIDI.sendNoteOn(currentNote, 127, midiChannel);
    }
  }

  delay(10);
}


int readTouch(int number) {
  digitalWrite(pinMux0, bitRead(number, 0));
  digitalWrite(pinMux1, bitRead(number, 1));
  digitalWrite(pinMux2, bitRead(number, 2));
  digitalWrite(pinMux3, bitRead(number, 3));
  uint16_t value = touchRead(pinSense);

  return value;
}
