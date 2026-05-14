#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "includes/crc.h"

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

void startup() {

  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(1);

  String mocklinuxlogs = "[    0.000000] Booting Linux kernel...\n"
"[    0.000000] Linux version 6.x.x\n"
"[    0.000000] CPU: Processor detected\n"
"[    0.000000] Memory: initializing system memory\n"
"[    0.003882] clocksource: timer initialized\n"
"[    0.007114] random: entropy pool initialized\n"
"[    0.012771] NET: Registered protocol family 16\n"
"[    0.019222] GPIO subsystem initialized\n"
"[    0.025117] SPI subsystem initialized\n"
"[    0.030994] I2C subsystem initialized\n"
"[    0.036183] UART subsystem initialized\n"
"[    0.041991] USB core initialized\n"
"[    0.048772] Storage device detected\n"
"[    0.056101] Mounting root filesystem...\n"
"[    0.064882] VFS: Mounted root filesystem";

  String tux = "            a8888b.\n"
"           d888888b.\n"
"           8P\"YP\"Y88\n"
"           8|o||o|88\n"
"           8'    .88\n"
"           8`._.' Y8.\n"
"          d/      `8b.\n"
"         dP   .    Y8b.\n"
"        d8:'  \"  `::88b\n"
"       d8\"         'Y88b\n"
"      :8P    '      :888\n"
"       8a.   :     _a88P\n"
"     ._\/\"Yaa_:   .| 88P|\n"
"jgs  \\    YP\"    `| 8P  `.\n"
"a:f  /     \\.___.d|    .'\n"
"     `--..__)8888P`._.' ";

  tft.println(mocklinuxlogs);

  delay(3000);

  tft.fillScreen(ST77XX_BLACK);

  cursorY = 0;

  tft.println(tux);

  delay(3000);

  tft.fillScreen(ST77XX_BLACK);

  cursorY = 0;
} 

void setup() {

  Serial.begin(115200);
  Serial1.begin(921600);

  tft.init(240, 320);
  tft.setRotation(1);

  tft.fillScreen(ST77XX_BLACK);

  startup();
}


void loop() {
  readPort(Serial1);
  readPort(Serial);
}