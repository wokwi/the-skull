/*
     ___________.__               ___________         .__  .__   
     \__    ___/|  |__   ____    /   _____/  | ____ __|  | |  |  
       |    |   |  |  \_/ __ \   \_____  \|  |/ /  |  \  | |  |  
       |    |   |   Y  \  ___/   /        \    <|  |  /  |_|  |__
       |____|   |___|  /\___  > /_______  /__|_ \____/|____/____/
                     \/     \/          \/     \/                
                     A mind-bending hardware puzzle in three acts
     
This firmware is released under an open-curse license. If you don't
play fair, you'll be punished. 

https://skullctf.com/start

FIRMWARE RELEASE v1.00

Recommended soundtrack when reading the code:

https://www.youtube.com/watch?v=Fku7hi5kI-c

Copyright (C) 2020, Uri Shaked. 

*/

#include <avr/boot.h>
#include <avr/sleep.h>
#include <EEPROM.h>

const char flagflag[15] __attribute__((used)) EEMEM = "CTF{flag1 here}";
const uint8_t flagCheck __attribute__((used)) EEMEM = 0x2e;

#define TOUCH_PIN 2
#define LED1_PIN 3
#define LED2_PIN 4

#define CLK_PIN A2
#define DIN_PIN A3

const int firmwareRevision = 0x1;
const int hypnosisDuration = 5000;

void indicate(int8_t value) {
  for (; value > 0; value--) {
    digitalWrite(LED2_PIN, HIGH);
    delay(250);
    digitalWrite(LED2_PIN, LOW);
    delay(250);
  }
}

void spitByte(uint8_t value, int delayMs) {
  for (uint8_t b = 0; b < 8; b++) {
    digitalWrite(LED2_PIN, (value & bit(b)) ? HIGH : LOW);
    digitalWrite(LED1_PIN, HIGH);
    delay(delayMs / 2);
    digitalWrite(LED1_PIN, LOW);
    delay(delayMs / 2);
  }
  digitalWrite(LED2_PIN, LOW);
}

uint8_t crc8(uint8_t const message[], int nBytes) {
  static const uint8_t tbl[] PROGMEM = {
    0, 213, 127, 170, 254, 43, 129, 84, 41, 252, 86, 131, 215, 2, 168, 125, 82, 135, 45, 248, 172, 121, 211, 6, 123, 174, 4, 209, 133, 80, 250, 47, 164, 113, 219, 14, 90, 143, 37, 240, 141, 88, 242, 39, 115, 166, 12, 217, 246, 35, 137, 92, 8, 221, 119, 162, 223, 10, 160, 117, 33, 244, 94, 139, 157, 72, 226, 55, 99, 182, 28, 201, 180, 97, 203, 30, 74, 159, 53, 224, 207, 26, 176, 101, 49, 228, 78, 155, 230, 51, 153, 76, 24, 205, 103, 178, 57, 236, 70, 147, 199, 18, 184, 109, 16, 197, 111, 186, 238, 59, 145, 68, 107, 190, 20, 193, 149, 64, 234, 63, 66, 151, 61, 232, 188, 105, 195, 22, 239, 58, 144, 69, 17, 196, 110, 187, 198, 19, 185, 108, 56, 237, 71, 146, 189, 104, 194, 23, 67, 150, 60, 233, 148, 65, 235, 62, 106, 191, 21, 192, 75, 158, 52, 225, 181, 96, 202, 31, 98, 183, 29, 200, 156, 73, 227, 54, 25, 204, 102, 179, 231, 50, 152, 77, 48, 229, 79, 154, 206, 27, 177, 100, 114, 167, 13, 216, 140, 89, 243, 38, 91, 142, 36, 241, 165, 112, 218, 15, 32, 245, 95, 138, 222, 11, 161, 116, 9, 220, 118, 163, 247, 34, 136, 93, 214, 3, 169, 124, 40, 253, 87, 130, 255, 42, 128, 85, 1, 212, 126, 171, 132, 81, 251, 46, 122, 175, 5, 208, 173, 120, 210, 7, 83, 134, 44, 249
  };
  uint8_t r = 0;
  for (int i = 0; i < nBytes; ++i) {
    r = pgm_read_byte(tbl + (message[i] ^ r));
  }
  return (r);
}

ISR(INT0_vect) {
  bitClear(GIMSK, INT0);  // Disable INT0 interrupt
  sleep_disable();
}

void sleep() {
  uint8_t oldAdc = ADCSRA;

  // Disable ADC - saves about 324.5uA in sleep mode!
  ADCSRA = 0;

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  noInterrupts();
  bitSet(GIMSK, INT0);  // Enable INT0 interrupt
  interrupts();
  sleep_cpu();

  // Restore ADC
  ADCSRA = oldAdc;
}

void spell() {

                  uint8_t*a,pc=16,sp=0,
               s[32]={0},op;while(!0){op=
            EEPROM.read(pc);switch(+op){case
          ',':delay(s[sp-1]);sp--;break;case'>':
         s[sp-1]>>=1|1;break;case'<':s[sp-1]<<=1;
        break;case'=':pc=s[sp-1]-1;sp--;break;case
       '@':if(s[sp-2]){s[sp-2]--;pc=s[sp-1]-1;sp+=
      1;}sp-=2;break;case'&':s[sp-2]&=s[sp-1];sp-=1;
      break;case'|':s[sp-2]|=s[sp-1];sp-=1;break;case
    '^':s[sp-2]^=s[sp-1];sp--;break;case'+':s[sp-2]+=
   s[sp-1];sp=sp-1;break;case'-':s[sp-2]-=s[sp-1];sp--;
   break;case'2':s[sp]=s[sp-1];sp=sp+1;break;case'?':s[
 sp-1]=EEPROM.         read(s[sp-1]|0        );break;case
  "!!!"[0]:             666,EEPROM              .write(s
   [sp-1]                ,s[sp-2]                );sp=+
    sp-02;               ;break;                 1;case
    "Arr"[               1]:  s[+               sp-1]=
    *(char*)            (s[+   sp-1           ]);break
      ;case'w':*   (char*)(     s[+sp-1])  =s[sp-+2];
        sp-=2;break;case+       'x':s[sp] =s[sp-1
           ];s[sp-1]=s[sp   +    -2];s[sp-2]=s[
            0|sp];break;    ;;    case"zzz"[0
             ]:sleep();"   Arrr  ";break;case
             255  :return;;  default:s  [sp]
              =+   op;sp+=    1,1   ;}pc=
               +    pc  +      1;      %>

}

void factoryTest() {
  uint8_t buf[sizeof(flagflag)];
  eeprom_read_block(buf, &flagflag, sizeof(buf));
  bool okay = crc8(buf, sizeof(buf)) == EEPROM[&flagCheck];
  bool locked = boot_lock_fuse_bits_get(GET_LOCK_BITS) == 0xfc;
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, locked && okay ? HIGH : LOW);
  delay(1000);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  for (uint8_t i = 255; i >= 16; i--) {
    EEPROM.write(i, 0xff);
  }
  spitByte((firmwareRevision & 0x3f) | (okay << 7) | (locked << 6), 200);
  TCNT1 = (int)spell ^ (int)F("CTF{BonusFlagForTheBrave}");
  delay(500);
}

void setup() {
  // Set most pins as output, saving power during sleep
  for (uint8_t i = 0; i <= 5; i++) {
    pinMode(i, i == TOUCH_PIN ? INPUT : OUTPUT);
  }

  if (!digitalRead(TOUCH_PIN)) {
    factoryTest();
  } else {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    delay(50);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
  }

  analogReference(INTERNAL1V1);
  DIDR0 |= (1 << ADC2D) | (1 << ADC3D);
}

int sample(uint8_t pin, int &minVal) {
  pinMode(pin, INPUT);
  delayMicroseconds(300);
  uint16_t result = 0;
  uint8_t samplesLeft = 4;
  for (; samplesLeft && result < 128; samplesLeft--) {
    result = analogRead(pin);
  }
  pinMode(pin, OUTPUT);
  delayMicroseconds(samplesLeft * 116);
  minVal = min(result, minVal);
  result -= minVal;
  return result;
}

int trackMax(int &maxVal, int value) {
  maxVal = max(value, maxVal);
  return value;
}

int lowPass(uint16_t &filter, int value) {
  filter -= filter >> 5;
  filter += value;
  return filter >> 5;
}

bool isAhoy() {
  uint8_t counter = 0;
  int lMin = INT16_MAX, lPeak = 0;
  int rMin = INT16_MAX, rPeak = 0;
  uint16_t lFilter = 0, rFilter = 0;
  bool rNext = false;

  for (int i = 0; i < 2200; i++) {
    if (!digitalRead(TOUCH_PIN)) {
      return false;
    }
    auto l = trackMax(lPeak, lowPass(lFilter, sample(CLK_PIN, lMin)));
    auto r = trackMax(rPeak, lowPass(rFilter, sample(DIN_PIN, rMin)));
    if (!rNext && l < lPeak / 8) {
      rNext = true;
      rPeak = 0;
    }
    if (rNext && r < rPeak / 8) {
      counter++;
      rNext = false;
      lPeak = 0;
    }
  }

  if (counter == 4) {
    return true;
  }

  if (counter > 2) {
    indicate(min(counter - 1, 9));
  }

  return false;
}

void doIt(uint8_t cmd[16], uint8_t cmdLen) {
  if (cmdLen < 2) {
    indicate(2);
    return;
  }

  // Read
  if (cmdLen == 2) {
    uint8_t addr = cmd[0];
    uint8_t speed = cmd[1] >> 4;
    uint8_t count = 1 + 2 * (cmd[1] & 0xf);
    for (uint8_t i = 0; i < count; i++) {
      spitByte(EEPROM.read((addr + i) & 0xff), 10 + speed * 33);
    }
    return;
  }

  // Write
  int crcValue = crc8(cmd, cmdLen - 1);
  if (crcValue != cmd[cmdLen - 1]) {
    indicate(3);
    return;
  }
  
  uint8_t addr = cmd[0];
  if (addr < sizeof(flagflag) + 1){
    indicate(4);
    return;
  }

  for (uint8_t i = 0; i < cmdLen - 2; i++) {
    EEPROM.write((addr + i) & 0xff, cmd[i + 1]);
  }
  indicate(1);
}

void mindBender() {
  for (uint8_t i = 0; i < 6; i++) {
    digitalWrite(LED1_PIN, i % 2 ? LOW : HIGH);
    delay(100);
  }

  int timeLeft = hypnosisDuration;
  int changeTime = 4;
  int lMin = INT16_MAX, lPeak = 0;
  int vMin = INT16_MAX, vPeak = 0;
  bool lastL = false;
  uint8_t sequence = 0;
  bool sequenceMatch = false;
  uint8_t buf[16];
  int8_t bufIdx = 0;
  uint8_t cmdLen = 0;
  while (timeLeft-- && digitalRead(TOUCH_PIN)) {
    bool l = trackMax(lPeak, sample(CLK_PIN, lMin)) >= 4 + lPeak / 8;
    bool v = trackMax(vPeak, sample(DIN_PIN, vMin)) >= 4 + vPeak / 8;
    if (l != lastL) {
      changeTime = 0;
      lastL = l;
    }
    if (changeTime++ == 3) {
      sequence = (sequence << 1) | v;
      bufIdx++;
      if (!sequenceMatch) {
        sequenceMatch = (sequence & 0x7f) == 0b1001011;
        bufIdx = -4;
      } else {
        timeLeft = hypnosisDuration;
        if (bufIdx == 0) {
          cmdLen = sequence & 0xf;
        } else if (bufIdx % 8 == 0) {
          buf[bufIdx / 8 - 1] = sequence;
          if (bufIdx / 8 == cmdLen) {
            delay(500);
            doIt(buf, cmdLen);
            sequenceMatch = false;
          }
        }
      }
    }
  }
}

void loop() {
  sleep();

  // Someone touched - Briefly wake up!
  int cnt = 0;
  while (!digitalRead(TOUCH_PIN)) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    delay(100);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    delay(100);
    cnt++;
  }

  if (cnt > 10) {
    spell();
  }

  if (isAhoy()) {
    mindBender();
  }
}
