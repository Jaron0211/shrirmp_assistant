#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define WL_P A0 //water level sensor pin
#define BUT 2   //button pin
#define BEEP 3  //beeper pin

SoftwareSerial wifi(10, 9);
LiquidCrystal_I2C lcd(0x27, 20, 4);

//waring
bool WARING = false;
unsigned long waring_timer, beep_timer, refrash_timer, buffer_timer, waring_pause_timer, eeprom_write_timer, wifi_timer;

//keeping
unsigned long feed_timer;
int feed_counter[4] = {0, 0, 0, 0} ; //day,hour,min,seconds
boolean feeding_reminder = false;

//gui
int select = 0;
int page = 0;
unsigned long button_timer ;

//sensor
int level;

void setup() {
  Serial.begin(9600);
  wifi.begin(115200);

  lcd.init();
  lcd.backlight();

  pinMode(WL_P, INPUT);
  pinMode(BUT, INPUT);
  pinMode(BEEP, OUTPUT);
  digitalWrite(BEEP, LOW);

  waring_timer = millis();
  feed_timer = millis();
  refrash_timer = millis();
  button_timer = millis();
  eeprom_write_timer = millis();
  wifi_timer = 0;
  waring_pause_timer = 0;

  EEPROM.get(0, feed_counter);
  digitalWrite(BEEP, HIGH);
  delay(100);
  digitalWrite(BEEP, LOW);
}

void loop() {

  if (millis() - refrash_timer > 5000) {
    TWCR = 0;
    delay(50);
    Wire.begin();
    delay(50);
    lcd.clear();
    refrash_timer = millis();
  }


  //FUNC
  get_water_level_waring();
  feedtime_counting();
  feed_time_over24h();

  //GUI
  if (!WARING) {
    normal_menu();
  } else {
    WARING_SYS();
  }

  if (millis() - eeprom_write_timer > 1800000) {
    EEPROM.put(0, feed_counter);
  }
  Serial_RX();
}

//gui
void normal_menu() {
  if (page == 0) {
    lcd.setCursor(0,1);
    lcd.print("LineID:");
    lcd.print("@944eyjso");
    lcd.setCursor(0, 0);
    lcd.print("Feed ");

    for (int i = 0; i < 4; i++) {
      if (feed_counter[i] > 0) {
        lcd.print(feed_counter[i]);
        switch (i) {
          case 0:
            lcd.print(" Day");
            break;
          case 1:
            lcd.print(" Hour");
            break;
          case 2:
            lcd.print(" Min");
            break;
          case 3:
            lcd.print(" Sec");
            break;
        }

        lcd.print(" ago");
        break;
      }
    }

    if (digitalRead(BUT) == HIGH ) {
      button_timer = millis();
    } else {
      if (millis() - button_timer > 50) {
        if (millis() - buffer_timer > 5000) {
          digitalWrite(BEEP, HIGH);
          delay(50);
          digitalWrite(BEEP, LOW);
          wifi.write("feed");
          feedtime_reset();
          buffer_timer = millis();
        }
      }
    }
  } else if (page == 1) {

    if (digitalRead(BUT) == HIGH ) {
      button_timer = millis();
    } else {
      if (millis() - button_timer > 50) {
        if (millis() - buffer_timer > 5000) {
          feedtime_reset();
          buffer_timer = millis();
        }
      }
    }

  }

}

void WARING_SYS() {

  lcd.setCursor(0, 0);
  lcd.print("    WARING!     ");
  lcd.setCursor(0, 1);
  lcd.print("                ");

  if (millis() - wifi_timer > 4 * 60 * 60 * 1000) {
    wifi.write("wl");
    wifi_timer = millis();
  }

  if (millis() - beep_timer < 500) {
    lcd.noBacklight();
    digitalWrite(BEEP, LOW);
  } else if (millis() - beep_timer < 1000) {
    lcd.backlight();
    digitalWrite(BEEP, HIGH);
  } else {
    beep_timer = millis();
  }

  if (digitalRead(BUT) == HIGH ) {
    button_timer = millis();
  } else {
    if (millis() - button_timer > 50) {
      lcd.backlight();
      WARING = false;
      buffer_timer = millis();
      waring_pause_timer = millis();
      digitalWrite(BEEP, LOW);
    }
  }
}

//func
void feed_time_over24h(){
  if( feed_counter[0] > 0 and feeding_reminder == false){
    Serial.println("Over 24h no eating");
    wifi.write("24h");
    feeding_reminder = true;
  }
}

void get_water_level_waring() {
  level = analogRead(WL_P);

  if (level < 650) {
    waring_timer = millis();
  }

  if (millis() - waring_timer > 3000 and millis() - waring_pause_timer > 60000) {
    WARING = true;
  }
}

void feedtime_counting() {

  if (millis() - feed_timer >= 1000) {
    feed_counter[3] += 1;
    feed_timer = millis();
  }
  if (feed_counter[3] == 60) {
    feed_counter[3] = 0;
    feed_counter[2] += 1;
  }
  if (feed_counter[2] == 60) {
    feed_counter[2] = 0;
    feed_counter[1] += 1;
  }
  if (feed_counter[1] == 24) {
    feed_counter[1] = 0;
    feed_counter[0] += 1;
  }

}

void feedtime_reset() {
  for (int i = 0; i < 4; i++) {
    feed_counter[i] = 0;
  }
  feeding_reminder = false;
  feed_timer = millis();
}

void Serial_RX() {
  if (Serial.available()) {
    String DATA = Serial.readString();
    Serial.println("got date!");

    int MARK1 = 1;
    int MARK2 = 0;
    int i = 0;
    while (1) {
      MARK2 = DATA.indexOf(",", MARK1);
      if (MARK2 == -1)break;
      String t = DATA.substring(MARK1, MARK2);
      feed_counter[i] = t.toInt();
      MARK1 = MARK2 + 1;
      EEPROM.put(0, feed_counter);
      i++;
      if (i == 4)break;
    }
  }
}
