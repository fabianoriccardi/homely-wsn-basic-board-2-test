/**
 * Read the battery voltage.
 * Waiting for an update... The analogRead() is actually broken.
 */

const int analogInPin = 6;

void setup() {
  Serial.begin(115200);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
}

void loop() {
  int sensorValueRaw = analogRead(analogInPin);
  int sensorValue = map(sensorValueRaw, 0, 8192, 0, 2600);

  Serial.print("sensor = ");
  Serial.print(sensorValueRaw);
  Serial.print(" ");
  Serial.println(sensorValue);

  delay(300);
}
