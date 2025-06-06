#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Pin definitions
const int sensorPin = A0;          // Analog pin connected to TGS 1820 sensor output
const int switchPin = 3;           // Digital pin connected to the 2-pin micro switch

#define DHTPIN 2 
#define DHTTYPE DHT11 

LiquidCrystal_I2C lcd(0x27, 16, 2); 

DHT dht(DHTPIN, DHTTYPE);

unsigned long displayStartTime = 0;
const unsigned long displayDuration = 10000; // 10 seconds
bool displayActive = false;

void setup() {
    Serial.begin(9600);
    
    pinMode(switchPin, INPUT_PULLUP); // Use internal pull-up resistor
    
    lcd.begin();
    lcd.backlight(); 
    lcd.setCursor(0, 0);
    lcd.print("Acetone & Temp");
    
    dht.begin();

    delay(2000);
}

void loop() {
    // Read the micro switch state (active LOW when pressed)
    bool switchPressed = (digitalRead(switchPin) == LOW);

    if (switchPressed && !displayActive) {
        // Start displaying values for 10 seconds
        displayActive = true;
        displayStartTime = millis();
    }

    if (displayActive) {
        int sensorValue = analogRead(sensorPin);
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();

        if (isnan(humidity) || isnan(temperature)) {
            Serial.println("Failed to read from DHT sensor!");
            delay(1000);
            return;
        }

        Serial.print("Raw Value: ");
        Serial.print(sensorValue);
        Serial.print(" | Humidity: ");
        Serial.print(humidity);
        Serial.print("% | Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");

        lcd.setCursor(0, 1);
        lcd.print("A:");
        lcd.print(sensorValue);
        lcd.print(" H:");
        lcd.print(humidity, 0);
        lcd.print("% T:");
        lcd.print(temperature, 1);
        lcd.print("C  "); // Extra spaces to clear leftover chars

        // Check if 10 seconds have elapsed
        if (millis() - displayStartTime >= displayDuration) {
            displayActive = false;
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Acetone & Temp");
        }
    }
}
