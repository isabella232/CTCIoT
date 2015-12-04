

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
int group = 1;

// teacher short memory api for this exercise
String teacherUrl = "http://verkstad.cc/iot/mmx/6/get_value.php";

// threshold for the LDR  sensor
int threshold = 300;

// 0= car parking spot not taken, 1= car parking spot not taken taken
int carparkTaken = 0;
//  Time for the car to on a carparking spot
unsigned long int parkTimer = 0; //  parktimer
//general timer
unsigned long int timer = 1000; //  1000=1 sec

unsigned long int callurlTimer = 0; //  url timer
unsigned long int callurlDelay = 5000; // url delay. 1000=1 sec




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

  while (!Serial); // waits for the serial monitor to open
}

void loop() {

  // Take in the value from the sensor
  int sensor = analogRead(0); // from analog sensor 0

  //check if the light levels are lower then the threshold
  if (sensor < threshold && carparkTaken == 0) {

    //the light levels are lower ,thus the car parking spot is taken
    carparkTaken = 1;
    parkTimer = millis(); // sets  parkTimer to be millis()/the time the arduino been on

  } else {
    // not taken
    parkTimer = 0; //reset the time
    carparkTaken = 1;
  }



  // Update the carpark state to the api
  // to slow down the amount of calls to the url
  if (callurlTimer + callurlDelay < millis()) {


    //check the control units current values
    client.get(teacherUrl); // sends the url to the client

    //the time for the car to stay parked from the api
    int time = 0;

    // get the  statusLight value
    while (client.available()) {
      char c = client.read();
      time = time * 10 + c - 48;
    }

    unsigned long int carparkTime = time;

    // if the parkingspace is taken for more then the carparkTime (60 sec), give out the command for the redlight, else the green light
    if (parkTimer + (carparkTime * timer) < millis() && carparkTaken == 1) {
      Serial.print("Parking Time out");
      //redlight
      //loop for updating the colours of the NeoPixel
      for (int i = 0; i < 1; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(150, 50, 50)); // Moderately red color.
        pixels.show(); // This sends the updated pixel color to the hardware.
      }

      //update the the number of cars that have overstayed
      //Setup for the content of the URL/web address
      String url =  "http://verkstad.cc/iot/mmx/";
      url += group;
      url += "/index.php?carparkchecker=";
      url += carparkTaken;
      // Sends the carparkTaken state to the api
      client.get(url); // sends the url to the client
    } else {
      //greenlight
      //loop for updating the colours of the NeoPixel
      for (int i = 0; i < 1; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(50, 150, 50)); // Moderately bright greencolor.
        pixels.show(); // This sends the updated pixel color to the hardware.
      }

      // set the API as no change have happen
      //Setup for the content of the URL/web address
      String url =  "http://verkstad.cc/iot/mmx/";
      url += group;
      url += "/index.php?carparkchecker=";
      url += carparkTaken;
      // Sends the carparkTaken state to the api
      client.get(url); // sends the url to the client
    }
    callurlTimer = millis(); // sets callurlTimer to be millis()/the time the arduino been on

    Serial.flush();//Waits for the transmission of outgoing serial data to complete
  }


  // prints the current light levels and the car parking state in the serial montitor
  if (carparkTaken == 1) {
    carparkTaken = 0; //resets the carparkTaken to 0
    Serial.print("Light levels is: ");
    Serial.println(sensor);
    Serial.println("Car parking space taken ");
  } else {
    Serial.print("Light levels is: ");
    Serial.println(sensor);
    Serial.println("Car parking space not taken ");
  }

  delay(500); //small delay
}


