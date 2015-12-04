//Libraries for the Yúns connection
#include <Bridge.h>// communication between the ATmega32U4(the arduino) and the AR9331 (Linux side)
#include <HttpClient.h> //Creates a HTTP client on Linux.

HttpClient client; // the HTTP client

// ask the temperature in London, GB
// the web address / url to get the temperature

// Country and city for the message
String cityName = "London"; // name of your picked city
String countryCode = "GB"; // country code for that city

//counter for the number of tweets that have been sent
int numberofTweets = 0;

//  temperature variable
int temperature = 0;
//  led for the temperature
int  temperatureLed = 9;

int buttonPin = 2;    // the number of the button pin
int buttonState = 0;   // variable for reading the button status
int buttonSwitch = 0; // variable for Switching the button on and off
int buttonPushed = 0; // variable for checking if the button been pushed or not


unsigned long int callurlTimer = 0; //  url timer
unsigned long int callurlDelay = 10000; // url delay. 1000=1 sec

void setup() {
  // configure the 13 pin as a OUTPUT
  pinMode(13, OUTPUT);


  // Blink once with the led on pin 13 when the connection is ready
  digitalWrite(13, LOW);
  Bridge.begin(); //starts the Bridge
  digitalWrite(13, HIGH);

  // configure the serial
  Serial.begin(9600);

  while (!Serial);// waits for the serial monitor to open
}

void loop() {

  // read the state of the button value:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH && buttonSwitch == LOW) { //button is pushed
    buttonPushed = 1;
  }

  // to slow down the amount of calls to the url
  if (callurlTimer + callurlDelay < millis()) {

    //Setup for the content of the URL/web address
    // get the url
    String country = "http://verkstad.cc/iot/mmx/code/get_temperature.php?city=";
    country += cityName; // adds in the city name in the url
    country += "&country="; // adds the country part to the url
    country += countryCode; // adds the country code to the url
    client.get(country);
    // prints the current URL in the serial montitor
    Serial.println("");
    Serial.print("Current URL is :");
    Serial.println(country);

    // set the temperature to 0
    temperature = 0;

    int temperatureDec = 0; // varabile for handling the decimals in the temperature
    boolean temperatureDecimals = false; //boolean for checking if the temperature has decimals
    boolean temperatureNegative = false;//boolean for checking if the temperature is negative





    // get the temperature and set it to the temperature variable
    while (client.available()) {
      char c = client.read();
      //check if the temperature is negative
      if (c == '-') {
        temperatureNegative = true;
      } else if (c == '.') {//check if the temperature has decimals
        temperatureDecimals = true;
      } else if (!temperatureDecimals) {
        temperature = temperature  * 10 + int(c) - 48;
      } else {
        temperatureDec = temperatureDec  * 10 + int(c) - 48;
      }
    }
    // remove the decimals, rounding down to a full number
    temperature += temperatureDec * 0.1;
    // if the temperature was negative, change the variable to negative
    if (temperatureNegative) {
      temperature = temperature - (temperature * 2);
    }


    Serial.flush();//Waits for the transmission of outgoing serial data to complete



    // check if the button have been pushed

    if (buttonPushed == 1 ) {

      //Setup for the content off the URL/web address
      String twitterAddress = "http://verkstad.cc/iot/mmx/code/twitter/code/sendTweet.php?pass=Schl400ch3R.&message=";
      //build your message
      String url = "";
      String message = "Its ";
      message += temperature; // adds the temperature
      // write in what city you have picked
      message += " degrees in ";
      message += cityName; // adds the city to the message
      message += ", " ; // adds a "," in the messade
      message += countryCode; // adds the county code to the message

      //add the number of the tweet
      message += ",Tweet ";
      numberofTweets++; //add one to the tweet counter
      message += numberofTweets;
      message = urlEncode(message); // This sets the message to put in the url
      url = twitterAddress + message; // join the address and the message
      client.get(url); // call the url to send the tweet

      Serial.println("");
      Serial.println("Tweet sent");
      // prints the current URLthat was send out in the serial montitor
      Serial.print("URL for the tweet is: ");
      Serial.println(url);

      buttonPushed = 0; // reset the buttonPushed'
    }
    Serial.flush();//Waits for the transmission of outgoing serial data to complete

  }





  buttonSwitch = buttonState;  // sets the buttonstate to the buttonswitch
  Serial.println("");
  // prints the current temperature in the serial montitor
  Serial.print("Temperature in ");
  Serial.print(cityName);
  Serial.print(", ");
  Serial.print(countryCode);
  Serial.print(" is ");
  Serial.println(temperature);

  delay(250); // small delay
}


//small function to change the text to url format
String urlEncode(String s) {
  for (int i = 0; i < s.length(); i++) {
    if (s[i] == ' ') s[i] = '+';
  }
  return s;
}


