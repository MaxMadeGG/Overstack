// Overstack firmware (first version)
// XIAO ESP32S3 + MPU6050 + Waveshare 0.85" GC9107 128x128 + button + vibration motor
//
// Libraries (Arduino Library Manager):
//   - GFX Library for Arduino (Arduino_GFX by moononournation)
//   - ArduinoJson
//
// Controls:
//   tilt left/right   -> scroll letters
//   tilt toward you   -> take the word suggestion
//   short press       -> add letter
//   long press        -> send message to the AI

#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino_GFX_Library.h>
#include "secrets.h"   // copy secrets_example.h to secrets.h and fill it in

// ---------- pins (from the main board schematic) ----------
#define PIN_VBAT   D0   // battery divider (10k/10k)
#define PIN_BTN    D1   // button to GND
#define PIN_TFT_RST D2  // HDR_GPIO1 (or tie RST to 3V3)
#define PIN_TFT_DC D3   // LED_EN pad
#define PIN_TFT_SCK D8  // HDR_GPIO3
#define PIN_MOTOR  D9   // BC337 base via 1k
#define PIN_TFT_MOSI D10 // LED_DATA pad

// ---------- display ----------
Arduino_DataBus *bus = new Arduino_ESP32SPI(PIN_TFT_DC, GFX_NOT_DEFINED, PIN_TFT_SCK, PIN_TFT_MOSI, GFX_NOT_DEFINED);
Arduino_GFX *gfx = new Arduino_GC9107(bus, PIN_TFT_RST, 0 /*rotation*/, true /*IPS*/);

#define C_BG    0x0000
#define C_CYAN  0x47FF
#define C_PINK  0xFB56
#define C_WHITE 0xFFFF
#define C_GREY  0x4208

// ---------- MPU6050 (raw registers, no library) ----------
const uint8_t MPU = 0x68;
void mpuInit() {
  Wire.beginTransmission(MPU); Wire.write(0x6B); Wire.write(0); Wire.endTransmission(); // wake up
}
void mpuRead(float &ax, float &ay) {
  Wire.beginTransmission(MPU); Wire.write(0x3B); Wire.endTransmission(false);
  Wire.requestFrom(MPU, (uint8_t)4);
  int16_t x = (Wire.read() << 8) | Wire.read();
  int16_t y = (Wire.read() << 8) | Wire.read();
  ax = x / 16384.0; ay = y / 16384.0;   // in g
}

// ---------- haptics ----------
void buzz(int ms) { digitalWrite(PIN_MOTOR, HIGH); delay(ms); digitalWrite(PIN_MOTOR, LOW); }

// ---------- battery ----------
float batteryVolts() { return analogReadMilliVolts(PIN_VBAT) * 2 / 1000.0; }

// ---------- letter picker ----------
const char LETTERS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ _<";  // '_' = space, '<' = delete
const int  NLET = sizeof(LETTERS) - 1;
int    letterIdx = 0;
String message = "";

const char *WORDS[] = {"HELLO","HOW","ARE","YOU","WHAT","WHY","WHO","WHERE","WHEN","THE",
                       "IS","TELL","ME","JOKE","WEATHER","TODAY","THANKS","GOOD","NIGHT","MORNING"};
const int NWORDS = sizeof(WORDS) / sizeof(WORDS[0]);

String currentWord() {
  int sp = message.lastIndexOf(' ');
  return message.substring(sp + 1);
}
String suggestion() {
  String w = currentWord();
  if (w.length() == 0) return "";
  for (int i = 0; i < NWORDS; i++)
    if (String(WORDS[i]).startsWith(w) && String(WORDS[i]) != w) return WORDS[i];
  return "";
}

// ---------- drawing ----------
void drawFace() {
  gfx->fillScreen(C_BG);
  int p = 8;                                    // chunky pixel size
  gfx->fillRect(40, 32, 2*p, 3*p, C_CYAN);      // eyes
  gfx->fillRect(72, 32, 2*p, 3*p, C_CYAN);
  gfx->fillRect(24, 72, p, p, C_PINK);          // cheeks
  gfx->fillRect(96, 72, p, p, C_PINK);
  gfx->fillRect(32, 80, p, p, C_CYAN);          // smile
  gfx->fillRect(88, 80, p, p, C_CYAN);
  gfx->fillRect(40, 88, 6*p, p, C_CYAN);
}

void drawPicker() {
  gfx->fillScreen(C_BG);
  // neighbours
  gfx->setTextColor(C_GREY); gfx->setTextSize(2);
  gfx->setCursor(10, 40);  gfx->print(LETTERS[(letterIdx + NLET - 1) % NLET]);
  gfx->setCursor(106, 40); gfx->print(LETTERS[(letterIdx + 1) % NLET]);
  // current letter, big
  gfx->setTextColor(C_CYAN); gfx->setTextSize(5);
  gfx->setCursor(50, 26); gfx->print(LETTERS[letterIdx]);
  // suggestion
  String s = suggestion();
  gfx->setTextSize(1);
  if (s.length()) { gfx->setTextColor(C_PINK); gfx->setCursor(4, 82); gfx->print("> " + s); }
  // typed text (last 21 chars)
  gfx->setTextColor(C_WHITE); gfx->setCursor(4, 100);
  gfx->print(message.length() > 21 ? message.substring(message.length() - 21) : message);
  gfx->print('_');
  // battery
  gfx->setTextColor(C_GREY); gfx->setCursor(92, 4); gfx->printf("%.1fV", batteryVolts());
}

void showText(const String &t) {
  gfx->fillScreen(C_BG);
  gfx->setTextColor(C_WHITE); gfx->setTextSize(1);
  gfx->setCursor(0, 4);
  gfx->setTextWrap(true);
  gfx->print(t);
}

// ---------- AI ----------
String askAI(const String &question) {
  if (WiFi.status() != WL_CONNECTED) return "No WiFi :(";
  WiFiClientSecure client; client.setInsecure();     // simple first version
  HTTPClient http;
  http.begin(client, "https://api.anthropic.com/v1/messages");
  http.addHeader("content-type", "application/json");
  http.addHeader("x-api-key", AI_API_KEY);
  http.addHeader("anthropic-version", "2023-06-01");

  JsonDocument req;
  req["model"] = AI_MODEL;
  req["max_tokens"] = 120;
  req["system"] = "You live inside a tiny 30mm pixel buddy with a 128x128 screen. Answer in 1-2 short sentences, plain text.";
  req["messages"][0]["role"] = "user";
  req["messages"][0]["content"] = question;
  String body; serializeJson(req, body);

  int code = http.POST(body);
  String reply = "Error " + String(code);
  if (code == 200) {
    JsonDocument res;
    if (!deserializeJson(res, http.getString())) reply = res["content"][0]["text"].as<String>();
  }
  http.end();
  return reply;
}

// ---------- main ----------
unsigned long lastMove = 0, pressStart = 0;
bool pressed = false, idle = true;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BTN, INPUT_PULLUP);
  pinMode(PIN_MOTOR, OUTPUT);
  Wire.begin();            // SDA = D4, SCL = D5
  mpuInit();
  gfx->begin();
  drawFace();
  buzz(80);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
  float ax, ay; mpuRead(ax, ay);

  // tilt left/right scrolls letters (faster when tilted more)
  int interval = abs(ax) > 0.6 ? 120 : 300;
  if (abs(ax) > 0.3 && millis() - lastMove > interval) {
    if (idle) { idle = false; }
    letterIdx = (letterIdx + (ax > 0 ? 1 : NLET - 1)) % NLET;
    lastMove = millis();
    buzz(8);
    drawPicker();
  }

  // tilt toward you takes the suggestion
  if (!idle && ay > 0.55 && millis() - lastMove > 600) {
    String s = suggestion();
    if (s.length()) {
      message = message.substring(0, message.length() - currentWord().length()) + s + " ";
      buzz(30); drawPicker();
    }
    lastMove = millis();
  }

  // button: short = add letter, long = send
  bool down = digitalRead(PIN_BTN) == LOW;
  if (down && !pressed) { pressed = true; pressStart = millis(); }
  if (!down && pressed) {
    pressed = false;
    unsigned long held = millis() - pressStart;
    if (held > 700) {                       // long press: send
      if (message.length()) {
        buzz(60); delay(80); buzz(60);
        showText("Thinking...");
        String reply = askAI(message);
        showText(reply);
        buzz(150);
        message = "";
      }
    } else if (held > 30) {                 // short press: add letter
      char c = LETTERS[letterIdx];
      if (c == '<') { if (message.length()) message.remove(message.length() - 1); }
      else message += (c == '_') ? ' ' : c;
      idle = false;
      buzz(20);
      drawPicker();
    }
  }
  delay(10);
}
