/*
 * User button test.
 * 
 * It relies on internal pull resistor,
 * so if not pressed its value is high.
 */

const int buttonPin = 21;

void setup(){
  Serial.begin(115200);
  Serial.println("Button check");

  pinMode(buttonPin, INPUT_PULLUP);
}

void loop(){
  Serial.println(digitalRead(buttonPin));
  delay(40);
}
