
//Include the library files
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//Initialize the LCD display
// LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "Q5gplj5_zkZuBf9EiBstOJAwoKLtNfL_";
char ssid[] = "farmers-aid";
char pass[] = "test1234";

BlynkTimer timer;
bool Relay = 0;

//Define component pins
#define sensor A0
#define waterPump D3

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, LOW);
  // lcd.begin();
  // lcd.backlight();

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);




  //Call the function
  timer.setInterval(100L, soilMoistureSensor);
}

//Get the button value
BLYNK_WRITE(V1) {
  Relay = param.asInt();

  if (Relay == 1) {
    digitalWrite(waterPump, HIGH);
    // lcd.setCursor(0, 1);
    // lcd.print("Motor is ON ");
  } else {
    digitalWrite(waterPump, LOW);
    // lcd.setCursor(0, 1);
    // lcd.print("Motor is OFF");
  }
}

//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V0, value);
  // lcd.setCursor(0, 0);
  // lcd.print("Moisture :");
  // lcd.print(value);
  // lcd.print(" ");

}

void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}