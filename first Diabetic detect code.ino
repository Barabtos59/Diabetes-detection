#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Pin definitions
const int sensorPin = A0; // Analog pin connected to TGS 1820 sensor output

#define DHTPIN 2 
#define DHTTYPE DHT11 

LiquidCrystal_I2C lcd(0x27, 16, 2); 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    
    lcd.begin();
    lcd.backlight(); 
    lcd.setCursor(0, 0);
    lcd.print("Acetone & Temp");
    
    dht.begin();

    delay(2000);
}

void loop() {
    int sensorValue = analogRead(sensorPin);
    float voltage = sensorValue * (5.0 / 1023.0);  // Convert to voltage

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        delay(1000);
        return;
    }

    Serial.print("Raw Value: ");
    Serial.print(sensorValue);
    Serial.print(" | Voltage: ");
    Serial.println(voltage);

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("% | Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");

    lcd.setCursor(0, 1);
    lcd.print("A:");
    lcd.print(sensorValue);
    lcd.print(" V:");
    lcd.print(voltage, 2);
    lcd.print("   "); // Clear trailing characters

    delay(1000);
}
