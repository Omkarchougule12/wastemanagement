#define BLYNK_TEMPLATE_ID "TMPL35TOJ0GGn"
#define BLYNK_TEMPLATE_NAME "waste management"
#define BLYNK_AUTH_TOKEN "P22JdRLnvdQ2_CXqBs9kp_irnRMBgM0S"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <ESP32Servo.h>
Servo myservo;
long duration1, distance1 = 0, duration2, distance2 = 0;
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "network"; // Your Wifi Name
char pass[] = "1122334455"; // Your Wifi Password

#define trigPin1 16
#define echoPin1 17
#define trigPin2 18
#define echoPin2 19
#define IR 13
#define buzz 4
#define servo 14
#define plate 23

BlynkTimer timer;
int IRdata, platedata;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Waste management");
  delay(1000);
  lcd.clear();
  lcd.print("Searching WIFI");

  Blynk.begin(auth, ssid, pass);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(plate, INPUT);
  pinMode(IR, INPUT);
  pinMode(buzz, OUTPUT);

  lcd.clear();
  lcd.print("Connected");
  delay(500);

  digitalWrite(buzz, HIGH);
  delay(500);
  digitalWrite(buzz, LOW);
  myservo.attach(servo);
  timer.setInterval(2000L, sendUltrasonicData);
  myservo.write(90);
}

void loop() {
  readultra();
  Blynk.run();
  timer.run();
  //SERVO MOTOR
  IRdata = digitalRead(IR);
  if (IRdata == 0)
  {
    //tune();
    platedata = digitalRead(plate);
    {
      if (platedata == 1)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WET");
        myservo.write(0);
        delay(1000);
        myservo.write(90);
        delay(2000);
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("DRY");
        myservo.write(180);
        delay(1000);
        myservo.write(90);
        delay(2000);
      }
    }
  }

  delay(1000);
}

//ULTRASONIC 
void readultra()
{
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1 * 0.0343) / 2;

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2 * 0.0343) / 2;

  Serial.print("DRY: ");
  Serial.print(distance1);
  Serial.print(" cm");
  Serial.print("  WET: ");
  Serial.print(distance2);
  Serial.println(" cm");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DRY: ");
  lcd.print(distance1);
  lcd.print(" cm");
  lcd.setCursor(0, 1);
  lcd.print("WET: ");
  lcd.print(distance2);
  lcd.print(" cm");
  if (distance1 < 10)
  {
    tune();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WET storage");
    lcd.setCursor(0, 1);
    lcd.print("is full");
  }
  else if (distance2 < 10)
  {
    tune();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DRY storage");
    lcd.setCursor(0, 1);
    lcd.print("is full");
  }
}
//BLYNK
void sendUltrasonicData() {

  Blynk.virtualWrite(V1, distance1);
  Blynk.virtualWrite(V2, distance2);

}

void tune() {
  digitalWrite(buzz, HIGH);
  delay(100);
  digitalWrite(buzz, LOW);
  delay(50);
  digitalWrite(buzz, HIGH);
  delay(100);
  digitalWrite(buzz, LOW);
}
