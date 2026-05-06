#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define TFT_CS   17
#define TFT_DC   21
#define TFT_RST  20

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

String lineBuffer = "";
String textBuffer = "";

bool receivingText = false;

int cursorY = 0;

const int CHAR_W = 6;
const int CHAR_H = 8;
const int SCREEN_W = 320;
const int SCREEN_H = 240;


void nextLine() {

  cursorY += CHAR_H;

  if (cursorY >= SCREEN_H - CHAR_H) {
    tft.fillScreen(ST77XX_BLACK);
    cursorY = 0;
  }
}


void printWrapped(String msg) {

  int x = 0;

  for (int i = 0; i < msg.length(); i++) {

    char c = msg[i];

    if (c == '\n') {
      x = 0;
      nextLine();
      continue;
    }

    if (x >= SCREEN_W - CHAR_W) {
      x = 0;
      nextLine();
    }

    tft.setCursor(x, cursorY);
    tft.write(c);

    x += CHAR_W;
  }

  nextLine();
}


void processLine(String line) {

  line.trim();

  if (line == "TEXT_BEGIN") {
    textBuffer = "";
    receivingText = true;
    return;
  }

  if (line == "TEXT_END") {

    receivingText = false;

    tft.fillScreen(ST77XX_BLACK);
    cursorY = 0;

    tft.setTextColor(ST77XX_GREEN);
    tft.setTextSize(1);

    printWrapped(textBuffer);

    return;
  }

  if (line == "CLEAR") {
    tft.fillScreen(ST77XX_BLACK);
    cursorY = 0;
    return;
  }

  if (receivingText) {
    textBuffer += line;
    textBuffer += '\n';
  }
}


void readPort(Stream &port) {

  while (port.available()) {

    char c = port.read();

    if (c == '\r')
      continue;

    if (c == '\n') {

      processLine(lineBuffer);

      lineBuffer = "";
    }
    else {
      lineBuffer += c;
    }
  }
}


void setup() {

  Serial.begin(115200);
  Serial1.begin(921600);

  tft.init(240, 320);
  tft.setRotation(1);

  tft.fillScreen(ST77XX_BLACK);
}


void loop() {

  readPort(Serial1);
  readPort(Serial);
}