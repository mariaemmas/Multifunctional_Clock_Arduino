// states
// 0 - initial state, display date and time
void showTimeState() {
  sendTime();

  if (button1Pressed) {  // go to QuestionSetTimeState
    button1Pressed = false;
    state++;
    clearScreen();
  }
  if (button2Pressed) {   // go to CRONOMETER
    button2Pressed = false;
    milliseconds_new = 0;
    second_new = 0;
    minute_new = 0;
    hourofday_new = 0;
    state = 30;
    clearScreen();
  }
  if (button3Pressed) {   // go to TIMER
    button3Pressed = false;
    second_new = 0;
    minute_new = 0;
    hourofday_new = 0;
    state = 20;
    clearScreen();
  }
}

// 1 - ask about clock setting
void showQuestionSetTimeState() {
  tft.setCursor(15, 80);  // Set position (x,y)
  tft.setTextSize(2); 
  tft.println("Set time?");

  if (button1Pressed) {  // go to showSetTimeYear
    button1Pressed = false;
    state = 2;
    readTime();
    clearScreen();
  }
  if (button2Pressed) {   // go to showTimeState
    button2Pressed = false;
    state = 0;
    clearScreen();
  }
  if (button3Pressed) {   // go to showQuestionSetAlarmState
    button3Pressed = false;
    state = 10;
    clearScreen();
  }
}

byte ldate() {
  byte last_date = 31;
  // find last day of month
  if (month == 4 || month == 6 ||month == 9 || month == 11) {
      last_date = 30;
    }
    if (month == 2) {
      if (year % 4 == 0) {
        last_date = 29;
      } else {
        last_date = 28;
      }
    }
  return last_date;
}

// 2-7 states
void showSetTimeAll() {
  char dateString[15];  // Enough to hold "6 Apr 2024" + null terminator
  byte last_date = ldate();

  sprintf(dateString, "%d %s %02d", 2000 + year, months[month - 1], date);
  // underline value to change
  if (state == 2) {
    tft.fillRect(25, 60, 22, 2, ST7735_YELLOW);
  }
  if (state == 3) {
    tft.fillRect(55, 60, 18, 2, ST7735_YELLOW);
  }
  if (state == 4) {
    tft.fillRect(78, 60, 15, 2, ST7735_YELLOW);
  }
  if (state == 5) {
    tft.fillRect(15, 100, 18, 2, ST7735_YELLOW);
  }
  if (state == 6) {
    tft.fillRect(55, 100, 18, 2, ST7735_YELLOW);
  }
  if (state == 7) {
    tft.fillRect(90, 100, 18, 2, ST7735_YELLOW);
  }

  tft.setCursor(25, 50);  // Set position (x,y)
  tft.setTextSize(1); 
  tft.println(dateString);  
  printTimeToScreen(hourofday, minute, second);

  if (button1Pressed) {  
    button1Pressed = false;
    if (state == 2) {
      year++;
    }
    if (state == 3) {
      month++;
      if(month > 12) {
        month = 1;
      }
    }
    if (state == 4) {
      date++;   
      if(date > last_date) {
        date = 1;
      }
    }
    if (state == 5) {
      hourofday++;
      if(hourofday > 23) {
        hourofday = 0;
      }
    }
    if (state == 6) {
      minute++;
      if(minute > 59) {
        minute = 0;
      }
    }
    if (state == 7) {
      second++;
      if(second > 59) {
        second = 1;
      }
    }
  }
  if (button2Pressed) {
    button2Pressed = false;
    if (state == 2) {
      year--;
    }
    if (state == 3) {
      month--;
      if(month == 0) {
        month = 12;
      }
    }
    if (state == 4) {
      date--;
      if(date == 0) {
        date = last_date;
      }
    }
    if (state == 5) {
      if(hourofday == 0) {
        hourofday = 23;
      } else {
        hourofday--;
      }
    }
    if (state == 6) {
      if(minute == 0) {
        minute = 59;
      } else {
        minute--;
      }
    }
    if (state == 7) {
      if(second == 0) {
        second = 59;
      } else {
        second--;
      }
    }
  }
  if (button3Pressed) { 
    button3Pressed = false;
    if (state >= 2 && state < 7) {
      state++;
    }
      if (state == 7) {
        setTime();
        state = 0;
      }    
    clearScreen();
  }
}

// 10 - state : ask about setting alarm
void showSetAlarmState() {
  tft.setCursor(10, 80);  // Set position (x,y)
  tft.setTextSize(2);  
  tft.println("Set alarm?"); 

  if (button1Pressed) {  // go to showSetAlarmAll
    button1Pressed = false;
    state = 11;
    readTime();
    clearScreen();
  }
  if (button2Pressed) {   // go to showTimeState
    button2Pressed = false;
    state = 0;
    clearScreen();
  }
  if (button3Pressed) {   // go to viewAlarms
    button3Pressed = false;
    state = 18;
    clearScreen();
  }
}

void showSetAlarmAll() {
  char dateString[15];  
  byte last_date = ldate();

  printAlarmHeaderToScreen();

  if (state == 11) {
    tft.fillRect(72, 30, 5, 2, ST7735_YELLOW);
    date_new = date;
    hourofday_new = hourofday;
    minute_new = minute;
  }
  if (state == 12) {  
    tft.fillRect(78, 60, 15, 2, ST7735_YELLOW);
  }
  if (state == 13) {  
    tft.fillRect(15, 100, 18, 2, ST7735_YELLOW);
  }
  if (state == 14) {  
    tft.fillRect(55, 100, 18, 2, ST7735_YELLOW);
  }
  sprintf(dateString, "%d %s %02d", 2000 + year, months[month - 1], date_new);

  tft.setCursor(25, 50);  // Set position (x,y)
  tft.println(dateString);  
  printTimeToScreen(hourofday_new, minute_new, 0);

  if (button1Pressed) {  
    button1Pressed = false;
    if (state == 11) {
      alarmIdx++;
      if(alarmIdx > 2) {
        alarmIdx = 1;
      }
    }
    if (state == 12) {  
      date_new++;    
      if(date_new > last_date) {
        date_new = 1;
      }
    }
    if (state == 13) {  
      hourofday_new++;
      if(hourofday_new > 23) {
        hourofday_new = 0;
      }
    }
    if (state == 14) {  
      minute_new++;
      if(minute_new > 59) {
        minute_new = 0;
      }
    }
  }
  if (button2Pressed) {   
    button2Pressed = false;
    if (state == 11) {
      alarmIdx--;
      if(alarmIdx == 0) {
        alarmIdx = 2;
      }
    }
    if (state == 12) {  
      date_new--;
      if(date_new == 0) {
        date_new = last_date;
      }
    }
    if (state == 13) {  
      if(hourofday_new == 0) {
        hourofday_new = 23;
      } else {
        hourofday_new--;
      }
    }
    if (state == 14) {  
      if(minute_new == 0) {
        minute_new = 59;
      } else {
        minute_new--;
      }
    }
  }
  if (button3Pressed) {   
    button3Pressed = false;
    if (state >= 11 && state < 15) {
      state++;
    }
    clearScreen();
  }
}

void printAlarmHeaderToScreen() {
  char dateString[15];  

  tft.setCursor(38, 20);  // Set position (x,y)
  tft.setTextSize(1);  
  sprintf(dateString, "Alarm %d", alarmIdx);
  tft.println(dateString); 
}

// 15 - state : set alarm melody
void showSetAlarmMelody() {
  char dateString[15];  
  printAlarmHeaderToScreen();

  tft.setCursor(38, 50);  
  tft.println("Melodie");

  tft.setCursor(20, 80); 

  // show alarm no and play it
  if (melodyIdx == 0) {
    tft.println("Melody 1");
    play_melody = true;
    playMelody();
  }
  if (melodyIdx == 1) {
    tft.println("Melody 2");
    play_melody = true;
    playMelody();
  } 

  if (button1Pressed) {  
    button1Pressed = false;
    melodyIdx++;
    if(melodyIdx >= 2) {
      melodyIdx = 0;
    }
  }
  if (button2Pressed) {
    button2Pressed = false;
    if(melodyIdx == 0) {
      melodyIdx = 1;
    } else {
      melodyIdx--;
    }
  }
  if (button3Pressed) {   // go to showSetAlarmState
    button3Pressed = false;
    if (alarmIdx == 1) {
        myRTC.setA1Time(date_new, hourofday_new, minute_new, 0, 0, false, false, false);
        myRTC.turnOnAlarm(1);
      } else {
      if (alarmIdx == 2) {
        myRTC.setA2Time(date_new, hourofday_new, minute_new, 0, false, false, false);
        myRTC.turnOnAlarm(2);
      }
    }
    minute_new = 0;
    hourofday_new = 0;
    date_new = 0;
    state = 10;
    clearScreen();
  }

}

// 18 - view alarms
void viewAlarms() {
  char dateString[15]; 
  printAlarmHeaderToScreen();

  if (!myRTC.checkAlarmEnabled(alarmIdx)) {
    tft.setCursor(20, 80);  
    tft.setTextSize(2);  
    tft.println("Not Set");  
  } else {
    // variables used to read alarm from RTC
    byte min_new = 0;
    byte sec_new = 0;
    byte alarmBits = 0;
    bool A1Dy = false, A1h12 = false, A1PM = false;

    if (alarmIdx == 1) {
      // read alarm 1
      myRTC.getA1Time(date_new, hourofday_new, min_new, sec_new, alarmBits,A1Dy, A1h12, A1PM);
    } else {
      if (alarmIdx == 2) {
        // read alarm 2
        myRTC.getA2Time(date_new, hourofday_new, min_new, alarmBits, A1Dy, A1h12, A1PM);
      } 
    }
    // display read alarm
    printTimeToScreen(hourofday_new, min_new, 0);
    printDateToScreen(date_new, month, year);
  }

  if (button1Pressed) { // delete alarm
    button1Pressed = false;
    myRTC.turnOffAlarm(alarmIdx);
    clearScreen();
    tft.setCursor(20, 80);
    tft.setTextSize(1);
    tft.println("Alarm deleted!");
    delay(1000);
    clearScreen();
    state = 18;
  }
  if (button2Pressed) { // Out
    button2Pressed = false;
    state = 0;
    second_witness = 99;
    clearScreen();
    sendTime();

  }
  if (button3Pressed) {   // Next alarm
    button3Pressed = false;
    clearScreen();
    alarmIdx++;
      if(alarmIdx > 2) {
        alarmIdx = 1;
      }
  }
}


void showTimer() {
  tft.setCursor(38, 20); 
  tft.setTextSize(1);  
  tft.println("Timer");  
}

// 20 - 22 states : set timer
void showSetTimerAll() {  
  showTimer();
  if (state == 20) {
    tft.fillRect(15, 100, 18, 2, ST7735_YELLOW);
  }
  if (state == 21) {
    tft.fillRect(55, 100, 18, 2, ST7735_YELLOW);
  }
  if (state == 22) {
    tft.fillRect(90, 100, 18, 2, ST7735_YELLOW);
  }
  printTimeToScreen(hourofday_new, minute_new, second_new);

  if (button1Pressed) {  
    button1Pressed = false;
    if (state == 20) {
      hourofday_new++;
      if(hourofday_new > 23) {
        hourofday_new = 0;
      }
    }
    if (state == 21) {
      minute_new++;
      if(minute_new > 59) {
        minute_new = 0;
      }
    }
    if (state == 22) {
      second_new++;
      if(second_new > 59) {
        second_new = 1;
      }
    }
  }
  if (button2Pressed) {
    button2Pressed = false;
    if (state == 20) {
      if(hourofday_new == 0) {
        hourofday_new = 23;
      } else {
        hourofday_new--;
      }
    }
    if (state == 21) {
      if(minute_new == 0) {
        minute_new = 59;
      } else {
        minute_new--;
      }
    }
    if (state == 22) {
      if(second_new == 0) {
        second_new = 59;
      } else {
        second_new--;
      }
    }
  }
  if (button3Pressed) { 
    button3Pressed = false;
    if (state >= 20 && state < 23) {
      state++;
    } 
    clearScreen();
  }
}

// 23 - state start timer
void showStartTimer() {
  showTimer();
  int seconds_to_kill = 0;

  if (startTimer) {
    // how many seconds since Start button was pressed?, eg. 7 sec
    seconds_to_kill = (millis() - milliseconds)/1000;

    // how many seconds remain? eg 126 - 7 = 119 sec
    seconds_to_kill = milliseconds_new - seconds_to_kill;

    // transform remaining time (119 sec) in seconds, minutes, hours
    second_new = seconds_to_kill % 60;
    minute_new = seconds_to_kill / 60;
    
    hourofday_new = minute_new / 60;
    minute_new = minute_new % 60;
    minute_new = minute_new % 60;
  }

  printTimeToScreen(hourofday_new, minute_new, second_new);
  if ((minute_new <= 0 && second_new <= 0 && startTimer) || play_melody) {
      // Timer to stop, start melody
      startTimer = false;
      play_melody = true;

      playMelody(); 
    }

  if (button1Pressed) {  
    button1Pressed = false;
    startTimer = true;
    play_melody = false;
    // store milisecund when Start button was pressed
    milliseconds = millis();
    // reused variable, store how many seconds i programmed; eg for 00:02:06 store in milliseconds_new 126
    milliseconds_new = hourofday_new * 3600 + minute_new * 60 + second_new; 
  }
  if (button2Pressed) {
    button2Pressed = false;
    state = 0;
    startTimer = false;
    second_witness = 99;
    clearScreen();
    sendTime();
  }
  if (button3Pressed) {
    button3Pressed = false;
    startTimer = false;
    play_melody = false;
  }
}

void playMelody() {
    for (int thisNote = 0; thisNote < 16; thisNote++) {
    if (play_melody || alarmMelody1Start || alarmMelody2Start) {
      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 0;
      if (melodyIdx == 0) {
        noteDuration = 1000 / noteDurations1[thisNote];
        tone(speakerPin, melody1[thisNote], noteDuration);
      }
      if (melodyIdx == 1) {
        noteDuration = 1000 / noteDurations2[thisNote];
        tone(speakerPin, melody2[thisNote], noteDuration);
      }
      
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(speakerPin);
    }
  }
}

// 30 - set cronometer
void showCronometer() {
  tft.setCursor(30, 20);
  tft.setTextSize(1);  
  tft.println("Cronometer"); 

  char dateString[15];  

  if (startCronometer) {
    milliseconds_new = millis();
    second_new = (milliseconds_new - milliseconds)/1000;
    milliseconds_new = (milliseconds_new - milliseconds) % 1000;
    
    minute_new = second_new/60;
    second_new = second_new % 60;
    if (second_new > 59) {
      second_new = 0;
    }
    minute_new = minute_new % 60;
    if (minute_new > 59) {
      minute_new = 0;
    }
  }

  printTimeToScreen(minute_new, second_new, milliseconds_new);

  if (button1Pressed) {
    button1Pressed = false;
    startCronometer = true;
    milliseconds = millis();
  }
  if (button2Pressed) {
    button2Pressed = false;
    state = 0;                // go to initial state
    startCronometer = false;
    second_witness = 99;
    clearScreen();
    sendTime();
  }
  if (button3Pressed) {
    button3Pressed = false;
    startCronometer = false;
  }
}

void checkAlarms() {
  if (myRTC.checkAlarmEnabled(1) && myRTC.checkIfAlarm(1)) {
    // alarm 1 is ready
    alarmMelody1Start = true;
    clearScreen();
    
    while (alarmMelody1Start) {
      showAlarmON();
    }
    myRTC.turnOffAlarm(1);
  }
  
  if (myRTC.checkAlarmEnabled(2) && myRTC.checkIfAlarm(2)) {
    // alarm 2 is ready
    alarmMelody2Start = true;
    clearScreen();

    while (alarmMelody2Start) {
      showAlarmON();
    }
    // delete alarm
    myRTC.turnOffAlarm(2);
  }
}
void showAlarmON() {
  // show alarm message on TFT
  tft.setCursor(25, 80);
  tft.setTextSize(2);
  tft.println("ALARM!");

  // start melody
  playMelody();
}

void writeMenu() {
  tft.setCursor(5, 150);
  tft.setTextSize(1);
  if (state == 0) {
    tft.println("SET    CRON    TIMER"); 
  }
  if (state == 1 || state == 17) {
    tft.println("SET    OUT     NEXT");     // set clock, set melody
  }  
  if ((state > 1 && state < 8) || (state > 10 && state < 16) || (state >=20 && state < 23)) {
    tft.println(" +      -      NEXT");     // clock, alarm, set timer
  }
  if (state == 10) {
    tft.println("SET    OUT     VIEW");     // set alarm
  }
  if (state == 18) {
    tft.println("DEL    OUT     NEXT");     // view alarm
  }
  if (state == 30 || state == 23) {
    tft.println("START   OUT    STOP");     // cron
  }
}

void sendTime() {
  // show date and time on TFT
  readTime();
  if (second != second_witness) {
    printTimeToScreen(hourofday, minute, second);
    second_witness = second;
    printDateToScreen(date, month, year);
  }
}

