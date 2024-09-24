// Control 4 LEDs via Bluetooth
// using RoboRemo app
// www.roboremo.app

// Hardware setup:
// BT module   Arduino
// GND ------- GND
// VCC ------- 5V
// TX-O ------ pin2
// RX-I ------ pin3

// LEDs are connected on pins 13, 12, 11, 10

#include <SoftwareSerial.h>  
#include <LiquidCrystal.h>
#include <GxEPD.h>
#include <GxFont_GFX.h>

#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"
#include "dht.h"
dht DHT22;
#define DHT22PIN 29    //przypisanie pinu 2 Arduino jako odczyt z sensora


int bluetoothTx = 11; // TX-O pin of BT module to Arduino pin2
int bluetoothRx = 12; // RX-I pin of B module to Arduino pin3

int in1 = 2;
int in2 = 4;
int enA = 3;

int in4 = 7;
int in3 = 5;
int enB = 6;

int glosnik = 49;
int woda = 24;
int ledy = 21;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

const int rs = 14, en = 15, d4 = 16, d5 = 17, d6 = 18, d7 = 19;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char cmd[100];
int cmdIndex;



int melody[] = {
  659, 622, 659, 622, 659, 494, 587, 523, 440, 
  262, 330, 440, 494, 330, 415, 494, 523, 330, 
  659, 622, 659, 622, 659, 494, 587, 523, 440, 
  262, 330, 440, 494, 330, 523, 494, 440
};

int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 4, 4, 2, 
  4, 4, 4, 4, 4, 4, 4, 2, 4, 
  4, 4, 4, 4, 4, 4, 4, 4, 2, 
  4, 4, 4, 4, 4, 4, 4, 2
};


int melody2[] = {
  392, 392, 440, 392, 523, 494, 392, 392, 440, 392, 587, 523,
  392, 392, 784, 659, 523, 494, 440, 698, 698, 659, 523, 587, 523
};

int noteDurations2[] = {
  4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 2
};

int melody3[] = {
  261, 261, 261, 261, 261, 261, 261, 261, // C4 C4 C4 C4 C4 C4 C4 C4
  261, 293, 261, 293, 261, 293, 261, // C4 D4 C4 D4 C4 D4 C4
  261, 293, 261, 293, 261, 293, 261, // C4 D4 C4 D4 C4 D4 C4
  261, 293, 261, 293, 261, 293, 261, // C4 D4 C4 D4 C4 D4 C4
  261, 293, 261, 293, 261, 293, 261, // C4 D4 C4 D4 C4 D4 C4
  293, 293, 293, 293, 293, 293, 293, 293, // D4 D4 D4 D4 D4 D4 D4 D4
  293, 261, 293, 261, 293, 261, 293, // D4 C4 D4 C4 D4 C4 D4
  261, 293, 261, 293, 261, 293, 261  // C4 D4 C4 D4 C4 D4 C4
};

int noteDurations3[] = {
  8, 8, 8, 8, 8, 8, 8, 8, // C4 C4 C4 C4 C4 C4 C4 C4
  8, 8, 8, 8, 8, 8, 8, // C4 D4 C4 D4 C4 D4 C4
  8, 8, 8, 8, 8, 8, 8, // C4 D4 C4 D4 C4 D4 C4
  8, 8, 8, 8, 8, 8, 8, // C4 D4 C4 D4 C4 D4 C4
  8, 8, 8, 8, 8, 8, 8, // C4 D4 C4 D4 C4 D4 C4
  8, 8, 8, 8, 8, 8, 8, 8, // D4 D4 D4 D4 D4 D4 D4 D4
  8, 8, 8, 8, 8, 8, 8, // D4 C4 D4 C4 D4 C4 D4
  8, 8, 8, 8, 8, 8, 8  // C4 D4 C4 D4 C4 D4 C4
};


void exeCmd() {
  
  if(strcmp(cmd, "p1")==0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enA, 200);
    analogWrite(enB, 200);
  }
  if(strcmp(cmd, "p0")==0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
  }

    if(strcmp(cmd, "t1")==0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enA, 200);
    analogWrite(enB, 200);
  }

    if(strcmp(cmd, "l1")==0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH); 
    digitalWrite(in4, LOW);
    analogWrite(enA, 200);
    analogWrite(enB, 200);
  }

      if(strcmp(cmd, "r1")==0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW); 
    digitalWrite(in4, HIGH);
    analogWrite(enA, 200);
    analogWrite(enB, 200);
  }

      if(strcmp(cmd, "m1")==0) {
  for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(int); thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(glosnik, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(glosnik);
  }
  }

        if(strcmp(cmd, "m2")==0) {
  for (int thisNote2 = 0; thisNote2 < sizeof(melody2) / sizeof(int); thisNote2++) {
    int noteDuration2 = 1000 / noteDurations2[thisNote2];
    tone(glosnik, melody2[thisNote2], noteDuration2);
    int pauseBetweenNotes2 = noteDuration2 * 1.30;
    delay(pauseBetweenNotes2);
    noTone(glosnik);
  }
  }

          if(strcmp(cmd, "m3")==0) {
  for (int thisNote3 = 0; thisNote3 < sizeof(melody3) / sizeof(int); thisNote3++) {
    int noteDuration3 = 1000 / noteDurations3[thisNote3];
    tone(glosnik, melody3[thisNote3], noteDuration3);
    int pauseBetweenNotes3 = noteDuration3 * 1.30;
    delay(pauseBetweenNotes3);
    noTone(glosnik);
  }
  }

  if(strcmp(cmd, "d1")==0) {
    Config_Init();
  LCD_Init();
  LCD_Clear(WHITE);
  LCD_SetBacklight(1000);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, WHITE);
  Paint_SetRotate(90);
  Paint_DrawImage(gImage_1, 4, 4, 120, 120); 
  }

    if(strcmp(cmd, "d2")==0) {
      Config_Init();
  LCD_Init();
  LCD_Clear(WHITE);
  LCD_SetBacklight(1000);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, WHITE);
  Paint_SetRotate(90);
  Paint_DrawImage(gImage_2, 4, 4, 120, 120); 
  }


    if(strcmp(cmd, "woda1")==0) {
    digitalWrite(woda, LOW);
    
  }

    if(strcmp(cmd, "woda0")==0) {
    digitalWrite(woda, HIGH);
  }

      if(strcmp(cmd, "ledy1")==0) {
    digitalWrite(ledy, LOW);
    Serial.println("dziala");
  }

    if(strcmp(cmd, "ledy0")==0) {
    digitalWrite(ledy, HIGH);
  }

      if(strcmp(cmd, "ekran")==0) {
        lcd.begin(16, 4);
         int chk = DHT22.read(DHT22PIN);         //sprawdzenie stanu sensora, a następnie wyświetlenie komunikatu na monitorze szeregowym
  int wilg = analogRead(A1);
  int wilgotnoscGleby = map(wilg,310,570, 100, 0);

  lcd.print("Wilgotnosc: ");
  lcd.print((float)DHT22.humidity, 2);
  lcd.print("%");

  lcd.print("Temperatura: ");
  lcd.print((float)DHT22.temperature, 2);
  lcd.print(" C");

    lcd.print("W. Gleby: ");
  lcd.print(wilgotnoscGleby, 2);
  lcd.print(" %");
  }


}



void setup() {
  Serial.begin(9600);
  delay(500); // wait for bluetooth module to start

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(woda, OUTPUT);
  digitalWrite(woda, HIGH);
  pinMode(ledy, OUTPUT);
  digitalWrite(ledy, HIGH);
  bluetooth.begin(115200); // Bluetooth default baud is 115200
  
  bluetooth.print("$");
  bluetooth.print("$");
  bluetooth.print("$"); // enter cmd mode
  delay(250);  
  bluetooth.println("U,9600,N"); // change baud to 9600
  
  bluetooth.begin(9600);

  
  cmdIndex = 0;

    Config_Init();
  LCD_Init();
  LCD_Clear(WHITE);
  LCD_SetBacklight(1000);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, WHITE);
  Paint_Clear(WHITE);
  Paint_SetRotate(90);

    lcd.begin(16, 4);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  
  if(bluetooth.available()) {
    
    char c = (char)bluetooth.read();
    
    if(c=='\n') {
      cmd[cmdIndex] = 0;
      exeCmd();  // execute the command
      cmdIndex = 0; // reset the cmdIndex
    } else {      
      cmd[cmdIndex] = c;
      if(cmdIndex<99) cmdIndex++;
    }
  }

}
  
