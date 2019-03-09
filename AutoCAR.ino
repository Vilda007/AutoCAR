long lastJob250ms = 0, lastJob5s = 0;
int carGO = 0; //go?
int carMIN = 50; // minimal Distance
int carXMIN = 30; // minimal Distance (Emergency)
long carF = 0; // Distance in Front
long carB = 0; // Distance in Back
long carR = 0; // Distance on Right
long carL = 0; // Distance on Left
long duration, distance;

/*
  US Sensor Echo Trigger
  RIGHT     D0   D2
  FRONT     D2   D3
  BACK      D4   D5
  LEFT      D6   D7
*/
#define echoPinR 0
#define trigPinR 2
#define echoPinF 3
#define trigPinF 4
#define echoPinB 5
#define trigPinB 6
#define echoPinL 7
#define trigPinL 8

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>
//#include <ArduinoOTA.h>

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

void handleRoot();
void handleGo();
void handleNotFound();
void drawGraph();

//SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP
//SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP
void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);

  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);
  pinMode(trigPinF, OUTPUT);
  pinMode(echoPinF, INPUT);
  pinMode(trigPinB, OUTPUT);
  pinMode(echoPinB, INPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);

  WiFi.mode(WIFI_STA);

  wifiMulti.addAP("HOUSLEcz-top", "guarneri");
  wifiMulti.addAP("HOUSLEcz-vrch", "guarneri");
  wifiMulti.addAP("HOUSLEcz-dole", "guarneri");
  wifiMulti.addAP("krabitchka", "guarneri");
  wifiMulti.addAP("HOUSLEcz-modem", "guarneri");
  wifiMulti.addAP("iTRUBEC", "1234567890");

  Serial.println("");
  Serial.print("Connecting");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(1000);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("autocar")) {              // http://autocar.home
    Serial.print("MDNS responder started: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.print(" - ");
    Serial.print("http://autocar.local");
    Serial.print(" - ");
    Serial.println("http://autocar.home");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  //webserver pages - Start
  server.on("/", handleRoot);
  server.on("/go", handleGo);
  server.on("/test.svg", drawGraph);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  //webserver pages - END
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  /*
    // OTA
    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    ArduinoOTA.setHostname("AutoCAR");

    // No authentication by default
    // ArduinoOTA.setPassword((const char *)"amati");

    ArduinoOTA.onStart([]() {
    Serial.println("OTA Start");
    });
    ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA End");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("OTA Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("OTA Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("OTA Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("OTA Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("OTA End Failed");
    });
    ArduinoOTA.begin();
  */
}

//LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP
//LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP
void loop() {
  //ArduinoOTA.handle();

  // LOOP-BLOCK-250ms-LOOP-BLOCK-250ms-LOOP-BLOCK-250ms-LOOP-BLOCK-250ms-LOOP-BLOCK-250ms-LOOP-BLOCK-250ms-LOOP-BLOCK-250ms-LOOP-BLOCK-250ms-LOOP-BLOCK-250ms-LOOP-BLOCK-250ms-
  if (millis() > (250 + lastJob250ms))
  {
    // runs every 250 miliseconds (250 ms)
    server.handleClient();
    SonarSensor(trigPinF, echoPinF);
    carF = distance;/*
    SonarSensor(trigPinR, echoPinR);
    carR = distance;
    SonarSensor(trigPinL, echoPinL);
    carL = distance;
    SonarSensor(trigPinB, echoPinB);
    carB = distance;*/
    lastJob250ms = millis();
  } // 5s end

  // LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-
  if (millis() > (5000 + lastJob5s))
  {
    // runs every 5 seconds (5000 ms)
    MDNS.update();
    Serial.print(carF);
    Serial.print(", ");
    Serial.print(carR);
    Serial.print(", ");
    Serial.print(carL);
    Serial.print(", ");
    Serial.print(carB);
    Serial.println("");
    lastJob5s = millis();
  } // 5s end
}

//FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS
//FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS
void handleRoot() {
  Serial.println("HTTP server: Root page requested!");
  carGO = 0;
  Serial.println("AutoCAR STOP!");
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  // <meta http-equiv='refresh' content='5'/>
  // <img src=\"/test.svg\" />
  snprintf(temp, 400,
           "<html>\
  <head>\
    <title>AutoCAR - Autonomous Car</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
      button { background-color: #00ff00; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>AutoCAR STOP</h1>\
    <form method=\"get\" action=\"/go\">\
       <button type=\"submit\">AutoCAR GO!</button>\
    </form>\
    <p>Uptime: %02d:%02d:%02d</p>\
  </body>\
  </html>",
           hr, min % 60, sec % 60
          );
  server.send(200, "text/html", temp);
}

void handleGo() {
  Serial.println("HTTP server: Go page requested!");
  carGO = 1;
  Serial.println("AutoCAR GO!");
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  snprintf(temp, 400,
           "<html>\
  <head>\
    <title>AutoCAR - Autonomous Car GO</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
      button { background-color: #ff0000; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>AutoCAR GO!</h1>\
    <form method=\"get\" action=\"/\">\
       <button type=\"submit\">AutoCAR STOP!</button>\
    </form>\
    <p>Uptime: %02d:%02d:%02d</p>\
  </body>\
  </html>",
           hr, min % 60, sec % 60
          );
  server.send(200, "text/html", temp);
}

void handleNotFound() {
  Serial.println("HTTP server: Page not found!");
  String message = "AutoCAR Error: File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void drawGraph() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x += 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}

void SonarSensor(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
}
