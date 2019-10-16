/*--------Use This Sketch To------------------------------------------
1) Displays Beats per minture to oled screen.
2) Prints average Beats per minute to database.
2) Learn about using a PulseSensor Library "Object".
4) Blinks LED on PIN 13 with user's Heartbeat.
--------------------------------------------------------------------*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define USE_ARDUINO_INTERRUPTS false
#include <PulseSensorPlayground.h>

 // Set-up low-level interrupts for most acurate BPM math.
#define OLED_RESET 0

const char *ssid = "IOTTEST";        // Hoofdlettergevoelig
const char *password = "TESTIOTIOT"; // Hoofdlettergevoelig
const int http_port = 9000;
const int buzzer = D5;
Adafruit_SSD1306 display(OLED_RESET); //Address set here 0x3C that I found in the scanner, and pins defined as D2 (SDA/Serial Data), and D5 (SCK/Serial Clock).

const int OUTPUT_TYPE = SERIAL_PLOTTER;
//  Variables
const int PulseWire = 0; // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 16;    // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;     // Use the getting started project form pulse playground to fine tune this.

PulseSensorPlayground pulseSensor; // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
unsigned long trackTime;           // Tracking time for when to make an API request.

/*
  For displaying an integer to the oled screen.
  @param BPM for displaying bpm to oled
*/
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

  Serial.begin(9600);          // For Serial Monitor
  Serial.setDebugOutput(true); // debugging esp8266 wifi
  display.begin();
  display.setTextWrap(false);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  draw(0);                 // set oled screen to 0
  pinMode(buzzer, OUTPUT); // for making the buzzer buzzzzzzzzz

  // write the buffer to the display

  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);
  pulseSensor.fadeOnPulse(LED13);

  pulseSensor.setThreshold(Threshold);

  // if pulkseensor is not connected or is not initialised yet keep trying
  while (!pulseSensor.begin())
  {
    delay(2000);
    Serial.println("\nwaiting");
  }
  WiFi.begin(ssid, password);         // Connect to WiFi
  WiFi.setSleepMode(WIFI_NONE_SLEEP); // for making the esp8266 less likely to disconnect on an active socket.
  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(2500);
    tone(buzzer, 15000);
    delay(200);
    noTone(buzzer);
    Serial.print(".");
  }
  // IP adres.
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  trackTime = millis(); // so we make an api request the moment the loop starts.
}
int httpCode;
String postdata;
void loop()
{
  int myBPM = pulseSensor.getBeatsPerMinute(); // Calls function on our pulseSensor object that returns BPM as an "int".

  if (trackTime + 250000 <= millis())
  {
    trackTime = millis();
    Serial.println("Time Reached");
    if (WiFi.status() == WL_CONNECTED)
    { //Check WiFi connection status
      Serial.println("Trying to make http request");

      HTTPClient http;
      //http.begin("http://localhost:9000/bpms");
      http.begin("192.168.137.1", http_port, "/bpms"); // specifiy destination, change this to your ip <<<<<<<<<<<<<<<<<<<<<<<
      httpCode = http.GET();                      //Send the request
      if (httpCode > 0)
      {                                    //Check the returning code
        String payload = http.getString(); //Get the request response payload

        Serial.println(payload); //Print the response payload
      }
      http.end();                                                          //Close connection
      http.begin("192.168.137.1", http_port, "/bpm");                           // specifiy destination, change this to your ip <<<<<<<<<<<<<<<<<<<<<<
      http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Specify content-type header
      postdata = "bpm=" + String(myBPM);
      Serial.println(postdata);
      httpCode = http.POST(postdata);
      Serial.println(myBPM); //Send the request
      if (httpCode > 0)
      {
        String payload = http.getString(); //Get the response payload
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
  

  // test if a 'beat' has happend.
  if (pulseSensor.sawNewSample())
  {              
    draw(myBPM); // Print the value inside of myBPM.

  }

}
