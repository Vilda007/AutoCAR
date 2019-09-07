#include <SWTFT.h>
#include <TouchScreen.h>

long lastJob1s = 0,lastJob5s = 0,  lastJob10s = 0, lastJob30s = 0, lastJob1m = 0;
int  tftw = 0, tfth = 0; // Display width, height
int CarGO = 0;    //go?
int CarOK = 75;   // OK Distance
int CarMIN = 45;  // minimal Distance
int CarXMIN = 25; // minimal Distance (Emergency)
long CarF = 0;    // Distance in Front
long CarB = 0;    // Distance in Back
long CarR = 0;    // Distance on Right
long CarL = 0;    // Distance on Left
long duration, distance;

/*
Chasis:
RED     +6 V
BLACK   GND

YELLOW - & GREY  +  => FORWARDgg
YELLOW + & GREY  -  => BACKWARD
BLUE   + & WHITE -  => RIGHT
BLUE   - & WHITE +  => LEFT

US distance Sensors HC-SR04
       Violet  White
Sensor Trigger Echo
FRONT  49      48
BACK   51      50
LEFT   53      52
RIGHT  23      22

Relays
MAIN F/B - orange PIN 24
MAIN L/R - yellow PIN 25

4 relays board
F/B (1&2) - blue  PIN 30
L/R (3&4) - green PIN 31
*/

//Relay pins
#define releMainFBPIN 24
#define releMainLRPIN 25
#define releFBPIN 30
#define releLRPIN 31

//US sensors PINs
#define echoPinR 22
#define trigPinR 23
#define echoPinF 48
#define trigPinF 49
#define echoPinB 50
#define trigPinB 51
#define echoPinL 52
#define trigPinL 53

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
  pinMode(releMainFBPIN, OUTPUT);
  pinMode(releMainLRPIN, OUTPUT);
  pinMode(releFBPIN, OUTPUT);
  pinMode(releLRPIN, OUTPUT);
  digitalWrite(releMainFBPIN, HIGH);
  digitalWrite(releMainLRPIN, HIGH);
  digitalWrite(releFBPIN, HIGH);
  digitalWrite(releLRPIN, HIGH);
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tftw = tft.width();
  tfth = tft.height();
  tft.fillScreen(BLACK);
}

//LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-LOOP-
void loop() {
  
  SonarSensor(trigPinF, echoPinF);
  CarF = distance;
  //FormatDistance(0, 241, 120, 260, CarF);

  SonarSensor(trigPinR, echoPinR);
  CarR = distance;
  //FormatDistance(61, 261, 120, 280, CarR);

  SonarSensor(trigPinL, echoPinL);
  CarL = distance;
  //FormatDistance(0, 261, 60, 280, CarL);

  SonarSensor(trigPinB, echoPinB);
  CarB = distance;
  //FormatDistance(0, 281, 120, 300, CarB);

  DrawRadar(CarF, CarL, CarB, CarR);

  if ((CarF < CarXMIN) && (CarB < CarXMIN) && (CarL < CarXMIN) && (CarR < CarXMIN)){
    CarGO = 0;
  } else {
    CarGO = 1;
  }

  /*
  if (() && (CarGO == 1)) {
    
  }
  */

  if ((CarF > CarMIN) && (CarGO == 1)) {
    GoFWD();
  } else {
    if ((CarB > CarF) && (CarGO == 1)) {
      if ((CarL > CarR) && (CarGO == 1)) {
        TurnLEFT();
        GoBWD();
      }
      if ((CarL < CarR) && (CarGO == 1)) {
        TurnRIGHT();
        GoBWD();
      }
    } else {
      GoSTOP();  
    }
  }

  if ((CarL > CarOK) && (CarR > CarOK) && (CarGO == 1)) {
    GoSTRAIT();
  } else {
    if ((CarL > CarR) && (CarGO == 1)) {
      TurnLEFT();
    } else {
      if ((CarR > CarL) && (CarGO == 1)) {
        TurnRIGHT();
      } else {
        GoSTOP();  
      }  
    } 
  }
  
  //LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s-LOOP-BLOCK-1s
  if (millis() > (1000 + lastJob1s))
  {
    // kód vykonaný každou 1 vteřinu (1000 ms)


    lastJob1s = millis();
  }

  //LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s-LOOP-BLOCK-5s
  if (millis() > (5000 + lastJob5s))
  {
    // kód vykonaný každých 5 vteřin (5000 ms)
    

    lastJob5s = millis();
  }

    //LOOP-BLOCK-10s-LOOP-BLOCK-10s-LOOP-BLOCK-10s-LOOP-BLOCK-10s-LOOP-BLOCK-10s-LOOP-BLOCK-10s-LOOP-BLOCK-10s-LOOP-BLOCK-10s-LOOP-BLOCK-10s-LOOP-BLOCK-10s-LOOP-BLOCK-10s-LOOP-BLOCK-10s
  if (millis() > (10000 + lastJob10s))
  {
    // kód vykonaný každych 10 vteřin (10000 ms)

    lastJob10s = millis();
  }

  //LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-LOOP-BLOCK-30s-
  if (millis() > (30000 + lastJob30s))
  {
    // kód vykonaný každých 30 vteřin (30000 ms)
    

    lastJob30s = millis();
  }

  //LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min-LOOP-BLOCK-1min
  if (millis() > (60000 + lastJob1m))
  {
    // kód vykonaný každou 1 minutu (60000 ms)


    lastJob1m = millis();
  }

  //LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-LOOP-BLOCK-
}

//FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS
//FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS
void SonarSensor(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  //delayMicroseconds(2);
  delayMicroseconds(1);
  digitalWrite(trigPin, HIGH);
  //delayMicroseconds(5);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.31;
  if (distance > 110) {
    distance = 110;
  }
}

//STOP
void GoSTOP(){
  digitalWrite(releMainFBPIN, HIGH);
  digitalWrite(releMainLRPIN, HIGH);
  digitalWrite(releFBPIN, HIGH);
  digitalWrite(releLRPIN, HIGH);
  Serial.println("STOP");
}

//FORWARDS
void GoFWD(){
  digitalWrite(releMainFBPIN, HIGH);
  digitalWrite(releFBPIN, HIGH);
  digitalWrite(releMainFBPIN, LOW);
  Serial.println("Forwards");
}

//BACKWARDS
void GoBWD(){
  digitalWrite(releMainFBPIN, HIGH);
  digitalWrite(releFBPIN, LOW);
  digitalWrite(releMainFBPIN, LOW);
  Serial.println("Backwards");
}

//TURN LEFT
void TurnLEFT(){
  digitalWrite(releMainLRPIN, HIGH);
  digitalWrite(releLRPIN, LOW);
  digitalWrite(releMainLRPIN, LOW);
  Serial.println("Turning LEFT");
}

//TURN RIGHT
void TurnRIGHT(){
  digitalWrite(releMainLRPIN, HIGH);
  digitalWrite(releLRPIN, HIGH);
  digitalWrite(releMainLRPIN, LOW);
  Serial.println("Turning RIGHT");
}

//GO STRAIT
void GoSTRAIT(){
  digitalWrite(releMainLRPIN, HIGH);
  Serial.println("Going STRAIT");
}

uint16_t Colorize (long mydistance) {
  uint16_t result;
  if (mydistance > CarOK)  {
    result = GREEN;
  }
  if ((mydistance <= CarOK) && (mydistance > CarMIN))  {
    result = ORANGE;
  }
  if ((mydistance <= CarMIN) && (mydistance > CarXMIN))  {
    result = YELLOW;
  }
  if (mydistance <= CarXMIN)  {
    result = RED;
  }
  return result;
}

void FormatDistance(int rx1, int ry1, int rx2, int ry2, long mydistance)
{
  tft.fillRect(rx1, ry1, rx2, ry2, BLACK);
  tft.setCursor(rx1 + ((rx2 - rx1) / 4), ry1);
  tft.setTextColor(Colorize(mydistance));
  tft.setTextSize(2);
  if (mydistance == 110) {
    tft.print("free");
  } else {
    tft.print(mydistance);
  }
}

void DrawRadar(long CarF, long CarL, long CarB, long CarR)
{
  tft.fillRect(100, 0, 40, 100, Colorize(CarF));
  tft.fillRect(0, 100, 100, 40, Colorize(CarL));
  tft.fillRect(100, 140, 40, 100, Colorize(CarB));
  tft.fillRect(140, 100, 100, 40, Colorize(CarR));
}
