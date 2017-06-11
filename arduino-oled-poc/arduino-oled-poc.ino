#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);

double minValue = -1.0;
double maxValue = -1.0;

void setup() {
  Serial.begin(9600);

  Serial.println("Width...: " + String(display.width()));
  Serial.println("Height..: " + String(display.height()));

  randomSeed(display.width());

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // 0x3C = I2C Address

  clear();
  update();

  display.setTextSize(2);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.println("BarGraph");
  update();
}

void loop() {
  double v1 = format(random(0, 99) + (random(0, 99) / 100.0), 2);

  if (minValue == -1.0 || maxValue == -1.0) {
    minValue = maxValue = v1;
  } else if (v1 < minValue) {
    minValue = v1;
  } else if (v1 > maxValue) {
    maxValue = v1;
  }

  logData(v1);

  clearLine(16);
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 24);
  display.println("v1: " + String(v1));
  update();

  clearLine(34, 8);
  display.fillRect(0, 34, normalize(v1, 128), 8, WHITE);

  for (int i = 1; i < 13; i++) {
    display.fillRect(i * 10, 34, 2, 8, BLACK);
  }

  clearLine(48);
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 48);
  display.println("min: " + String(minValue));
  display.setCursor(64, 48);
  display.println("max: " + String(maxValue));
  update();

  update();

  delay(1000);
}

void logData(double v1) {
  Serial.println("##########");
  Serial.println("Random value...: " + String(v1));
  Serial.println("Min value......: " + String(minValue));
  Serial.println("Max value......: " + String(maxValue));
  Serial.println("##########");
}

double normalize(double value, double max) {
  return (max * value) / 100;
}

double format(double value, int decimals) {
  return round(value * pow(10, decimals)) / pow(10, decimals);
}

void clear() {
  display.clearDisplay();
}

void clearLine(int startRow) {
  clearLine(startRow, 16);
}

void clearLine(int startRow, int rows) {
  display.fillRect(0, startRow, 128, rows, BLACK);
}

void update() {
  display.display();
}

