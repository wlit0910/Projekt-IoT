#include <WiFi.h>
#include <WiFiServer.h>
#include <Wire.h>
#include <U8g2lib.h>

// Create a custom Wire instance on different pins
TwoWire myWire(i2c1, 4, 5); // SDA = GP4, SCL = GP5

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 5, /* data=*/ 4);

const char* ssid = "ssid";
const char* password = "haslo";


WiFiServer server(5000); // TCP server on port 5000
const int ledPin = LED_BUILTIN;
const int auxPin = 2;
const int sensorPin = 26;  // GPIO26 = ADC0
bool autoMode = false;
bool auxState = false;
unsigned long auxOnTimestamp = 0;

void connectToWiFi()
{
    while (true)
    {
        Serial.println("Connecting to WiFi");

        for (int i = 0; i < 4; i++)
        {
            digitalWrite(ledPin, HIGH);
            delay(250);
            digitalWrite(ledPin, LOW);
            delay(250);
        }

        WiFi.begin(ssid, password);
        delay(1000); // Give it a moment

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("\nConnected!");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
            digitalWrite(ledPin, HIGH);
            break; // Exit loop
        }

        Serial.println("\nFailed to connect. Retrying in 20 seconds...");
        // Blink for 20 seconds
        unsigned long blinkStart = millis();
        while (millis() - blinkStart < 20000)
        {
            digitalWrite(ledPin, HIGH);
            delay(250);
            digitalWrite(ledPin, LOW);
            delay(250);
        }       
    }
}

void setup()
{
    Serial.begin(9600);
    analogReadResolution(12);
    pinMode(ledPin, OUTPUT);
    pinMode(auxPin, OUTPUT);
    digitalWrite(auxPin, HIGH);
    myWire.begin();
    u8g2.begin();

    connectToWiFi();
    server.begin();
    Serial.println("TCP server started.");
}

void displayTemp() {
  int raw = analogRead(sensorPin);  // 0–4095
  float voltage = (raw / 4095.0) * 3.3;
  float temperatureC = voltage * 100.0;

  // --- AUTO_MODE logic ---
  if (autoMode) {
    if (temperatureC >= 30.0) {
      if (!auxState) {
        digitalWrite(auxPin, HIGH);
        auxState = true;
        auxOnTimestamp = millis();
      }
    } else {
      if (auxState && (millis() - auxOnTimestamp >= 30000)) {
        digitalWrite(auxPin, LOW);
        auxState = false;
      }
    }
  }

  // --- OLED display ---
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 20, "Temperature:");
  char tempStr[16];
  snprintf(tempStr, sizeof(tempStr), "%.2f deg C", temperatureC);
  u8g2.drawStr(0, 40, tempStr);

  // Show mode status
  if (autoMode) {
    u8g2.drawStr(0, 60, "AUTO_MODE ON");
  } else {
    u8g2.drawStr(0, 60, "AUTO_MODE OFF");
  }

  u8g2.sendBuffer();
}

void loop()
{
    // Check WiFi connection status
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi disconnected. Attempting to reconnect...");
        connectToWiFi();
    }
    displayTemp();
    WiFiClient client = server.available();
    if (client)
    {
        Serial.println("Client connected.");
        while (client.connected())
        {
            displayTemp();
            if (client.available())
            {
                String command = client.readStringUntil('\n');
                command.trim();
                Serial.print("Received: ");
                Serial.println(command);

                client.print("Pico message: ");

                if (command == "ON") {
                    autoMode = false; // disable auto mode
                    digitalWrite(auxPin, HIGH);
                    auxState = true;
                    client.print("ON\n");
                }
                else if (command == "OFF") {
                    autoMode = false; // disable auto mode
                    digitalWrite(auxPin, LOW);
                    auxState = false;
                    client.print("OFF\n");
                }
                else if (command == "AUTO_MODE") {
                    autoMode = !autoMode; // toggle
                    client.print(autoMode ? "AUTO_MODE enabled\n" : "AUTO_MODE disabled\n");
                }
                else if (command == "TEMP")
                {
                    int raw = analogRead(sensorPin);  // 0–4095
                    Serial.println(raw);
                    float voltage = (raw / 4095.0) * 3.3;  // Convert to voltage
                    float temperatureC = voltage * 100.0;  // LM35: 10mV/°C
                    client.print("Temperature: ");
                    client.print(temperatureC);
                    client.print(" deg C\n");
                }
                else
                {
                    client.print("Unknown command\n");
                }
            }
        }
        client.stop();
        Serial.println("Client disconnected.");
    }
}