#ifdef ESP32
#include <WiFi.h>
#define VOLTAGE_INDEXER 188
#elif ESP8266
#include <ESP8266WiFi.h>
#define VOLTAGE_INDEXER 1238
#endif

#include <ArduinoOTA.h>

#include <Adafruit_NeoPixel.h>

bool debug = true;
bool ota_mode = false;

#define NUM 5
#define GROUP_NUM 20
byte ttl = 2;

// const char* ssid = "ExSelf";
// const char* pass = "gggggggggg";

uint8_t command = 15;
uint8_t lastprogram;
uint8_t parameter;
uint8_t constant_commands[12];

boolean state;
boolean ledState;
unsigned long previousMillis;
unsigned long StrobeMillis;
byte interval = 25;
unsigned long lastbutton;

byte d = 75;
int sinInterval = 3000;

byte bright;
#define breatheint1 250
#define breatheint2 2500
#define pressdelay 750
#define MIN_BRIGHT 16
#define MAX_BRIGHT 192
#define HANDS_MAX_BRIGHT 255  //64
#define MIN_VOLTAGE 9500
#define MAX_VOLTAGE 12600
#define SAFE_CHARGE 75
#define VOLTAGE_CHECK_INTERVAL 10
#define STATUS_SEND_INTERVAL 25
#define WIFI_CHECK_INTERVAL 2500
#define NUMBER_OF_READS 100
// #define VOLTAGE_INDEXER 188


// costume 2,3,4,5

#define INNTER_DATA_PIN 35  // LV3 U2
#define EXTRA_DATA_PIN 33   // LV2 U3
#define OUTER_DATA_PIN 34   // LV1 U4
#define MIDDLE_DATA_PIN 36  // LV4 U5

#define INNER_NUM_PIXELS 55
#define MIDDLE_NUM_PIXELS 62
#define OUTER_NUM_PIXELS 68

// all others

// #define INNTER_DATA_PIN 35 // LV3 U2
// #define EXTRA_DATA_PIN 34 // LV2 U3
// #define OUTER_DATA_PIN 33 // LV1 U4
// #define MIDDLE_DATA_PIN 36 // LV4 U5

// #define INNER_NUM_PIXELS 34
// #define MIDDLE_NUM_PIXELS 30
// #define OUTER_NUM_PIXELS 98

#ifdef ESP32

#define INNER_PWM_PIN 1   //Q1
#define MIDDLE_PWM_PIN 2  //Q2
#define OUTER_PWM_PIN 4   //Q3
#define HANDS_PWM_PIN 6   //Q4

#elif ESP8266

#define INNER_PWM_PIN 13
#define MIDDLE_PWM_PIN 15
#define OUTER_PWM_PIN 12
#define HANDS_PWM_PIN 2

#endif

Adafruit_NeoPixel inner(INNER_NUM_PIXELS, INNTER_DATA_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel middle(MIDDLE_NUM_PIXELS, MIDDLE_DATA_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel outer(OUTER_NUM_PIXELS, OUTER_DATA_PIN, NEO_GRB + NEO_KHZ800);

#define BUTTON_PIN 4
#define VREF_PIN 5

unsigned long last_read_voltage;
unsigned long last_send_status;
uint32_t lastWifiCheck;
long mil;
long mesh_millis_offset;
long start_millis;

uint8_t voltage_read;
uint16_t voltage;
uint8_t charge;
uint16_t voltage_buffer[NUMBER_OF_READS];

WiFiUDP Udp;
IPAddress ipAnswer(224, 0, 0, 1);
unsigned int port = 6000;

// Function prototypes //
void pixelStripWrite(Adafruit_NeoPixel &strip, uint8_t brightness);

void setup() {
  if (debug) Serial.begin(115200);

  //analogReadResolution(12);

  MIDI();

  String host = "Solaris_" + String(NUM);
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin("SvetlitsaNet", "gggggggggg");
  WiFi.hostname(host);

  pinMode(INNER_PWM_PIN, OUTPUT);
  pinMode(MIDDLE_PWM_PIN, OUTPUT);
  pinMode(OUTER_PWM_PIN, OUTPUT);
  pinMode(HANDS_PWM_PIN, OUTPUT);

  if (digitalRead(BUTTON_PIN) == LOW) {  // button is pressed
                                         //    OTA();
  }

  byte a = 0;
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(10);
    if (a++ > 250) break;
  }

  Udp.begin(12321);

  ArduinoOTA.setHostname(host.c_str());
  ArduinoOTA.begin();

  int v = analogRead(VREF_PIN);
  for (byte i = 0; i < NUMBER_OF_READS; i++) {
    voltage_buffer[i] = v * VOLTAGE_INDEXER / 100;
  }

  inner.begin();
  middle.begin();
  outer.begin();
}

void loop() {
  mil = millis();

  ArduinoOTA.handle();

  if (millis() - lastWifiCheck > WIFI_CHECK_INTERVAL) {
    lastWifiCheck = millis();

    if (WiFi.status() != 3) {
      command = 15;
      start_millis = 0;
    }
  }

  if (millis() - last_read_voltage > VOLTAGE_CHECK_INTERVAL) {
    last_read_voltage = mil;
    read_voltage();
  }

  if (millis() - last_send_status > STATUS_SEND_INTERVAL) {
    last_send_status = mil;
    sendMessage();
  }

  // if (!digitalRead(4)) {
  //   if (millis() - lastbutton > pressdelay) {
  //     lastbutton = mil;
  //     if (command) {
  //       command = 12;
  //     } else {
  //       command = 15;
  //     }
  //   }
  // }

  parse_commands();
  MIDI();
}
