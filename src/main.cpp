#include <WiFi.h>
#include <HTTPClient.h>
#include <base64.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include "secret.h"

// --- WiFi Credentials ---
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD"; 

// --- Twilio Credentials ---
const char* account_sid = "TWILIO_ACCOUNT_SID";
const char* auth_token = "TWILIO_AUTH_TOKEN";
const char* from_number = "FROM_NUMBER"; // Your Twilio number
const char* to_number = "TO_NUMBER";   // Your verified number

// --- Pin Definitions ---
const int redLed = 2;
const int greenLed = 4;
const int buzzer = 16;
const int flamePin = 13;
const int smokePin = 34; // Analog
const int sprinklerPin = 5;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int thresh = 2800; // Adjusted for ESP32 12-bit (0-4095)
bool status = true;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Loading..");
  delay(2000);
  lcd.clear();
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(sprinklerPin, OUTPUT);
  pinMode(flamePin, INPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

void sendTwilioSMS(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.twilio.com/2010-04-01/Accounts/" + String(account_sid) + "/Messages.json";
    
    http.begin(url);
    
    // Auth Header
    String auth = String(account_sid) + ":" + String(auth_token);
    String encodedAuth = base64::encode(auth);
    http.addHeader("Authorization", "Basic " + encodedAuth);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    String postData = "To=" + String(to_number) + "&From=" + String(from_number) + "&Body=" + message;
    
    int httpResponseCode = http.POST(postData);
    lcd.clear();
    if (httpResponseCode == 201) {
      Serial.println("SMS Sent Successfully, Response: " + String(httpResponseCode));
      //lcd.setCursor(0, 0);
      lcd.print("SMS Sent Successfully.");
    } else {
      Serial.println("Error sending SMS: " + String(httpResponseCode));
      //lcd.setCursor(0, 0);
      lcd.print("Error sending SMS");
    }
    delay(500);
    http.end();
  }
}

void loop() {
  int smokeVal = analogRead(smokePin);
  int fireVal = digitalRead(flamePin); 
  
  Serial.print("Smoke: "); Serial.print(smokeVal);
  Serial.print(" | Flame: "); Serial.println(fireVal);

  // --- ALARM CONDITION ---
  // Note: Flame sensors are ACTIVE LOW (0 = Fire)
  if (fireVal == LOW || smokeVal > thresh) {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(sprinklerPin, HIGH);
    tone(buzzer, 1000);

    lcd.setCursor(0, 0);
    lcd.print("ALARM! "); // Spaces at end to clear old text
    lcd.setCursor(0, 1);
    lcd.print("S:"); lcd.print(smokeVal); 
    lcd.print(" F:"); lcd.print(fireVal == LOW ? "DET " : "OK  ");

    if (status) {
      status = false;
      sendTwilioSMS("ALERT! Fire or Smoke detected at Home.");
    }
  } 
  // --- NORMAL CONDITION ---
  else {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(sprinklerPin, LOW);
    noTone(buzzer);
    
    lcd.setCursor(0, 0);
    lcd.print("STATUS: SECURE  ");
    lcd.setCursor(0, 1);
    lcd.print("S:"); lcd.print(smokeVal);
    lcd.print("   F:CLEAN  ");
    
    status = true; 
  }
  delay(500); 
}