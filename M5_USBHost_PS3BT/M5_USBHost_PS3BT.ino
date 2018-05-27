/*
 Example sketch for the PS3 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS3BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

#include <M5Stack.h>
#include "M5StackUpdater.h"
extern const unsigned char gImage_logoM5[];

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

bool printTemperature, printAngle;

void setup() {
  M5.begin();

  if(digitalRead(BUTTON_A_PIN) == 0) {
    Serial.println("Will Load menu binary");
    updateFromFS(SD);
    ESP.restart();
  }

  startupLogo();

  M5.Lcd.setCursor(70, 100);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("M5USB_PS3BT");
  delay(1000);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.fillScreen(BLACK);

  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    M5.Lcd.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));

  M5.Lcd.setTextColor(RED);
  M5.Lcd.print(F("PS3 Bluetooth Library Started\r\n"));
}

int ROW = 0;
int SkipCount = 0;
int SkipCount2 = 0;
int SkipCount3 = 0;
int AnalogHatLX;
int AnalogHatLY;
int AnalogHatRX;
int AnalogHatRY;
int AnalogBtnL2;
int AnalogBtnR2;
int AnglePitch;
int AngleRoll;

boolean PS3BTConnected = false;

void loop() {
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    if(PS3BTConnected == false){
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.print(F("PS3BTConnected\r\n"));
      ROW++;
      PS3BTConnected = true;
      M5.Lcd.setTextColor(WHITE);
    }
  }

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    AnalogHatLX = PS3.getAnalogHat(LeftHatX);
    AnalogHatLY = PS3.getAnalogHat(LeftHatY);
    AnalogHatRX = PS3.getAnalogHat(RightHatX);
    AnalogHatRY = PS3.getAnalogHat(RightHatY);

    if (AnalogHatLX > 137 || AnalogHatLX < 117 || AnalogHatLY > 137 || AnalogHatLY < 117 || AnalogHatRX > 137 || AnalogHatRX < 117 || AnalogHatRY > 137 || AnalogHatRY < 117) {
      SkipCount++;

      if(SkipCount > 1000){
        Serial.print(F("\r\nLeftHatX: "));
        Serial.print(AnalogHatLX);
        Serial.print(F("\tLeftHatY: "));
        Serial.print(AnalogHatLY);

        M5.Lcd.print(F("LHatX: "));
        M5.Lcd.print(AnalogHatLX);
        M5.Lcd.print(F(" LHatY: "));
        M5.Lcd.print(AnalogHatLY);
        
        if (PS3.PS3Connected) { // The Navigation controller only have one joystick
          Serial.print(F("\tRightHatX: "));
          Serial.print(AnalogHatRX);
          Serial.print(F("\tRightHatY: "));
          Serial.print(AnalogHatRY);

          M5.Lcd.print(F(" RHatX: "));
          M5.Lcd.print(AnalogHatRX);
          M5.Lcd.print(F(" RightHatY: "));
          M5.Lcd.print(AnalogHatRY);
        }
        M5.Lcd.print(F("\r\n"));
        ROW++;
        SkipCount = 0;
      }
   }

    // Analog button values can be read from almost all buttons
    AnalogBtnL2 = PS3.getAnalogButton(L2);
    AnalogBtnR2 = PS3.getAnalogButton(R2);
   if (AnalogBtnL2 || AnalogBtnR2 ) {
      SkipCount2++;

      if(SkipCount2 > 1000){
        Serial.print(F("\r\nL2: "));
        Serial.print(AnalogBtnL2);

        M5.Lcd.print(F("L2: "));
        Serial.print(AnalogBtnL2);
        
        if (PS3.PS3Connected) {
          Serial.print(F("\tR2: "));
          Serial.print(AnalogBtnR2);

          M5.Lcd.print(F(" R2: "));
          M5.Lcd.print(AnalogBtnR2);
        }
        M5.Lcd.print(F("\r\n"));
        ROW++;
        SkipCount2 = 0;
      }
    }

    if (PS3.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));

      M5.Lcd.print(F("PS\r\n"));
      ROW++;
      M5.Lcd.print(F("PS3 disconnect\r\n"));
      PS3BTConnected = false;
      ROW++;
      PS3.disconnect();
    }
    else {
      if (PS3.getButtonClick(TRIANGLE)) {
        Serial.print(F("\r\nTraingle"));

        M5.Lcd.print(F("Traingle\r\n"));
        ROW++;

//        PS3.setRumbleOn(RumbleLow);
      }
      if (PS3.getButtonClick(CIRCLE)) {
        Serial.print(F("\r\nCircle"));

        M5.Lcd.print(F("Circle\r\n"));
        ROW++;

//        PS3.setRumbleOn(RumbleHigh);
      }
      if (PS3.getButtonClick(CROSS)) {
        Serial.print(F("\r\nCross"));

        M5.Lcd.print(F("Cross\r\n"));
        ROW++;
      }
      if (PS3.getButtonClick(SQUARE)) {
        Serial.print(F("\r\nSquare"));

        M5.Lcd.print(F("Square\r\n"));
        ROW++;
      }
      if (PS3.getButtonClick(UP)) {
        Serial.print(F("\r\nUp"));

        M5.Lcd.print(F("Up\r\n"));
        ROW++;
        if (PS3.PS3Connected) {
          PS3.setLedOff();
          PS3.setLedOn(LED4);
        }
      }
      if (PS3.getButtonClick(RIGHT)) {
        Serial.print(F("\r\nRight"));

        M5.Lcd.print(F("Right\r\n"));
        ROW++;
        if (PS3.PS3Connected) {
          PS3.setLedOff();
          PS3.setLedOn(LED1);
        }
      }
      if (PS3.getButtonClick(DOWN)) {
        Serial.print(F("\r\nDown"));

        M5.Lcd.print(F("Down\r\n"));
        ROW++;
        if (PS3.PS3Connected) {
          PS3.setLedOff();
          PS3.setLedOn(LED2);
        }
      }
      if (PS3.getButtonClick(LEFT)) {
        Serial.print(F("\r\nLeft"));

        M5.Lcd.print(F("Left\r\n"));
        ROW++;
        if (PS3.PS3Connected) {
          PS3.setLedOff();
          PS3.setLedOn(LED3);
        }
      }

      if (PS3.getButtonClick(L1)) {
        Serial.print(F("\r\nL1"));

        M5.Lcd.print(F("L1\r\n"));
        ROW++;
      }
      if (PS3.getButtonClick(L3)) {
        Serial.print(F("\r\nL3"));

        M5.Lcd.print(F("L3\r\n"));
        ROW++;
      }
      if (PS3.getButtonClick(R1)) {
        Serial.print(F("\r\nR1"));

        M5.Lcd.print(F("R1\r\n"));
        ROW++;
      }
      if (PS3.getButtonClick(R3)) {
        Serial.print(F("\r\nR3"));

        M5.Lcd.print(F("R3\r\n"));
        ROW++;
      }

      if (PS3.getButtonClick(SELECT)) {
        Serial.print(F("\r\nSelect - "));

        M5.Lcd.print(F("Select - \r\n"));
        ROW++;
        PS3.printStatusString();
      }
      if (PS3.getButtonClick(START)) {
        Serial.print(F("\r\nStart"));

        M5.Lcd.print(F("Start\r\n"));
        ROW++;
        printAngle = !printAngle;
      }
    }
#if 1 // Set this to 1 in order to see the angle of the controller
    if (printAngle) {
      AnglePitch = PS3.getAngle(Pitch);
      AngleRoll = PS3.getAngle(Roll);

      SkipCount3++;

      if(SkipCount3 > 1000){
        Serial.print(F("\r\nPitch: "));
        Serial.print(AnglePitch);
        Serial.print(F("\tRoll: "));
        Serial.print(AngleRoll);

        M5.Lcd.print(F("Pitch: "));
        M5.Lcd.print(AnglePitch);
        M5.Lcd.print(F(" Roll: "));
        M5.Lcd.print(AngleRoll);
        M5.Lcd.print(F("\r\n"));
        ROW++;
        SkipCount3 = 0;
      }
    }
#endif
  }

  if(ROW >28){
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.fillScreen(BLACK);
    ROW = 0;
  }
}

void startupLogo() {
    static uint8_t brightness, pre_brightness;
    M5.Lcd.setBrightness(0);
    M5.Lcd.pushImage(0, 0, 320, 240, (uint16_t *)gImage_logoM5);
    for(int i=0; i<256; i++) {
        delayMicroseconds(40);
        brightness = i;
        if(pre_brightness != brightness) {
            pre_brightness = brightness;
            M5.Lcd.setBrightness(brightness);
        }
    }

    for(int i=255; i>=0; i--) {
        M5.Lcd.setBrightness(i);
        delay(2);
    }

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setBrightness(100);
}
