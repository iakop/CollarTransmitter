#include <CollarTransmitter.h>
#include <Bounce2.h>
#include <LiquidCrystal.h>

#define RS 3
#define E 4
#define D4 5
#define D5 6
#define D6 7 
#define D7 8
#define OUTPUT_PIN       11
#define BTN_PIN          2

CollarTransmitter transmitter(0x6490);
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
Bounce button = Bounce();
const char *msg = "Push to hack!";

unsigned long long millisTimer = 0;

void setup() {
  transmitter.attach(OUTPUT_PIN);
  button.attach (BTN_PIN, INPUT);
  button.interval(25);
  
  lcd.begin(16, 2);
  lcd.setCursor(0 /*x*/ , 0 /*y*/); 
  lcd.print(msg);
}

void loop() {
  button.update();
  //Serial.println(button.rose());
  if(button.rose()){
    millisTimer = millis();
    lcd.setCursor(0 /*x*/ , 1 /*y*/);
    lcd.print("Hacking...");
    delay(1000);
    while(millis() < millisTimer + 2000){
      transmitter.blockingSend(1, ZAP_MODE, 15);
      delay(20);
    }
    lcd.clear();
    lcd.setCursor(0 /*x*/ , 0 /*y*/); 
    lcd.print(msg);
  }
}
