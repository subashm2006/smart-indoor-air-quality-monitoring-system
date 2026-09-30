#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

/* WIFI */
const char* ssid = "Enter Wifi name here";
const char* password = "Enter Password here";

/* THINGSPEAK */
unsigned long channelID = 3289317;
const char* writeAPIKey = "Enter ThingSpeak Write API Key here";

WiFiClient client;

/* SENSOR PINS */
#define DHTPIN D4
#define DHTTYPE DHT11
#define gasSensor A0
#define buzzer D5

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int threshold = 150;

void setup()
{
  Serial.begin(115200);

  pinMode(buzzer, OUTPUT);

  Wire.begin(D2, D1);

  lcd.init();
  lcd.backlight();

  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("SMART AQI SYSTEM");

  lcd.setCursor(0, 1);
  lcd.print("Starting...");

  delay(3000);

  /* WIFI */
  lcd.clear();
  lcd.print("Connecting WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");

  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  delay(2000);

  ThingSpeak.begin(client);
}

void loop()
{
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  int gasValue = analogRead(gasSensor);

  Serial.print("Temp: ");
  Serial.print(temp);

  Serial.print(" Hum: ");
  Serial.print(hum);

  Serial.print(" Gas: ");
  Serial.println(gasValue);

  /* BUZZER */
  if (gasValue > threshold)
    digitalWrite(buzzer, HIGH);
  else
    digitalWrite(buzzer, LOW);

  /* LCD TEMP + HUM */
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temp);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Hum:");
  lcd.print(hum);
  lcd.print("%");

  delay(3000);

  /* LCD GAS */
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Gas Value:");

  lcd.setCursor(0, 1);
  lcd.print(gasValue);

  delay(3000);

  /* LCD STATUS */
  lcd.clear();

  if (gasValue > threshold)
  {
    lcd.setCursor(0, 0);
    lcd.print("GAS DETECTED!");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Air Quality");

    lcd.setCursor(0, 1);
    lcd.print("SAFE :)");
  }

  delay(2000);  // shorter time

  /* THINGSPEAK */
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hum);
  ThingSpeak.setField(3, gasValue);

  ThingSpeak.writeFields(channelID, writeAPIKey);

  delay(15000);
}
