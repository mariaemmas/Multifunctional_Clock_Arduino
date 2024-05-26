void printDateToScreen(byte day, byte month, byte year) {
  char dateString[15];  // Enough to hold "6 Apr 2024" + null terminator
  
  // Format the date as "day month year"
  sprintf(dateString, "%d %s %d", day, months[month - 1], 2000 + year);
  
  tft.setCursor(25, 50);    // Set position (x,y)
  tft.setTextSize(1); 
  tft.println(dateString);
}

void printTimeToScreen(byte hour, byte minute, byte second) {
  char timeString[9];  // HH:MM:SS is 8 characters + null terminator 
  sprintf(timeString, "%02d:%02d:%02d", hour, minute, second);
  
  tft.setCursor(15, 80);  // Set position (x,y)
  tft.setTextSize(2);  
  tft.println(timeString);  

  tft.fillRect(101, 8, 20, 11, ST7735_BLACK);  // Draw filled rectangle (x,y,width,height,color)

}

void clearScreen() {
  tft.fillRect(0, 0, 130, 160, ST7735_BLACK);
}

