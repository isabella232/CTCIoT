
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
int group = 5;

// teacher short memory api for this exercise
String teacherUrl = "http://verkstad.cc/iot/mmx/10/get_value.php";

// threshold for the temperature  sensor
int threshold = 25;

// 0= seat not taken, 1= seat taken
int seatTaken = 0;

unsigned long int seatTimer = 0; //  seattimer
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
  int sensorRaw = analogRead(0);// from analog sensor 0
  int sensor = (5.0 * analogRead(sensorRaw) * 100.0) / 1024;  // calculate the celsus from the raw value of the sensor
  // set the sensor value to the temperatureCount
  if (sensor > threshold) {

    //the temperature is higher,thus the seat is taken
    seatTaken = 1;
    seatTimer = millis(); // sets  parkTimer to be millis()/the time the arduino been on

  } else {
    // not taken
    seatTaken = 0;
    seatTimer = 0; //reset the time
  }



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

    unsigned long int sittingTime = time;

    // if the seat is taken for more then the sittingTime (60 sec), give out the command for the redlight, else the green light
    if (seatTimer + (sittingTime * timer) < millis() && seatTaken  == 1) {
      Serial.print("Sitting Time out");
      //redlight
      //loop for updating the colours of the NeoPixel
      for (int i = 0; i < 1; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(150, 50, 50)); // Moderately red color.
        pixels.show(); // This sends the updated pixel color to the hardware.
      }

      //update the the number of time someone oversat
      //Setup for the content of the URL/web address
      String url =  "http://verkstad.cc/iot/mmx/";
      url += group;
      url += "/index.php?seatchecker=";
      url += seatTaken;
      // Sends the seatchecker state to the api
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
      url += "/index.php?seatchecker=";
      url += seatTaken;
      // Sends the seatchecker state to the api
      client.get(url); // sends the url to the client
    }
    callurlTimer = millis(); // sets callurlTimer to be millis()/the time the arduino been on

    Serial.flush();//Waits for the transmission of outgoing serial data to complete
  }


  // prints the current temperature and the seat state in the serial montitor
  if (seatTaken == 1) {
    seatTaken = 0;
    Serial.print("Temp is: ");
    Serial.println(sensor);
    Serial.println("Seat taken ");
  } else {
    Serial.print("Temp is: ");
    Serial.println(sensor);
    Serial.println("Seat not taken ");
  }
  delay(500); //small delay

}


