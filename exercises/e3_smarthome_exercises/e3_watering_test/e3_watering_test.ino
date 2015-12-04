

// solenoid pin
int solenoidPin = 9;

void setup() {

  // configure the motorPin as a OUTPUT
  pinMode(solenoidPin, OUTPUT);

  // configure the serial
  Serial.begin(9600);

  while (!Serial);// waits for the serial monitor to open
}

void loop() {

  // read the state of the button value:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) { //button is pushed
    // solenoid on
    Serial.println("water on");
    digitalWrite(solenoidPin, HIGH);
  } else {
    // solenoid  off
    Serial.println("water off");
    digitalWrite(solenoidPin, LOW);
  }

  delay(250);  //small delay
}



