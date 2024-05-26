#include "pitches.h"
#include <Wire.h>
#include <SPI.h>
#include <DS3231.h>
#include <Adafruit_GFX.h>  // Include core graphics library
#include <Adafruit_ST7735.h>  // Include Adafruit_ST7735 library to drive the display

// pin definition for Arduino UNO
#define cs   10
#define dc   8
#define rst  9
#define speaker 7

#define MAX_MELODIES 4
#define NAME_LENGTH 12

const int buttonPin1 = 2; // Pin for Button 1
const int buttonPin2 = 3; // Pin for Button 2
const int buttonPin3 = A0; // Pin for Button 3
const int buttonPin4 = A1; // Pin for Button 4
const int speakerPin = 7;
const int ledPin = 5;

// Variables to store the button states
volatile bool button1Pressed = false;
volatile bool button2Pressed = false;
volatile bool button3Pressed = false;
volatile bool button4Pressed = false;
// clock state
int state = 0;
int alarmIdx = 1;
bool startCronometer = false;
bool startTimer = false;
bool play_melody = false;
bool alarmMelody1Start = false;
bool alarmMelody2Start = false;
int melodyIdx = 0;

int ledVar = 0;

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

const int DS1307 = 0x68;
DS3231 myRTC;

int analogLOW = 50; // 5% from 1023, HIGH 

byte second = 0;
byte minute = 0;
byte hourofday = 0;
byte dayofweek = 0;
byte date = 0;
byte month = 0;
byte year = 0;

unsigned long milliseconds = 0;
unsigned long milliseconds_new = 0;
unsigned long second_new = 0;
unsigned long minute_new = 0;
byte hourofday_new = 0;
byte date_new = 0;

byte second_witness = 88;

const char* months[] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

int melody1[] = {
  REST, NOTE_D4, NOTE_G4, NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_D5, NOTE_C5, NOTE_A4,
  NOTE_G4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_GS4, NOTE_D4, NOTE_D4, NOTE_G4, NOTE_AS4, NOTE_A4,
  NOTE_G4, NOTE_D5, NOTE_F5, NOTE_E5, NOTE_DS5, NOTE_B4, NOTE_DS5, NOTE_D5, NOTE_CS5,
  NOTE_CS4, NOTE_B4, NOTE_G4, NOTE_AS4
};

int noteDurations1[] = {
  2, 4, 4, 8, 4, 2, 4, 2, 2, 4, 8, 4, 2, 4, 1, 4, 4, 8, 4, 2, 4, 2, 4, 2, 4, 4, 8, 4, 2, 4, 1, 4
};

int melody2[] = {
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_G4
};

int noteDurations2[] = {
  8, 8, 4, 8, 8, 4, 8, 8, 8, 8, 2, 8, 8, 8, 8, 4, 8, 8, 8, 8, 4, 8, 8, 8, 2
};

void checkStopAlarm() {
  if (alarmMelody1Start) {
      alarmMelody1Start = false;
      clearScreen();
    }
    if (alarmMelody2Start) {
      alarmMelody2Start = false;
      clearScreen();
    }
}

ISR(INT0_vect) {
  int currentButtonState1 = digitalRead(buttonPin1);
  if (currentButtonState1 == HIGH) {
    button1Pressed = true;
    play_melody = false;
    checkStopAlarm();
  } else {
    button1Pressed = false;
  }
}


ISR(INT1_vect) {
  int currentButtonState2 = digitalRead(buttonPin2);
  if (currentButtonState2 == HIGH) {
    button2Pressed = true;
    play_melody = false;
    checkStopAlarm();
  } else {
    button2Pressed = false;
  }
}

ISR(PCINT1_vect) {
  int currentButtonState3 = analogRead(buttonPin3);
  if (currentButtonState3 > analogLOW) {
    button3Pressed = true;
    checkStopAlarm();
    play_melody = false;
  } else {
    button3Pressed = false;
  }

  int currentButtonState4 = analogRead(buttonPin4);
  if (currentButtonState4 > analogLOW) {
    button4Pressed = true;
    if (ledVar == 0) {
      digitalWrite(ledPin, HIGH);
      ledVar = 1; 
    } else {
      digitalWrite(ledPin, LOW);
      ledVar = 0; 
    }
  } else {
    button4Pressed = false;
  }
}



void setup() {
  // 1. RTC
  Wire.begin();

  // 2. Serial (pt test...)
  Serial.begin(9600);//9600,8,Even,1
  Serial.setTimeout(800);

  // // Use this initializer if you're using a 1.8" TFT
  tft.initR(INITR_BLACKTAB);  // Initialize a ST7735S chip, black tab

  tft.fillScreen(ST7735_BLACK);  // Fill screen with black

  tft.setTextWrap(false); 
  
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(ledPin, OUTPUT);

  // Enable global interrupts
  cli();

  // activate interrupt for button 1
  EIMSK |= (1 << INT0);
  EICRA |= (1 << ISC00);

  // activate interrupt for button 2
  EIMSK |= (1 << INT1);    
  EICRA |= (1 << ISC10);

  // activate interrupt for button 3 (A0)
  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT8);

  // activate interrupt for button 4 (A1)
  PCMSK1 |= (1 << PCINT9);

  
  sei();  
}

void loop() {
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  writeMenu();
  checkAlarms();

  if (state == 0) {
    showTimeState();
  }
  if (state == 1) {
    showQuestionSetTimeState();
  }
  if (state >= 2 && state < 8) {
    showSetTimeAll();
  }
  if (state == 10) {
    showSetAlarmState();
  }
  if (state >= 11 && state < 15) {
    showSetAlarmAll();
  }
  if (state == 15) {
    showSetAlarmMelody();
  }
  if (state == 18) {
    viewAlarms();
  }
  if (state >= 20 && state < 23) {
    showSetTimerAll();
    //Serial.println(state);
  }
  if (state == 23) {
    showStartTimer();
  }
  if (state == 30) {
    showCronometer();
  }
}

