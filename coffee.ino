/*************************************************************
 ESP8266    ARDUINO
 ------------------
 RX      ->  3
 GND     ->  GND
 VCC     ->  3.3V
 TX      ->  2
 CH_PD   ->  3.3V
 
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <math.h>
#define earthRadiusKm 6371.0

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "914edba0608741c4ad883564f8431e3a";
//char auth[] = "6fbf8267698c4b50b0456c7b4e9cc03d";
char auth[] = "8b67b5ee09394fd89b8a7ef9567402e9";

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "Chris";
//char pass[] = "e44ggbvswiqyb";
//char ssid[] = "RESNET-WARREN-WEST";
//char pass[] = "";
//char ssid[] = "Verizon VS990 66CF";
//char pass[] = "a]6eW@8a";

char ssid[] = "Nancy";
char pass[] = "gintama1";

// Hardware Serial on Mega, Leonardo, Micro...
// #define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

double currentLat = 32.883490;
double currentLong = -117.241500;
double phoneLat = currentLat;
double phoneLong = currentLong;
const double maxKmRange = 0.02;

int coffeePin = 13;

// This function converts decimal degrees to radians
double deg2rad(double deg) {
  return (deg * M_PI / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
  return (rad * 180 / M_PI);
}

double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

BLYNK_WRITE(V13) {
  phoneLat = param[0].asFloat();
  phoneLong = param[1].asFloat();

  pinMode(coffeePin, OUTPUT);
  double dist = distanceEarth(currentLat, currentLong, phoneLat, phoneLong);
  if (dist <= maxKmRange)
  {
    digitalWrite(coffeePin, HIGH);
  }
  else
  {
    digitalWrite(coffeePin, LOW);
  }
  Serial.println(dist);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  delay(10);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
}

void loop()
{
  Blynk.run();
}

