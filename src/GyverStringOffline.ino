/*
  Скетч к проекту "Бегущая строка"
  Страница проекта (схемы, описания): https://alexgyver.ru/GyverString/
  Исходники на GitHub: https://github.com/AlexGyver/GyverString/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver, AlexGyver Technologies, 2019
  https://AlexGyver.ru/
*/

// ================ НАСТРОЙКИ ================
#define MATR_NUM 4            // количество матриц последовательно

// ============ ДЛЯ РАЗРАБОТЧИКОВ ============
// ПИНЫ
#define BT_RX 3
#define BT_TX 2
#define TEXT_SPEED 100

// БИБЛИОТЕКИ
#include <SoftwareSerial.h>
#include <avr/eeprom.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <FastLED.h>
#include "fonts.h"
#include "timerMinim.h"

#define WIDTH MATR_NUM*8      // ширина матрицы
#define HEIGHT 8              // высота матрицы

Max72xxPanel matrix = Max72xxPanel(10, 4, 1);
SoftwareSerial btSerial(BT_TX, BT_RX); // RX, TX
timerMinim scrollTimer(100);

boolean loadingFlag, fullTextFlag, autoFlag = true, showText = true;
byte color;


// Common issues:
// - same ground!!!!!!!!
// count of project
#define COUNT_PROJECTS 1
// Count of letters in name of project
#define COUNT_LETTERS 4
// Number of leds
#define NUM_LEDS 5
// Count of leds (should be same as above - refactor this)
#define NUMBER_LIGHTS 5

#define DEBUG 0
#define INFO 1
#define PRINT_LEVEL DEBUG

#define WIDTH MATR_NUM*8      // ширина матрицы
#define HEIGHT 8              // высота матрицы

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 3
//#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS*NUMBER_LIGHTS];

char Incoming_value = 0;

void setup() {
  Serial.begin(9600); //Sets the data rate in bits per second (baud) for serial data transmission
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS*NUMBER_LIGHTS);
  btSerial.begin(9600);
  btSerial.setTimeout(100);

  matrix.setIntensity(50);
  for (byte i = 0; i < MATR_NUM; i++) {
    // матрицы расположены криво, здесь поворачиваем
    matrix.setRotation(i, 1);
  }
  matrix.fillScreen(LOW);
  matrix.write();

  scrollTimer.setInterval(TEXT_SPEED);
}

String runningText = "";

void loop() {
    int projectIndex = 0;


color = 0;
  if (Serial.available() > 0) {
//    Serial.print("Test!");
//    for all projects we need to read name (max length = COUNT_LETTERS) and state for leds (max length = COUNT_LEDS)
    while (projectIndex < COUNT_PROJECTS) {
      // Read project name (if project name less than COUNT_LETTERS use white spaces)
      int letterIndex = 0;
      String s;
      char sign = '\0';
      while (true) { // TODO remove COUNT_LETTERS
        sign = Serial.read();
        s += sign;
        delay(3);
        letterIndex++;
        if ('\0' == sign) {
          break; 
        }
      }
      runningText = s;
//      infoPrint("Project Name: \'");
//      Serial.print(projectName);
//      infoPrint("\'\n");
//      infoPrint(( {':', ' '}));

      // Read states of build (if count of builds less than count of leds use '0')

      int ledIndex = 0;
      while (ledIndex < NUMBER_LIGHTS) {
        Incoming_value = Serial.read();
        delay(3);
//        Serial.print(Incoming_value); //Print Value of Incoming_value in Serial monitor
//        Serial.print("\n");        //New line
//        Serial.print("Led: ");
//        Serial.print(char('0' + ledIndex + 1));
        for (int i = 0; i < 5; i++) {
        if (Incoming_value == '1') {
          leds[NUMBER_LIGHTS * projectIndex + ledIndex * NUM_LEDS + i] = CRGB::Green;
//          Serial.print(" - green" + (COUNT_LEDS * projectIndex + ledIndex * NUM_LEDS + i));
        } else if (Incoming_value == '2') {
          leds[NUMBER_LIGHTS * projectIndex + ledIndex * NUM_LEDS + i] = CRGB::Yellow;
//          Serial.print(" - yellow" + (COUNT_LEDS * projectIndex + ledIndex * NUM_LEDS + i));
        } else if (Incoming_value == '3') {
          leds[NUMBER_LIGHTS * projectIndex + ledIndex * NUM_LEDS + i] = CRGB::Red;
//          Serial.print(" - red" + (COUNT_LEDS * projectIndex + ledIndex * NUM_LEDS + i));
        } else {
          leds[NUMBER_LIGHTS * projectIndex + ledIndex * NUM_LEDS + i] = CRGB::Black;
//          Serial.print(" - black" + (COUNT_LEDS * projectIndex + ledIndex * NUM_LEDS + i));
        }
        }
//        infoPrint(&Incoming_value);
        Serial.print("\n");
        ledIndex++;
      }
      projectIndex++;
    }
//    Serial.print("----------------------------------\n");
    FastLED.show();
  }
fillString(runningText, color);
  
  
}
