#include <Adafruit_NeoPixel.h>

#define LED_COUNT 6

#define LED1_PIN 6
#define LED2_PIN 2
#define LED3_PIN 8
#define LED4_PIN 9

#define BUTTON1_PIN 7
#define BUTTON2_PIN 5
#define BUTTON3_PIN 4
#define BUTTON4_PIN 3

Adafruit_NeoPixel strip1(LED_COUNT, LED1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT, LED2_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3(LED_COUNT, LED3_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4(LED_COUNT, LED4_PIN, NEO_GRB + NEO_KHZ800);

bool beats1[LED_COUNT] = {1,1,1,1,1,1};
bool beats2[LED_COUNT] = {1,1,1,1,1,1};
bool beats3[LED_COUNT] = {1,1,1,1,1,1};
bool beats4[LED_COUNT] = {1,1,1,1,1,1};

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
  pinMode(BUTTON4_PIN, INPUT_PULLUP);

  strip1.begin(); strip1.show();
  strip2.begin(); strip2.show();
  strip3.begin(); strip3.show();
  strip4.begin(); strip4.show();
}

void handleButtons() {
  if (digitalRead(BUTTON1_PIN) == LOW) { Serial.println("BTN1"); delay(200); }
  if (digitalRead(BUTTON2_PIN) == LOW) { Serial.println("BTN2"); delay(200); }
  if (digitalRead(BUTTON3_PIN) == LOW) { Serial.println("BTN3"); delay(200); }
  if (digitalRead(BUTTON4_PIN) == LOW) { Serial.println("BTN4"); delay(200); }
}

void updateLEDs(Adafruit_NeoPixel &strip, bool beats[], int currentBeat) {
  for (int i = 0; i < LED_COUNT; i++) {
    bool isActive = beats[i];
    bool isCurrent = (i == currentBeat);

    uint8_t r, g, b;

    if (isActive) {
        // PLAY = baby blue
        r = isCurrent ? 143 : 40;
        g = isCurrent ? 211 : 40;
        b = isCurrent ? 255 : 40;
    } else {
        // SILENCE = dark red (not purple)
        r = isCurrent ? 128 : 40;   // deep red
        g = isCurrent ? 0   : 0;    // no green
        b = isCurrent ? 0   : 0;    // no blue
    }


    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void processSerial() {
  static String incoming = "";

  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      incoming.trim();

      if (incoming.startsWith("BEATS1:")) {
        String p = incoming.substring(7);
        for (int i = 0; i < LED_COUNT; i++) beats1[i] = (p.charAt(i) == '1');
      }
      else if (incoming.startsWith("BEATS2:")) {
        String p = incoming.substring(7);
        for (int i = 0; i < LED_COUNT; i++) beats2[i] = (p.charAt(i) == '1');
      }
      else if (incoming.startsWith("BEATS3:")) {
        String p = incoming.substring(7);
        for (int i = 0; i < LED_COUNT; i++) beats3[i] = (p.charAt(i) == '1');
      }
      else if (incoming.startsWith("BEATS4:")) {
        String p = incoming.substring(7);
        for (int i = 0; i < LED_COUNT; i++) beats4[i] = (p.charAt(i) == '1');
      }
      else if (incoming.startsWith("CUR:")) {
        int b = incoming.substring(4).toInt();
        updateLEDs(strip1, beats1, b);
        updateLEDs(strip2, beats2, b);
        updateLEDs(strip3, beats3, b);
        updateLEDs(strip4, beats4, b);
      }

      incoming = "";
    } else {
      incoming += c;
    }
  }
}

void loop() {
  handleButtons();
  processSerial();
}
