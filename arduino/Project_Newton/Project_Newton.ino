#define DEBOUNCE 7
#define LONG_PRESS 800

#define LC0 5 //A5 - PORTF
#define LC1 4 //A4 - PORTF
#define LC2 3 //A3 - PORTF
#define LC3 2 //A2 - PORTF
#define LC4 1 //A1 - PORTF
#define LC5 0 //A0 - PORTF
#define LC6 4 //2 - PORTE
#define LC7 5 //3 - PORTE
#define LC8 5 //4 - PORTG
#define LC9 3 //5 - PORTE

//{"PIN_ON", "PIN_OFF", "PORT"}
//MEGA:
//1 == PORTF; 2 == PORTE; 3 == PORTG
/*byte charlyPins[][3] = { {0b00100000, 0b11011111, 1}, 
                         {0b00010000, 0b11101111, 1}, 
                         {0b00001000, 0b11110111, 1}, 
                         {0b00000100, 0b11111011, 1}, 
                         {0b00000010, 0b11111101, 1}, 
                         {0b00000001, 0b11111110, 1}, 
                         {0b00010000, 0b11101111, 2}, 
                         {0b00100000, 0b11011111, 2}, 
                         {0b00100000, 0b11011111, 3},
                         {0b00001000, 0b11110111, 2} };
*/
//NANO:
//1 == PORTD; 2 == PORTB
byte charlyPins[][3] = { {0b00001000, 0b11110111, 2}, 
                         {0b00000100, 0b11111011, 2}, 
                         {0b00000010, 0b11111101, 2}, 
                         {0b00000001, 0b11111110, 2}, 
                         {0b10000000, 0b01111111, 1}, 
                         {0b01000000, 0b10111111, 1}, 
                         {0b00100000, 0b11011111, 1}, 
                         {0b00010000, 0b11101111, 1}, 
                         {0b00001000, 0b11110111, 1},
                         {0b00000100, 0b11111011, 1} };

//{Anode+, Cathode-}
byte charlyMatrix[][9][2] = { { {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0} },
                              { {0, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}, {9, 1} },
                              { {0, 2}, {1, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2}, {9, 2} },
                              { {0, 3}, {1, 3}, {2, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}, {8, 3}, {9, 3} },
                              { {0, 4}, {1, 4}, {2, 4}, {3, 4}, {5, 4}, {6, 4}, {7, 4}, {8, 4}, {9, 4} },
                              { {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {6, 5}, {7, 5}, {8, 5}, {9, 5} },
                              { {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}, {7, 6}, {8, 6}, {9, 6} },
                              { {0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}, {6, 7}, {8, 7}, {9, 7} },
                              { {0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {7, 8}, {9, 8} } };
//cellLedMatric[CELL][LED]
byte cellLedMatrix[][4] = { {0, 1, 9, 10},
                            {2, 3, 11, 12},
                            {4, 5, 13, 14},
                            {6, 7, 15, 16},
                            
                            {10, 11, 19, 20},
                            {12, 13, 21, 22},
                            {14, 15, 23, 24},
                            {16, 17, 25, 26},
                            
                            {18, 19, 27, 28},
                            {20, 21, 29, 30},
                            {22, 23, 31, 32},
                            {24, 25, 33, 34},
                            
                            {28, 29, 37, 38},
                            {30, 31, 39, 40},
                            {32, 33, 41, 42},
                            {34, 35, 43, 44},
                            
                            {36, 37, 45, 46},
                            {38, 39, 47, 48},
                            {40, 41, 49, 50},
                            {42, 43, 51, 52},
                            
                            {46, 47, 55, 56},
                            {48, 49, 57, 58},
                            {50, 51, 59, 60},
                            {52, 53, 61, 62},
                            
                            {54, 55, 63, 64},
                            {56, 57, 65, 66},
                            {58, 59, 67, 68},
                            {60, 61, 69, 70},

                            {64, 65, 73, 74},
                            {66, 67, 75, 76},
                            {68, 69, 77, 78},
                            {70, 71, 79, 80} };

byte displayFunction = 0xE0;
byte boardArray[13] = {0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD};
int secondCounter = 0;

byte serialData[15];
int serialIndex = 0;

void displayCellLights(byte cell, byte duration) {
  for(int i = 0; i < duration; i++) {
    turnOnLED(cellLedMatrix[cell][0], 1);
    turnOnLED(cellLedMatrix[cell][1], 1);
    turnOnLED(cellLedMatrix[cell][2], 1);
    turnOnLED(cellLedMatrix[cell][3], 1);
  }
}

void turnOnLED(byte led, int msecs) {
  byte col = led % 9;
  byte row = led / 9;
  
  byte *currAnode = charlyPins[charlyMatrix[row][col][0]];
  byte *currCathode = charlyPins[charlyMatrix[row][col][1]];

//MEGA
/*
  if(currAnode[2] == 1)
  {
    DDRF |= currAnode[0];
    PORTF |= currAnode[0];
  }
  if(currAnode[2] == 2)
  {
    DDRE |= currAnode[0];
    PORTE |= currAnode[0];
  }
  if(currAnode[2] == 3)
  {
    DDRG |= currAnode[0];
    PORTG |= currAnode[0];
  }

  if(currCathode[2] == 1)
  {
    DDRF |= currCathode[0];
    PORTF &= currCathode[1];
  }
  if(currCathode[2] == 2)
  {
    DDRE |= currCathode[0];
    PORTE &= currCathode[1];
  }
  if(currCathode[2] == 3)
  {
    DDRG |= currCathode[0];
    PORTG &= currCathode[1];
  }
*/

//NANO
  if(currAnode[2] == 1)
  {
    DDRD |= currAnode[0];
    PORTD |= currAnode[0];
  }
  if(currAnode[2] == 2)
  {
    DDRB |= currAnode[0];
    PORTB |= currAnode[0];
  }

  if(currCathode[2] == 1)
  {
    DDRD |= currCathode[0];
    PORTD &= currCathode[1];
  }
  if(currCathode[2] == 2)
  {
    DDRB |= currCathode[0];
    PORTB &= currCathode[1];
  }

  delay(msecs);

//MEGA
/*
  if(currAnode[2] == 1)
  {
    DDRF &= currAnode[1];
    PORTF &= currAnode[1];
  }
  if(currAnode[2] == 2)
  {
    DDRE &= currAnode[1];
    PORTE &= currAnode[1];
  }
  if(currAnode[2] == 3)
  {
    DDRG &= currAnode[1];
    PORTG &= currAnode[1];
  }

  if(currCathode[2] == 1)
  {
    DDRF &= currCathode[1];
    PORTF &= currCathode[1];
  }
  if(currCathode[2] == 2)
  {
    DDRE &= currCathode[1];
    PORTE &= currCathode[1];
  }
  if(currCathode[2] == 3)
  {
    DDRG &= currCathode[1];
    PORTG &= currCathode[1];
  }
*/

//NANO
if(currAnode[2] == 1)
  {
    DDRD &= currAnode[1];
    PORTD &= currAnode[1];
  }
  if(currAnode[2] == 2)
  {
    DDRB &= currAnode[1];
    PORTB &= currAnode[1];
  }
  
  if(currCathode[2] == 1)
  {
    DDRD &= currCathode[1];
    PORTD &= currCathode[1];
  }
  if(currCathode[2] == 2)
  {
    DDRB &= currCathode[1];
    PORTB &= currCathode[1];
  }
  
}

void readButtons() {
  static bool previousDebounceArray[2] = {false, false};
  static byte previousStateArray[2] = {0, 0};
  static unsigned long pushTimeArray[2] = {0, 0};
  static unsigned long lastTime;
  
  if((lastTime + DEBOUNCE) > millis()) {
    return;
  }

  lastTime = millis();

  //MEGA:
  //bool btn1 = !(PINH & 0b00001000);
  //bool btn2 = !(PINH & 0b00010000);

  //NANO:
  bool btn1 = !(PINC & 0b00001000);
  bool btn2 = !(PINC & 0b00010000);
  

  if(btn1 == previousDebounceArray[0]) {
    if(btn1) {
      if(previousStateArray[0] == 0) {
        pushTimeArray[0] = millis();
        previousStateArray[0] = 1;
      } else if(previousStateArray[0] != 2 && millis() > (pushTimeArray[0] + LONG_PRESS)) {
        Serial.write('C');
        previousStateArray[0] = 2;
      }
    } else {
      if(previousStateArray[0] == 1) {
        Serial.write('A');
      }
      previousStateArray[0] = 0;
    }
  }

  if(btn2 == previousDebounceArray[1]) {
    if(btn2) {
      if(previousStateArray[1] == 0) {
        pushTimeArray[1] = millis();
        previousStateArray[1] = 1;
      } else if(previousStateArray[1] != 2 && millis() > (pushTimeArray[1] + LONG_PRESS)) {
        Serial.write('D');
        previousStateArray[1] = 2;
      }
    } else {
      if(previousStateArray[1] == 1) {
        Serial.write('B');
      }
      previousStateArray[1] = 0;
    }
  }

  previousDebounceArray[0] = btn1;
  previousDebounceArray[1] = btn2;
}

void readComputerPlay() {
  int s = Serial.read();
  if(s != -1) {
    serialData[serialIndex] = s;
    if(s == 0xFF) {
      displayFunction = serialData[0];
      if(displayFunction == 0xE2)
      {
        secondCounter = 0;
      }
      for(int i = 0; i < 12; i++) { //Turn all LEDs off
        boardArray[i] = 0xDD; //0xDD means off (stop byte-code)
      }
      if(serialData[0] != 0xE0 && serialData[0] != 0xE3) {
        for(int i = 1; serialData[i] != 0xFF && i < 12; i++) {
          boardArray[i - 1] = serialData[i];
        }
      }
      serialIndex = 0;
      return;
    }
    serialIndex++;
  }
}

void setup() {
  Serial.begin(57600);
  while(!Serial) {
    ;
  }

  //NOT WRITTEN FOR MEGA
  //NANO:
  DDRD &= 0b00000011;
  PORTD &= 0b0000011;
  DDRB &= 0b11000000;
  PORTB &= 0b11000000;

  //MEGA:
  //DDRH &= 0b11100111;
  //PORTH |= 0b00011000;
  
  //NANO:
  DDRC &= 0b11100111;
  PORTC |=0b00011000;
  
  //pinMode(SW0, INPUT_PULLUP);
  //pinMode(SW1, INPUT_PULLUP);

  for(int i = 0; i < 810; i++) {
    turnOnLED(i % 81, 1);
  }
  for(int i = 0; i < 32; i++) {
    displayCellLights(i, 20);
  }
  displayFunction = 0xE3;
}

void loop() {
  readComputerPlay();
  readButtons();
  
  if(displayFunction == 0xE1) {
    for(int i = 0; boardArray[i] != 0xDD && i < 12; i++) {
      displayCellLights(boardArray[i], 1);
      readButtons();
    }
  }
  
  if(displayFunction == 0xE2) {
    for(int i = 1; boardArray[i] != 0xDD && i < 12; i++) {
      if((secondCounter % 100) < 50) {
        displayCellLights(boardArray[0], 1);
        displayCellLights(boardArray[i], 1);
      } else if(!(secondCounter % 3)) {
        displayCellLights(boardArray[0], 1);
      } else {
        delay(2);
      }
      readButtons();
      secondCounter++;
    }
  }

  if(displayFunction == 0xE3) {
    for(int i = 0; i < 3; i++) {
      for(int j = 0; j < 15; j++) {
        turnOnLED(0, 1);
        turnOnLED(8, 1);
        turnOnLED(10, 1);
        turnOnLED(16, 1);
        turnOnLED(20, 1);
        turnOnLED(24, 1);
        turnOnLED(30, 1);
        turnOnLED(32, 1);
        readButtons();
        turnOnLED(40, 1);
        turnOnLED(48, 1);
        turnOnLED(50, 1);
        turnOnLED(56, 1);
        turnOnLED(60, 1);
        turnOnLED(64, 1);
        turnOnLED(70, 1);
        turnOnLED(72, 1);
        turnOnLED(80, 1);
        readButtons();
      }
      for(int j = 0; j < 10; j++) {
        delay(8);
        readButtons();
        delay(8);
        readButtons();
      }
    }
    displayFunction = 0xE0;
  }
  
}
