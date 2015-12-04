



//Libraries for the Yúns connection
#include <Bridge.h>// communication between the ATmega32U4(the arduino) and the AR9331 (Linux side)
#include <HttpClient.h> //Creates a HTTP client on Linux.

HttpClient client; // the HTTP client

//Libraries for the NeoPixel
#include <Adafruit_NeoPixel.h>
// Which pin on the Arduino is connected to the NeoPixel
#define PIN            6  // pin for the NeoPixel

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800); // Setup for the NeoPixel


// your group number
int group = 3;

// teacher short memory api for this exercise
String teacherUrl = "http://verkstad.cc/iot/mmx/8/get_value.php";


//  state for if the wind is to strong
int windState = 0;

int oldwindState = 0; // save the oldwindState
unsigned long int windTimer = 0; //wind timer
unsigned long int windtimeDelay = 2000; // wind time delay. 1000=1 sec

unsigned long int callurlTimer = 0; //  url timer
unsigned long int callurlDelay = 5000; // url delay. 1000=1 sec

//the wind threshold from the api
int windThreshold = 0;

void setup() {
  // configure the 13 pin as a OUTPUT
  pinMode(13, OUTPUT);

  // Blink once with the led on pin 13 when the connection is ready
  digitalWrite(13, LOW);
  Bridge.begin(); //starts the Bridge
  digitalWrite(13, HIGH);

  pixels.begin(); // This initializes the NeoPixel library.


  // configure the serial
  Serial.begin(9600);

  //get the windThreshold one time to start with
  //check the control units current values
  client.get(teacherUrl); // sends the url to the client
  windThreshold = 0;
  // get the  windThreshold value
  while (client.available()) {
    char c = client.read();
    windThreshold = windThreshold * 10 + c - 48;
  }
  while (!Serial); // waits for the serial monitor to open
}

void loop() {

  // Take in the value from the sensor
  int sensor = analogRead(0);// from analog sensor 0
  if (sensor <= windThreshold) {
    windTimer = millis(); // sets callurlTimer to be millis()/the time the arduino been on
  }
  if (windTimer + windtimeDelay < millis()) {
    //the wind is ok
    //greenlight
    //loop for updating the colours of the NeoPixel
    for (int i = 0; i < 1; i++) {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(50, 150, 50)); // Moderately bright greencolor.
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
    windState = 0; // the state of the wind is now calm
  } else {

    //redlight
    //loop for updating the colours of the NeoPixel
    for (int i = 0; i < 1; i++) {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(150, 50, 50)); // Moderately red color.
      pixels.show(); // This sends the updated pixel color to the hardware.
    }

    windState = 1;// the state of the wind is to windy
  }

  // to slow down the amount of calls to the url
  if (callurlTimer + callurlDelay < millis()) {

    //check the control units current values
    client.get(teacherUrl); // sends the url to the client
    windThreshold = 0;
    // get the  windThreshold value
    while (client.available()) {
      char c = client.read();
      windThreshold = windThreshold * 10 + c - 48;
    }



    Serial.println ("Wind limit reached");
    Serial.println (windThreshold);


    //Setup for the content of the URL/web address
    String url =  "http://verkstad.cc/iot/mmx/";
    url += group;
    url += "/index.php?windThreshold=";
    url += windState; //current state of the wind
    // Sends the windThreshold state to the api
    client.get(url); // sends the url to the client

    callurlTimer = millis(); // sets callurlTimer to be millis()/the time the arduino been on
  }
  Serial.flush();//Waits for the transmission of outgoing serial data to complete


  // prints the current windState in the serial montitor
  Serial.print("windState: ");
  Serial.println(windState);
  Serial.print("wind sensor: ");
  Serial.println(sensor);


  delay(500); //small delay

}


