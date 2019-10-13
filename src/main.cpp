/*  Getting_BPM_to_Monitor prints the BPM to the Serial Monitor, using the least lines of code and PulseSensor Library.
 *  Tutorial Webpage: https://pulsesensor.com/pages/getting-advanced
 *
--------Use This Sketch To------------------------------------------
1) Displays user's live and changing BPM, Beats Per Minute, in Arduino's native Serial Monitor.
2) Print: "♥  A HeartBeat Happened !" when a beat is detected, live.
2) Learn about using a PulseSensor Library "Object".
4) Blinks LED on PIN 13 with user's Heartbeat.
--------------------------------------------------------------------*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"
#define USE_ARDUINO_INTERRUPTS false // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>   // Includes the PulseSensorPlayground Library.
#define OLED_RESET 0
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char *ssid = "IOTTEST";         // Hoofdlettergevoelig
const char *password = "TESTIOTIOT"; // Hoofdlettergevoelig
const char *http_site = "http://jsonplaceholder.typicode.com/users";
const int http_port = 9000;
Adafruit_SSD1306 display(OLED_RESET); //Address set here 0x3C that I found in the scanner, and pins defined as D2 (SDA/Serial Data), and D5 (SCK/Serial Clock).

const int OUTPUT_TYPE = SERIAL_PLOTTER;
//  Variables
const int PulseWire = 0; // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;    // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;     // Determine which Signal to "count as a beat" and which to ignore.
                         // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                         // Otherwise leave the default "550" value.

PulseSensorPlayground pulseSensor; // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
unsigned long trackTime;
void draw(int BPM)
{

  display.clearDisplay();
  display.setTextWrap(false);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  //Add stuff into the 'display buffer'
  Serial.println(BPM);
  //String BPMString = String("BPM" + 100);
  display.printf("BPM %d", BPM);
  //display.print(BPM);

  display.display(); //output 'display buffer' to screen
}

void setup()
{

  Serial.begin(9600); // For Serial Monitor
  Serial.setDebugOutput(true);
  display.begin();
  display.setTextWrap(false);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  draw(0);

  // write the buffer to the display

  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);
  pulseSensor.fadeOnPulse(LED13);

  //pulseSensor.setSerial(Serial);
  //pulseSensor.setOutputType(OUTPUT_TYPE);
  pulseSensor.setThreshold(Threshold);

  // Double-check the "pulseSensor" object was created and "began" seeing a signal.
  while (!pulseSensor.begin())
  {
    delay(2000);
    Serial.println("\nwaiting"); //This prints one time at Arduino power-up,  or on Arduino reset.
  }
  WiFi.begin(ssid, password); // Connect to WiFi
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // IP adres.
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  trackTime = millis();
}
int httpCode;
String postdata;
void loop()
{
  int myBPM = pulseSensor.getBeatsPerMinute(); // Calls function on our pulseSensor object that returns BPM as an "int".

  if(trackTime + 250000 <= millis()){
    trackTime = millis();
    Serial.println("Time Reached");
    if (WiFi.status() == WL_CONNECTED)
    { //Check WiFi connection status
      Serial.println("Trying to make http request");

      HTTPClient http;                          //Declare an object of class HTTPClient
      //http.begin("http://localhost:9000/bpms");
      http.begin("192.168.137.1",9000,"/bpms"); //Specify request destination
      httpCode = http.GET();                //Send the request
      if (httpCode > 0)
      {                                    //Check the returning code
        String payload = http.getString(); //Get the request response payload

        Serial.println(payload);           //Print the response payload
      }
      http.end();                                              //Close connection
      http.begin("192.168.137.1",9000,"/bpm"); //Specify request destination
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");            //Specify content-type header
      postdata = "bpm=" + String(myBPM);
      Serial.println(postdata);
      httpCode = http.POST(postdata);
      Serial.println(myBPM);               //Send the request
      if(httpCode > 0){
      String payload = http.getString();                       //Get the response payload
      Serial.println("Return code");
      Serial.println(httpCode); //Print HTTP return code
      Serial.println(payload);  //Print request response payload
      Serial.println("Printed request response payload");
      
      http.end(); //Close connection
      }
    }
    else
    {
      Serial.println("Error in WiFi connection");
    }
  }
  //int heartValue = analogRead(PulseWire);    //reading the analog value
  //Serial.println(heartValue);
  
                                               // "myBPM" hold this BPM value now.
  if (pulseSensor.sawNewSample())
  { // Constantly test to see if "a beat happened".
    draw(myBPM); // Print the value inside of myBPM.
    
    // TODO buzzzzzzzzzzzz
  }
  // considered best practice in a simple sketch.
}
