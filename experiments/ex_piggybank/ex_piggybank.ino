//Libraries for the Yúns connection
#include <Bridge.h>// communication between the ATmega32U4(the arduino) and the AR9331 (Linux side)
#include <HttpClient.h> //Creates a HTTP client on Linux.

HttpClient client; // the HTTP client

// your group number
int group = 1;

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


unsigned long int callurlTimer = 0; //  url timer
unsigned long int callurlDelay = 5000; // url delay. 1000=1 sec


void setup() {
  // configure the 13 pin as a OUTPUT
  pinMode(13, OUTPUT);


  // Blink once with the led on pin 13 when the connection is ready
  digitalWrite(13, LOW);
  Bridge.begin(); //starts the Bridge
  digitalWrite(13, HIGH);



  // initialize the button pins as inputs:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);

  // configure the serial
  Serial.begin(9600);

  while (!Serial);// waits for the serial monitor to open

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
    coinCount = coinCount + 10;
    coinState3 = 0;
    delay(50); // small delay
  }

  if (callurlTimer + callurlDelay < millis()) {

    //get the current coin value form the API
    //Setup for the content of the URL/web address
    String url =  "http://verkstad.cc/iot/mmx/";
    url += group;
    url += "/get_value.php";
    // get current coin number
    client.get(url); // sends the url to the client

    //the coin value from the api
    int coinCountBank = 0;

    // get the  coin value and set it to the  coin value variable
    while (client.available()) {
      char c = client.read();
      coinCountBank = coinCountBank * 10 + c - 48;
    }

    Serial.flush();//Waits for the transmission of outgoing serial data to complete


    //Setup for the content of the URL/web address
    url =  "http://verkstad.cc/iot/mmx/";
    url += group;
    url += "/index.php?coinCount=";
    coinCountBank = coinCountBank + coinCount; // the coin value from the Api with the local value
    url += coinCountBank; // update the url with new coin value


    // prints the current local coin value and the total coin value in the serial montitor
    Serial.print("Coin value added:");
    Serial.println(coinCount);
    Serial.print("Total Coin value:");
    Serial.println(coinCountBank);

    coinCount = 0; // resets the local coin count

    // Sends the coin count total to the api
    client.get(url); // sends the url to the client


    Serial.flush();//Waits for the transmission of outgoing serial data to complete


    callurlTimer = millis(); // sets callurlTimer to be millis()/the time the arduino been on
  }



  // sets the buttonstates to the buttonswitches
  buttonSwitch1 = buttonState1;
  buttonSwitch2 = buttonState2;
  buttonSwitch3 = buttonState3;





}
