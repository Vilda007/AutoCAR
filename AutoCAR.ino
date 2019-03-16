long lastJob1s = 0, lastJob30s = 0, lastJob1min = 0;
int  tftw = 0, tfth = 0; // Display width, height
int CarGO = 0;    //go?
int CarOK = 80;   // OK Distance
int CarMIN = 60;  // minimal Distance
int CarXMIN = 35; // minimal Distance (Emergency)
long CarF = 0;    // Distance in Front
long CarB = 0;    // Distance in Back
long CarR = 0;    // Distance on Right
long CarL = 0;    // Distance on Left
long duration, distance;

/*
  US distance Sensors HC-SR04
  Sensor Trigger Echo
  FRONT  49      48
  BACK   51      50
  LEFT   53      52
  RIGHT  23      22
*/
#define echoPinR 22
#define trigPinR 23
#define echoPinF 48
#define trigPinF 49
#define echoPinB 50
#define trigPinB 51
#define echoPinL 52
#define trigPinL 53

#include <Adafruit_GFX.h>    // Core graphics library
#include "SWTFT.h" // Hardware-specific library

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ORANGE  0xFC00

SWTFT tft;


//SETUP-SETUP-SETUP-SETUP-SETUP-SETUP-SETUP-SETUP-SETUP-SETUP-SETUP-SETUP-SETUP-SETUP-SETUP-SETUP-SETUP-SETUP-SETUP-
void setup() {
  Serial.begin(9600);
  Serial.println(F("AutoCAR booting up"));
  delay(10);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);
  pinMode(trigPinF, OUTPUT);
  pinMode(echoPinF, INPUT);
  pinMode(trigPinB, OUTPUT);
  pinMode(echoPinB, INPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tftw = tft.width();
  tfth = tft.height();
  tft.fillScreen(BLACK);
  /*
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Width: ");
    tft.print(tftw);
    tft.println("px");
    tft.print("Height: ");
    tft.print(tfth);
    tft.println("px");
  */

  //LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-
}
void loop() {
  SonarSensor(trigPinF, echoPinF);
  CarF = distance;
  FormatDistance(0, 241, 120, 260, CarF);
  
  SonarSensor(trigPinR, echoPinR);
  CarR = distance;
  FormatDistance(61, 261, 120, 280, CarR);
  
  SonarSensor(trigPinL, echoPinL);
  CarL = distance;
  FormatDistance(0, 261, 60, 280, CarL);
  
  SonarSensor(trigPinB, echoPinB);
  CarB = distance;
  FormatDistance(0, 281, 120, 300, CarB);

  DrawRadar(CarF, CarL, CarB, CarR);

  //LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s
  if (millis() > (1000 + lastJob1s))
  {
    // kód vykonaný každou 1 vteřinu (1000 ms)


    lastJob1s = millis();
  }

  //LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-
  if (millis() > (30000 + lastJob1s))
  {
    // kód vykonaný každých 30 vteřin (30000 ms)


    lastJob1s = millis();
  }

  //LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min
  if (millis() > (60000 + lastJob1s))
  {
    // kód vykonaný každou 1 minutu (60000 ms)


    lastJob1s = millis();
  }

  //LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-
}

//FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS
//FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS
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

void FormatDistance(int rx1, int ry1, int rx2, int ry2, long mydistance)
{
  tft.fillRect(rx1, ry1, rx2, ry2, BLACK);
  tft.setCursor(rx1+((rx2-rx1)/4), ry1);
  if (mydistance > CarXMIN) {
    tft.setTextColor(ORANGE);
  }
  if (mydistance > CarMIN) {
    tft.setTextColor(YELLOW);
  }
  if (mydistance > CarOK)  {
    tft.setTextColor(GREEN);
  }
  if (mydistance <= CarXMIN) {
    tft.setTextColor(RED);
  }
  tft.setTextSize(2);
  tft.print(mydistance);
}

void DrawRadar(long CarF, long CarL, long CarB, long CarR)
{
  if (CarF > CarXMIN) {
    tft.fillTriangle(120, 120, 0, 0, 240, 0, ORANGE);
  }
  if (CarF > CarMIN) {
    tft.fillTriangle(120, 120, 0, 0, 240, 0, YELLOW);
  }
  if (CarF > CarOK)  {
    tft.fillTriangle(120, 120, 0, 0, 240, 0, GREEN);
  }
  if (CarF <= CarXMIN) {
    tft.fillTriangle(120, 120, 0, 0, 240, 0, RED);
  }
  
  if (CarL > CarXMIN) {
    tft.fillTriangle(120, 120, 0, 0, 0, 240, ORANGE);
  }
  if (CarL > CarMIN) {
    tft.fillTriangle(120, 120, 0, 0, 0, 240, YELLOW);
  }
  if (CarL > CarOK)  {
    tft.fillTriangle(120, 120, 0, 0, 0, 240, GREEN);
  }
  if (CarL <= CarXMIN) {
    tft.fillTriangle(120, 120, 0, 0, 0, 240, RED);
  }

  if (CarB > CarXMIN) {
    tft.fillTriangle(120, 120, 240, 240, 0, 240, ORANGE);
  }
  if (CarB > CarMIN) {
    tft.fillTriangle(120, 120, 240, 240, 0, 240, YELLOW);
  }
  if (CarB > CarOK)  {
    tft.fillTriangle(120, 120, 240, 240, 0, 240, GREEN);
  }
  if (CarB <= CarXMIN) {
    tft.fillTriangle(120, 120, 240, 240, 0, 240, RED);
  }

  
  if (CarR > CarXMIN) {
    tft.fillTriangle(120, 120, 240, 240, 240, 0, ORANGE);
  }
  if (CarR > CarMIN) {
    tft.fillTriangle(120, 120, 240, 240, 240, 0, YELLOW);
  }
  if (CarR > CarOK)  {
    tft.fillTriangle(120, 120, 240, 240, 240, 0, GREEN);
  }
  if (CarR <= CarXMIN) {
    tft.fillTriangle(120, 120, 240, 240, 240, 0, RED);
  }
}
