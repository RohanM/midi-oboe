// Simple LED blink

const int pinLed = LED_BUILTIN;

const int pinSense = 0;
const int pinMux0 = 1;
const int pinMux1 = 2;
const int pinMux2 = 3;
const int pinMux3 = 4;

void setup() {
  pinMode(pinLed, OUTPUT);

  pinMode(pinMux0, OUTPUT);
  pinMode(pinMux1, OUTPUT);
  pinMode(pinMux2, OUTPUT);
  pinMode(pinMux3, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  Serial.println("---");

  Serial.println(readTouch(0));
  Serial.println(readTouch(1));
  Serial.println(readTouch(2));
  Serial.println(readTouch(3));

  delay(1000);
}



int readTouch(int number) {
  digitalWrite(pinMux0, bitRead(number, 0));
  digitalWrite(pinMux1, bitRead(number, 1));
  digitalWrite(pinMux2, bitRead(number, 2));
  digitalWrite(pinMux3, bitRead(number, 3));
  uint16_t value = touchRead(pinSense);

  return value;
}
