
//Libraries for the Yúns connection
#include <Bridge.h>// communication between the ATmega32U4(the arduino) and the AR9331 (Linux side)
#include <HttpClient.h> //Creates a HTTP client on Linux.

HttpClient client; // the HTTP client

//Libraries for the NeoPixel
#include <Adafruit_NeoPixel.h>
// Which pin on the Arduino is connected to the NeoPixel
#define PIN            6  // pin for the NeoPixel

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800); // Setup for the NeoPixel


//Library for the CapacitiveSensor
#include <CapacitiveSensor.h>
// set the pins that are used, in this case 4 and 2
CapacitiveSensor   cs_4_2 = CapacitiveSensor(4, 2);
// 10k resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired



// your group number
int group =4;

// teacher short memory api for this exercise
String teacherUrl = "http://verkstad.cc/iot/mmx/9/get_value.php";


//  number of coupons taken
int couponCount = 0;
// threshold for the sensitivity for the sensor
int threshold = 10;

// threshold for the sensitivity for the sensor
int thresholdLow = 5;
int thresholdHigh = 15;

// for checking if the couponCount value have been low once before its high
int low = 0;

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
  //reset count on red light and stop counting
  // Take in the value from the sensor
  long sensor =  cs_4_2.capacitiveSensor(30);


  // if the sensor value is bigger then the threshold, add to the coupon count
  if (sensor < thresholdLow) {
    low = 1;
  } else if (sensor > thresholdHigh && low == 1) {
    couponCount++;
    low = 0;
  }

  // to slow down the amount of calls to the url
  if (callurlTimer + callurlDelay < millis()) {
    //check the control units current values
    client.get(teacherUrl); // sends the url to the client

    //the time for the car to stay parked from the api
    int  limit = 0;

    // get the  statusLight value
    while (client.available()) {
      char c = client.read();
      limit = limit * 10 + c - 48;
    }

    unsigned long int couponLimit = limit;

    // if the couponCount is under the limit have the red light on else greenlight
    if ( couponCount < couponLimit) {

      //redlight
      //loop for updating the colours of the NeoPixel
      for (int i = 0; i < 1; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(150, 50, 50)); // Moderately red color.
        pixels.show(); // This sends the updated pixel color to the hardware.
      }

      //update the the a coupon has been dispensed
      //Setup for the content of the URL/web address
      String url =  "http://verkstad.cc/iot/mmx/";
      url += group;
      url += "/index.php?couponCount=";
      url += 0;
      // Sends the coupon state to the api
      client.get(url); // sends the url to the client
    } else {
      Serial.print("Coupon out");
      //greenlight
      //loop for updating the colours of the NeoPixel
      for (int i = 0; i < 1; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(50, 150, 50)); // Moderately bright greencolor.
        pixels.show(); // This sends the updated pixel color to the hardware.
      }

      // tell the API that a coupon have been dispensed
      //Setup for the content of the URL/web address
      String url =  "http://verkstad.cc/iot/mmx/";
      url += group;
      url += "/index.php?couponCount=";
      url += 1; // add a to the coupon count
      // Sends the coupon state to the api
      client.get(url); // sends the url to the client

      couponCount = 0; //reset the couponCount
      delay(5000); // delay the counting so that not to many coupons get out

    }
    callurlTimer = millis(); // sets callurlTimer to be millis()/the time the arduino been on

    Serial.flush();//Waits for the transmission of outgoing serial data to complete
  }

  // prints the current coupon count in the serial montitor
  Serial.print("couponCount: ");
  Serial.println(couponCount);

  delay(500); //small delay

}



