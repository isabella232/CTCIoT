


// solenoid pin
int solenoidPin = 10;


int buttonPin = 2;    // the number of the button pin

int buttonState = 0;   // variable for reading the button status



void setup() {

  // configure the solenoidPin as a OUTPUT
  pinMode(solenoidPin, OUTPUT);

  // configure the serial
  Serial.begin(9600);

  while (!Serial);// waits for the serial monitor to open
}

void loop() {

  // read the state of the button value:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) { //button is pushed
    Serial.println("vent closed");
    // solenoid on
    digitalWrite(solenoidPin, HIGH);
  } else {
    // solenoid  off
    Serial.println("vent open");
    digitalWrite(solenoidPin, LOW);
  }



  delay(250);  //small delay

}



