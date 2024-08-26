/* 
This code is written by CodeBuzz (Advait Muley) for the project FarmersAid
For any queries, contact me on github.com/codebuzz-ml
*/

//Include Libraries
  #include <LiquidCrystal_I2C.h>
  #include <ESP8266WiFi.h>
  #include <BlynkSimpleEsp8266.h>
  #include <DHT.h>
//Define Blynk Information
  #define BLYNK_TEMPLATE_ID           "TMPL3DENDY2hV"
  #define BLYNK_TEMPLATE_NAME         "FarmersAid"
  #define BLYNK_AUTH_TOKEN "uTOrEfrm_PgXidnbe9394SNdT-t-vMYQ"

//Define WiFI SSID + Passwd
  char ssid[] = "CodeBuzzWiFi_2.4Ghz";
  char pass[] = "Mangesh1812";


//Create an objcet for LCD, DHT and Timer
  LiquidCrystal_I2C lcd(0x27, 16, 2);
  DHT dht(D4, DHT11);
  BlynkTimer Timer;

//Define Component Pins
  #define soil A0

//Call a function to check the state of the push button connected
void checkPhysicalButton();


int relayState = LOW; 
int pushButtonState = HIGH;

#define relayPin D3
#define pushButton D7
#define virtualPushButton V12



void setup() {
  //Begin Commands
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  dht.begin();
  
  lcd.begin();
  lcd.backlight();

  //PinMode Commands
  pinMode(relayPin,OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);

  //Digitalwrite Commands
  digitalWrite(relayPin, relayState);

  //LCD Commands to add a Progress Bar
  lcd.setCursor(0, 0);
  lcd.print("  Initializing  ");
  for (int a = 5; a<= 10; a++)
  {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(500);
  }

  //Call the Functions
  
}

void loop() {
  if (relayState == HIGH)
  {
    lcd.setCursor(11, 1);
    lcd.print("W:ON ");
  }
  else if (relayState == LOW)
  {
    lcd.setCursor(11, 1);
    lcd.print("W:OFF");
  }

  Blynk.run();
  Timer.run();

}

//Custom Functions Definition

//DHT Sensor

void DHTsensor()
{
  //Store the values in local variables
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  //If the value is not readable, err then return
  if (isnan(h) || isnan(t)) 
  {
  Serial.println("Failed to read from DHT sensor!");
  return;
  }

  //Upload Value to Blynk.Cloud and Upate on LCD Display
    //Upload to Blynk
      Blynk.virtualWrite(V0,t);
      Blynk.virtualWrite(V1,h);
    //Update LCD Display
      lcd.setCursor(0, 0);
      lcd.print("T:");
      lcd.print(t);

      lcd.setCursor(8, 0);
      lcd.print("H:");
      lcd.print(h);
}


//Soil Moisture

void soilMoistureSensor()
{
  //Assign a Variable to the analog value
  int value = analogRead(soil);

  //Convert the ANalog Value to %age
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100)* - 1;

  //Upload the Value to Blynk cloud
  Blynk.virtualWrite(V3, value);
}

//Fetch and Sync Cloud
BLYNK_CONNECTED()
{
  //Request the latest state of the button
  Blynk.syncVirtual(virtualPushButton);
}

BLYNK_WRITE(virtualPushButton)
{
  relayState = param.asInt();
  digitalWrite(relayPin, relayState);
}

//Check Physcial Button State and Sync with Virtual
void checkPhysicalButton()
{
  if (digitalRead(pushButton) == LOW) {
    // pushButtonState is used to avoid sequential toggles
    if (pushButtonState != LOW) {

      // Toggle Relay state
      relayState = !relayState;
      digitalWrite(relayPin, relayState);

      // Update Button Widget
      Blynk.virtualWrite(virtualPushButton, relayState);
    }
    pushButtonState = LOW;
  } else {
    pushButtonState = HIGH;
  }
}