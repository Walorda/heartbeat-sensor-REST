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



### Running



End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc
