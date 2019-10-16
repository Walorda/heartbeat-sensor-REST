# Project Title

This project is used for monitoring some ones heartbeat on average for a certain amount of time, this will be uploaded to the database using REST
One Paragraph of project description goes here

## Getting Started

Pulsesensor 
### Billing materials
* esp8266 or esp32
* OLED screen
* Speaker
* Pulsesensor from playground.com

### Prerequisites

* PlatformIO or arduino ide
* PostgreSQL running
* npm

### Schematic

![alt text](https://github.com/Walorda/heartbeat-sensor-REST/blob/master/schematic.jpg)


### Architect
![alt text](https://github.com/Walorda/heartbeat-sensor-REST/blob/master/architect.png)

### Admin guide
* To set up the server first clone this project, command line:
```
git clone https://github.com/Walorda/heartbeat-sensor-REST
cd heartbeat-sensor-REST
npm start
```

* Import the database file into the pgAdmin console

* after you have pressed npm start the server should have started with the ip address and port.
* In the main.cpp if you want to change the server location ip address you have to change these 2 lines:
```
const char *http_site = "http://jsonplaceholder.typicode.com/users";
const int http_port = 9000;
```
* Always be sure you change the network ssid and password with these lines:
```
const char *ssid = "IOTTEST";
const char *password = "TESTIOTIOT"; 
```

* Now follow the schematics and build and upload the code to the esp8266 and it should work if done correctly.
