

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
int group = 2;

// teacher short memory api for this exercise
String teacherUrl = "http://verkstad.cc/iot/mmx/7/get_value.php";

//  number of cars passed
int carCount = 0;
// threshold for the sensitivity for the sensor
int threshold = 100;
// the old number of cars passed
int oldcarCount = 1;

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
  int sensor = analogRead(0);// from analog sensor 0


  // if the sensor value is bigger then the threshold, add to the car count
  if (sensor > threshold) {
    carCount++;
  }

  // to slow down the amount of calls to the url
  if (callurlTimer + callurlDelay < millis()) {


    //check the control units current values
    client.get(teacherUrl); // sends the url to the client

    //the number of cars from the api
    int carLimit = 0;

    // get the  carLimit value
    while (client.available()) {
      char c = client.read();
      carLimit = carLimit * 10 + c - 48;
    }
    // cut the Carcount number in half as most cars has 2 rows of wheels that whould have passes the sensor
    int currentCarcount = carCount / 2;

    if (currentCarcount >= carLimit) {
      Serial.println ("Car limit reached");
      //redlight

      //loop for updating the colours of the NeoPixel
      for (int i = 0; i < 1; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(150, 50, 50)); // Moderately red color.
        pixels.show(); // This sends the updated pixel color to the hardware.
      }

      //update the set of the carlimit that have happend
      //Setup for the content of the URL/web address
      String url =  "http://verkstad.cc/iot/mmx/";
      url += group;
      url += "/index.php?carCount=";
      url += 1; //add to the number of set of the carlimit have happend
      // Sends the Carcount state to the api
      client.get(url); // sends the url to the client

      carCount = 0; //reset the carCount
    } else if (currentCarcount <= carLimit && currentCarcount >= (carLimit / 2)) {

      //yellowlight

      //loop for updating the colours of the NeoPixel
      for (int i = 0; i < 1; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(150, 150, 0)); // Moderately bright yellowcolor.
        pixels.show(); // This sends the updated pixel color to the hardware.
      }
      // set the API as no new set have happen
      //Setup for the content of the URL/web address
      String url =  "http://verkstad.cc/iot/mmx/";
      url += group;
      url += "/index.php?carCount=";
      url += 0;
      // Sends the carparkTaken state to the api
      client.get(url); // sends the url to the client
    } else  {

      //greenlight

      //loop for updating the colours of the NeoPixel
      for (int i = 0; i < 1; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(50, 150, 50)); // Moderately bright greencolor.
        pixels.show(); // This sends the updated pixel color to the hardware.
      }
      // set the API as no new set have happen
      //Setup for the content of the URL/web address
      String url =  "http://verkstad.cc/iot/mmx/";
      url += group;
      url += "/index.php?carCount=";
      url += 0;
      // Sends the carparkTaken state to the api
      client.get(url); // sends the url to the client
    }

    callurlTimer = millis(); // sets callurlTimer to be millis()/the time the arduino been on
    Serial.flush();//Waits for the transmission of outgoing serial data to complete
  }



  // prints the current car count in the serial montitor
  Serial.print("carCount: ");
  Serial.println(carCount / 2);



  delay(500); //small delay

}


