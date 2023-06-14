#include <SPI.h>
#include <esp_sleep.h>

int dataPin1 = 12;
int clkPin1 = 14;
int csPin1 = 27;
int dataPin2 = 2;
int clkPin2 = 0;
int csPin2 = 4;

byte max7219_REG_noop = 0x00;
byte max7219_REG_digit0 = 0x01;
byte max7219_REG_digit1 = 0x02;
byte max7219_REG_digit2 = 0x03;
byte max7219_REG_digit3 = 0x04;
byte max7219_REG_digit4 = 0x05;
byte max7219_REG_digit5 = 0x06;
byte max7219_REG_digit6 = 0x07;
byte max7219_REG_digit7 = 0x08;
byte max7219_REG_decodeMode = 0x09;
byte max7219_REG_intensity = 0x0a;
byte max7219_REG_scanLimit = 0x0b;
byte max7219_REG_shutdown = 0x0c;
byte max7219_REG_displayTest = 0x0f;

// Pin connections for button
int buttonPin = 26;

/* Numara
byte dice1[8] = {0x18,0x38,0x78,0x18,0x18,0x18,0x18,0x7e};
byte dice2[8] = {0x7c,0x7e,0x06,0x1c,0x38,0x60,0x7e,0x7e};
byte dice3[8] = {0x7c,0x7e,0x06,0x3c,0x3c,0x06,0x7e,0x7c};
byte dice4[8] = {0x66,0x66,0x66,0x7e,0x3e,0x06,0x06,0x06};
byte dice5[8] = {0x7e,0x7e,0x60,0x7c,0x7e,0x02,0x7e,0x7c};
byte dice6[8] = {0x3e,0x7e,0x60,0x7c,0x7e,0x62,0x7e,0x3c};
*/

// Nokta
byte dice1[8] = { 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00 }; // 1
byte dice2[8] = { 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03 }; // 2
byte dice3[8] = { 0x03, 0x03, 0x00, 0x18, 0x18, 0x00, 0xC0, 0xC0 }; // 3
byte dice4[8] = { 0xC3, 0xC3, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xC3 }; // 4
byte dice5[8] = { 0xC3, 0xC3, 0x00, 0x18, 0x18, 0x00, 0xC3, 0xC3 }; // 5
byte dice6[8] = { 0xC3, 0xC3, 0x00, 0xC3, 0xC3, 0x00, 0xC3, 0xC3 }; // 6


void SPI_SendByte(byte data, int dataPin, int clkPin, int csPin) {
  byte i = 8;
  byte mask;
  while(i > 0) {
  mask = 0x01 << (i - 1);
  digitalWrite(clkPin, LOW);
  if (data & mask){
    digitalWrite(dataPin, HIGH);
  }
  else{
    digitalWrite(dataPin, LOW);
  }
    digitalWrite(clkPin, HIGH);
    --i;
  }
}

void MAX7219_1Unit(byte reg_addr, byte reg_data, int dataPin, int clkPin, int csPin) {
  digitalWrite(csPin, LOW);
  SPI_SendByte(reg_addr, dataPin, clkPin, csPin);
  SPI_SendByte(reg_data, dataPin, clkPin, csPin);
  digitalWrite(csPin,HIGH);
}

void Draw (byte *LED_matrix, int dataPin, int clkPin, int csPin){
  MAX7219_1Unit(1, LED_matrix[0], dataPin, clkPin, csPin);
  MAX7219_1Unit(2, LED_matrix[1], dataPin, clkPin, csPin);
  MAX7219_1Unit(3, LED_matrix[2], dataPin, clkPin, csPin);
  MAX7219_1Unit(4, LED_matrix[3], dataPin, clkPin, csPin);
  MAX7219_1Unit(5, LED_matrix[4], dataPin, clkPin, csPin);
  MAX7219_1Unit(6, LED_matrix[5], dataPin, clkPin, csPin);
  MAX7219_1Unit(7, LED_matrix[6], dataPin, clkPin, csPin);
  MAX7219_1Unit(8, LED_matrix[7], dataPin, clkPin, csPin);
}

void setup() {
  pinMode(dataPin1, OUTPUT);
  pinMode(clkPin1, OUTPUT);
  pinMode(csPin1, OUTPUT);
  pinMode(dataPin2, OUTPUT);
  pinMode(clkPin2, OUTPUT);
  pinMode(csPin2, OUTPUT);
  pinMode(buttonPin, INPUT);

  MAX7219_1Unit(max7219_REG_scanLimit, 0x07, dataPin1, clkPin1, csPin1);
  MAX7219_1Unit(max7219_REG_decodeMode, 0x00, dataPin1, clkPin1, csPin1);
  MAX7219_1Unit(max7219_REG_shutdown, 0x00, dataPin1, clkPin1, csPin1);
  MAX7219_1Unit(max7219_REG_displayTest, 0x00, dataPin1, clkPin1, csPin1);
  MAX7219_1Unit(max7219_REG_intensity, 0x08, dataPin1, clkPin1, csPin1);
  
  MAX7219_1Unit(max7219_REG_scanLimit, 0x07, dataPin2, clkPin2, csPin2);
  MAX7219_1Unit(max7219_REG_decodeMode, 0x00, dataPin2, clkPin2, csPin2);
  MAX7219_1Unit(max7219_REG_shutdown, 0x00, dataPin2, clkPin2, csPin2);
  MAX7219_1Unit(max7219_REG_displayTest, 0x00, dataPin2, clkPin2, csPin2);
  MAX7219_1Unit(max7219_REG_intensity, 0x08, dataPin2, clkPin2, csPin2);
}

unsigned long lastActionTime = 0;
const int timeout = 30000; // 30 milisaniye animasyon dönsün

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    Serial.print("BUton Pressed");
    MAX7219_1Unit(max7219_REG_shutdown, 0x01, dataPin1, clkPin1, csPin1);
    MAX7219_1Unit(max7219_REG_shutdown, 0x01, dataPin2, clkPin2, csPin2);
    lastActionTime = millis();
    int dice1Num = random(1, 7);
    int dice2Num = random(1, 7);
    byte* dice1Matrix;
    byte* dice2Matrix;
    // Zar atma animasyonunu gösterelim.
    for (int i = 0; i < 30; i++) {
      int randomDice1 = random(1, 7);
      int randomDice2 = random(1, 7);
      switch(randomDice1){
        case 1:
          dice1Matrix = dice1;
          break;
        case 2:
          dice1Matrix = dice2;
          break;
        case 3:
          dice1Matrix = dice3;
          break;
        case 4:
          dice1Matrix = dice4;
          break;
        case 5:
          dice1Matrix = dice5;
          break;
        case 6:
          dice1Matrix = dice6;
          break;
      }
      switch(randomDice2){
        case 1:
          dice2Matrix = dice1;
          break;
        case 2:
          dice2Matrix = dice2;
          break;
        case 3:
          dice2Matrix = dice3;
          break;
        case 4:
          dice2Matrix = dice4;
          break;
        case 5:
          dice2Matrix = dice5;
          break;
        case 6:
          dice2Matrix = dice6;
          break;
      }
      Draw(dice1Matrix, dataPin1, clkPin1, csPin1);
      Draw(dice2Matrix, dataPin2, clkPin2, csPin2);
      delay(100);
      }
      // Animasyon sonrası gelen rakamı gösterelim.
      switch(dice1Num){
        case 1:
        dice1Matrix = dice1;
        break;
        case 2:
        dice1Matrix = dice2;
        break;
        case 3:
        dice1Matrix = dice3;
        break;
        case 4:
        dice1Matrix = dice4;
        break;
        case 5:
        dice1Matrix = dice5;
        break;
        case 6:
        dice1Matrix = dice6;
        break;
      }
      switch(dice2Num){
        case 1:
        dice2Matrix = dice1;
        break;
        case 2:
        dice2Matrix = dice2;
        break;
        case 3:
        dice2Matrix = dice3;
        break;
        case 4:
        dice2Matrix = dice4;
        break;
        case 5:
        dice2Matrix = dice5;
        break;
        case 6:
        dice2Matrix = dice6;
        break;
      }
      Draw(dice1Matrix, dataPin1, clkPin1, csPin1);
      Draw(dice2Matrix, dataPin2, clkPin2, csPin2);      
  }

  if (millis() - lastActionTime > timeout) {
    // Ledleri kapatalım ve pilden tasarruf edelim.
    MAX7219_1Unit(max7219_REG_shutdown, 0x00, dataPin1, clkPin1, csPin1);
    MAX7219_1Unit(max7219_REG_shutdown, 0x00, dataPin2, clkPin2, csPin2);
    lastActionTime = millis(); // Zamanlayıcıyı sıfırlayalım.
  }
}
