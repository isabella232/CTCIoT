
// button set 1
int buttonPin1 = 2;    // the number of the first button pin
int buttonState1 = 0;   // variable for reading the button status
int buttonSwitch1 = 0; // variable for Switching the button on and off
// button set 2
int buttonPin2 = 3;    // the number of the the second button pin
int buttonState2 = 0;   // variable for reading the button status
int buttonSwitch2 = 0; // variable for Switching the button on and off
// button set 3
int buttonPin3 = 4;    // the number of the third button pin
int buttonState3 = 0;   // variable for reading the button status
int buttonSwitch3 = 0; // variable for Switching the button on and off

int coinCount = 0; // variable counting values of the coins




void setup() {


  // initialize the button pins as inputs:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);

  // configure the serial
  Serial.begin(9600);

}

void loop() {


  //  ---------------------------------
  //  the 1 value coin
  //  ---------------------------------
  // read the state of the button value:
  buttonState1 = digitalRead(buttonPin1);
  int coinState1 = 0; //variable for checking if a coin has fallen down or not
  if (buttonState1 == LOW && buttonSwitch1 == HIGH ) { //button is pushed
    // only one new coin added per button push
    coinState1 = 1;
  } else {
    coinState1 = 0;
  }
  // a coin has triggered the button once. add that coins value to the coinCount
  if (coinState1 == 1) {
    Serial.println("1 coin");
    coinCount = coinCount + 1;
    coinState1 = 0;
    delay(50); // small delay
  }


  //  ---------------------------------
  //  the 5 value coin
  //  ---------------------------------
  // read the state of the button value:
  buttonState2 = digitalRead(buttonPin2);
  int coinState2 = 0; //variable for checking if a coin has fallen down or not
  if (buttonState2 == LOW && buttonSwitch2 == HIGH ) { //button is pushed
    // only one new coin added per button push
    coinState2 = 1;
  } else {
    coinState2 = 0;
  }
  // a coin has triggered the button once. add that coins value to the coinCount
  if (coinState2 == 1) {
    Serial.println("5 coin");
    coinCount = coinCount + 5;
    coinState2 = 0;
    delay(50); // small delay
  }


  //  ---------------------------------
  //  the 10 value coin
  //  ---------------------------------
  // read the state of the button value:
  buttonState3 = digitalRead(buttonPin3);
  int coinState3 = 0; //variable for checking if a coin has fallen down or not
  if (buttonState3 == LOW && buttonSwitch3 == HIGH ) { //button is pushed
    // only one new coin added per button push
    coinState3 = 1;
  } else {
    coinState3 = 0;
  }
  // a coin has triggered the button once. add that coins value to the coinCount
  if (coinState3 == 1) {
    Serial.println("10 coin");
    coinCount = coinCount + 10;
    coinState3 = 0;
    delay(50); // small delay
  }





  // sets the buttonstates to the buttonswitches
  buttonSwitch1 = buttonState1;
  buttonSwitch2 = buttonState2;
  buttonSwitch3 = buttonState3;





}
