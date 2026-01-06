#define LED 13

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();

    if (c == '1') {
      digitalWrite(LED, HIGH);
    }
    else if (c == '0') {
      digitalWrite(LED, LOW);
    }
  }
}
