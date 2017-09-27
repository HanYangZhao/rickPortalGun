// Demo the quad alphanumeric display LED backpack kit
// scrolls through every character, then scrolls Serial
// input onto the display

//#include <i2c_t3.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#include <Audio.h>
#include <SD.h>
#include <SerialFlash.h>
AudioPlaySdWav playWav1;
AudioOutputAnalog audioOutput;

AudioConnection patchCord1(playWav1, 0, audioOutput, 0);
AudioConnection patchCord2(playWav1, 1, audioOutput, 1);

#define SDCARD_CS_PIN BUILTIN_SDCARD

#include <Encoder.h>
#define PIN_ENCODER_SWITCH 0

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
Encoder myEnc(2, 1);


void lcdInit(){
  alpha4.writeDigitRaw(3, 0x0);
  alpha4.writeDigitRaw(0, 0xFFFF);
  alpha4.writeDisplay();
  delay(1);
  alpha4.writeDigitRaw(0, 0x0);
  alpha4.writeDigitRaw(1, 0xFFFF);
  alpha4.writeDisplay();
  delay(1);
  alpha4.writeDigitRaw(1, 0x0);
  alpha4.writeDigitRaw(2, 0xFFFF);
  alpha4.writeDisplay();
  delay(1);
  alpha4.writeDigitRaw(2, 0x0);
  alpha4.writeDigitRaw(3, 0xFFFF);
  alpha4.writeDisplay();
  delay(1);
  
  alpha4.clear();
  alpha4.writeDisplay();

  // display every character, 
  for (uint8_t i='!'; i<='z'; i++) {
    alpha4.writeDigitAscii(0, i);
    alpha4.writeDigitAscii(1, i+1);
    alpha4.writeDigitAscii(2, i+2);
    alpha4.writeDigitAscii(3, i+3);
    alpha4.writeDisplay();
    
    delay(5);
  }
  alpha4.writeDigitAscii(0, 'R');
  alpha4.writeDigitAscii(1, 'I');
  alpha4.writeDigitAscii(2, 'C');
  alpha4.writeDigitAscii(3, 'K');
  alpha4.writeDisplay();
}




void setup() {
  Serial.begin(9600);
  alpha4.begin(0x70);  // pass in the address
  lcdInit();
  pinMode(PIN_ENCODER_SWITCH, INPUT_PULLUP);
  AudioMemory(8);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while(1){
      Serial.println("Unable to access the SD card");
    }
  }
}

void playFile(const char *filename){
  Serial.print("Playing file: ");
  Serial.println(filename);
  
  // Start playing the file. This sketch continues to
  // run while the file plays.
  playWav1.play(filename);
  
  // A brief delay for the library read WAV info
  delay(5);
  
  // Simply wait for the file to finish playing.
  while (playWav1.isPlaying()) {
  
  }
}

long oldPosition  = 0;
char displaybuffer[4] = {' ', ' ', ' ', ' '};
char x[2];
char y[2];
char z[2];
char file[20];
String str1,str2,str3,filename;
int switchState = 0;
void loop() {
  int newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    if(newPosition >= 999){
      newPosition = 999;
    }
    else if(newPosition <= 0){
      newPosition = 0;
    }
     oldPosition = newPosition;

    str1 = String(newPosition /100 % 10);
    str2 = String(newPosition /10 % 10);
    str3 = String(newPosition % 10);
    str1.toCharArray(x,2);
    str2.toCharArray(y,2);
    str3.toCharArray(z,2);
    alpha4.writeDigitAscii(0, 'C');
    alpha4.writeDigitAscii(1, x[0]);
    alpha4.writeDigitAscii(2, y[0]);
    alpha4.writeDigitAscii(3,z[0]);
    alpha4.writeDisplay();
    
  }
  switchState = digitalRead(PIN_ENCODER_SWITCH);
  if(switchState == LOW){
    if(newPosition > 15){
      playFile("portal.wav");
    }
    else{
      filename = str2 += str3 += ".wav";
      filename.toCharArray(file,20);
      playFile(file);
    }
  }




  
}
