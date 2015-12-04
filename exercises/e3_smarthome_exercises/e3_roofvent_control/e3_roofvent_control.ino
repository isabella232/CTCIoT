
//Libraries for the Yúns connection
#include <Bridge.h>// communication between the ATmega32U4(the arduino) and the AR9331 (Linux side)
#include <HttpClient.h> //Creates a HTTP client on Linux.

HttpClient client; // the HTTP client

// your group number
int group = 2;

// ask the temperature the house from the teacher api
// the web address / url to get the temperature
String temperatureUrl = "http://verkstad.cc/iot/mmx/10/get_value.php";



//Libraries for the NeoPixel
#include <Adafruit_NeoPixel.h>
// Which pin on the Arduino is connected to the NeoPixel
#define PIN            6  // pin for the NeoPixel
// SETUP for NeoPixel
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800); // Setup for the NeoPixel


// treshhold for what color the RGBl LED should be on
int coldTreshhold = 10; //lower temperature
int hotTreshhold = 20; //higher temperature

int buttonPin = 2;    // the number of the button pin
int buttonState = 0;   // variable for reading the button status
int tempControl = 0; // variable for for contoling the temperature, 0=off, 1= on

unsigned long int callurlTimer = 0; //  url timer
unsigned long int callurlDelay = 5000; // url delay. 1000=1 sec

void setup() {
  // configure the 13 pin as a OUTPUT
  pinMode(13, OUTPUT);


  // Blink once with the led on pin 13 when the connection is ready
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);



  pixels.begin(); // This initializes the NeoPixel library.
  // initialize the button pin as an input:
  pinMode(buttonPin, INPUT);

  // configure the serial
  Serial.begin(9600);

  while (!Serial);// waits for the serial monitor to open
}

void loop() {
  // get the country url
  client.get(temperatureUrl); //sends the url to the client

  // set the temperature to 0
  int temperature = 0;

  // get the temperature and set it to the temperature variable
  while (client.available()) {
    char c = client.read();
    temperature = temperature  * 10 + int(c) - 48;
  }
  Serial.flush();//Waits for the transmission of outgoing serial data to complete

  //check if the temperature is hot, cold or inbetween
  if ( temperature > hotTreshhold ) {
    // hot light
    //loop for updating the colours of the NeoPixel
    for (int i = 0; i < 1; i++) {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(150, 50, 0)); // Moderately red color.
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
  }
  else if (temperature > coldTreshhold && temperature < hotTreshhold) {
    // not hotter then the hot treshhold but hotter then the cold one
    // medium light
    //loop for updating the colours of the NeoPixel
    for (int i = 0; i < 1; i++) {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(150, 150, 150)); // Moderately bright white color.
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
  } else {
    // if its colder then the cold treshhold
    // cold light
    //loop for updating the colours of the NeoPixel
    for (int i = 0; i < 1; i++) {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(50, 50, 150)); // Moderately bright blue color.
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
  }


  // read the state of the button value:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {//button is pushed

    //changes the tempContol on if it was off, and off if it was on
    if (tempControl == 0) {
      tempControl = 1;

    } else {
      tempControl = 0;

    }
  }

  // to slow down the amount of calls to the url
  if (callurlTimer + callurlDelay < millis()) {


    //send the temperature to the Short Memory API
    //Setup for the content of the URL/web address
    String url =  "http://verkstad.cc/iot/mmx/";
    url += group;
    url += "/index.php?tempControl=";
    url += tempControl;
    // Sends the tempControl value to the api
    client.get(url); // sends the url to the client
    Serial.flush();//Waits for the transmission of outgoing serial data to complete

    callurlTimer = millis();// sets callurlTimer to be millis()/the time the arduino been on

  }
  // prints the current temperature and the Temp control state in the serial montitor
  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Temp control state: ");
  Serial.println(tempControl);

  delay(250); // small delay
}



