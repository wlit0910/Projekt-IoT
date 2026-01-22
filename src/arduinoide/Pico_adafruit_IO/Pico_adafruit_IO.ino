#include <WiFi.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "AdafruitIO_WiFi.h"
#include <LittleFS.h>


// --- Default AP credentials ---
const char* apSSID     = "picowifi";
const char* apPassword = "picoadmin";

#define IO_USERNAME  ""
#define IO_KEY       ""

// --- Stored WiFi credentials ---
String wifiSSID;
String wifiPassword;

// --- Adafruit IO object (initialized later after WiFi is known) ---
AdafruitIO_WiFi *io;

// --- Feeds ---
AdafruitIO_Feed *temperatureFeed;
AdafruitIO_Feed *relayFeed;
AdafruitIO_Feed *messageFeed;

// --- OLED setup ---
TwoWire myWire(i2c1, 4, 5); // SDA=GP4, SCL=GP5
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 5, 4);

// --- Pins ---
const int ledPin = LED_BUILTIN;
const int auxPin = 2;
const int sensorPin = 26;  // ADC0

// --- States ---
bool autoMode = false;
bool auxState = false;
unsigned long auxOnTimestamp = 0;
String lastMessage = "";

// --- TCP/UDP ---
WiFiServer server(5000);
WiFiUDP udp;
const int DISCOVERY_PORT = 5000;

// --- Timing ---
unsigned long lastTempUpdate = 0;
const unsigned long tempInterval = 5000; // 5 seconds

// --- Save WiFi credentials ---
void saveWiFiCredentials(const String& ssid, const String& pass)
{
    File f = LittleFS.open("/wifi.txt", "w");
    if (f)
    {
        f.println(ssid);
        f.println(pass);
        f.close();
        Serial.println("WiFi credentials saved.");
    }
}

// --- Load WiFi credentials ---
bool loadWiFiCredentials()
{
    if (!LittleFS.exists("/wifi.txt"))
    {
        Serial.println("No stored WiFi credentials.");
        return false;
    }

    File f = LittleFS.open("/wifi.txt", "r");
    if (!f) return false;

    wifiSSID = f.readStringUntil('\n');
    wifiSSID.trim();
    wifiPassword = f.readStringUntil('\n');
    wifiPassword.trim();
    f.close();

    Serial.printf("Loaded WiFi: %s\n", wifiSSID.c_str());
    return true;
}


// --- Stored Adafruit IO credentials ---
String aioUser;
String aioKey;

// --- Save AIO credentials ---
void saveAioCredentials(const String& user, const String& key)
{
    File f = LittleFS.open("/aio.txt", "w");
    if (f)
    {
        f.println(user);
        f.println(key);
        f.close();
        Serial.println("Adafruit IO credentials saved.");
    }
}

// --- Load AIO credentials ---
bool loadAioCredentials()
{
    if (!LittleFS.exists("/aio.txt"))
    {
        Serial.println("No stored Adafruit IO credentials.");
        return false;
    }

    File f = LittleFS.open("/aio.txt", "r");
    if (!f) return false;

    aioUser = f.readStringUntil('\n');
    aioUser.trim();
    aioKey = f.readStringUntil('\n');
    aioKey.trim();
    f.close();

    Serial.printf("Loaded AIO user: %s\n", aioUser.c_str());
    return true;
}

// --- WiFi connect loop ---
bool connectToWiFi()
{
    if (wifiSSID == "" || wifiPassword == "")
    {
        return false;
    }

    Serial.printf("Connecting to WiFi: %s\n", wifiSSID.c_str());
    WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());

    unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 10000)
    {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\nConnected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        digitalWrite(ledPin, HIGH);
        return true;
    }
    else
    {
        Serial.println("\nFailed to connect.");
        return false;
    }
}

// --- Start AP ---
void startAP()
{
    Serial.println("Starting Access Point...");
    WiFi.softAP(apSSID, apPassword);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}

// --- OLED display ---
void displayTemp(float temperatureC)
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);

    char tempStr[20];
    snprintf(tempStr, sizeof(tempStr), "Temp: %.2f C", temperatureC);
    u8g2.drawStr(0, 20, tempStr);

    if (autoMode)
    {
        u8g2.drawStr(0, 40, "AUTO_MODE ON");
    }
    else
    {
        u8g2.drawStr(0, 40, "AUTO_MODE OFF");
    }

    if (lastMessage.length() > 0)
    {
        u8g2.drawStr(0, 60, lastMessage.c_str());
    }

    u8g2.sendBuffer();
}

// --- UDP discovery ---
void handleDiscovery()
{
    int packetSize = udp.parsePacket();
    if (packetSize > 0)
    {
        char buf[64];
        int len = udp.read(buf, sizeof(buf) - 1);
        if (len > 0) buf[len] = '\0';

        if (strcmp(buf, "DISCOVER_PICO") == 0)
        {
            udp.beginPacket(udp.remoteIP(), udp.remotePort());
            udp.print("PICO2W:");
            udp.print(WiFi.localIP());
            udp.endPacket();
        }
    }
}

// --- Handlers for Adafruit IO ---
void handleRelay(AdafruitIO_Data *data)
{
    bool state = data->toBool();
    digitalWrite(auxPin, state ? HIGH : LOW);
    auxState = state;
    lastMessage = state ? "Relay ON" : "Relay OFF";
}

void handleMessage(AdafruitIO_Data *data)
{
    lastMessage = data->toString();
}

// --- Setup ---
void setup()
{
    Serial.begin(9600);
    analogReadResolution(12);
    pinMode(ledPin, OUTPUT);
    pinMode(auxPin, OUTPUT);
    digitalWrite(auxPin, LOW);

    myWire.begin();
    u8g2.begin();

    if (!LittleFS.begin())
    {
        Serial.println("LittleFS mount failed!");
    }

    bool connected = false;
    if (loadWiFiCredentials())
    {
        connected = connectToWiFi();
    }
    if (!connected)
    {
        startAP();
    }

    // Load Adafruit IO credentials
    if (!loadAioCredentials())
    {
        aioUser = IO_USERNAME; // fallback defaults
        aioKey  = IO_KEY;
    }

    server.begin();
    udp.begin(DISCOVERY_PORT);

    // Initialize Adafruit IO only if WiFi is connected
    if (WiFi.status() == WL_CONNECTED)
    {
        io = new AdafruitIO_WiFi(aioUser.c_str(), aioKey.c_str(),
                                 wifiSSID.c_str(), wifiPassword.c_str());

        relayFeed = io->feed("relay");
        messageFeed = io->feed("messages");
        temperatureFeed = io->feed("temperature");

        io->connect();
        relayFeed->onMessage(handleRelay);
        messageFeed->onMessage(handleMessage);

        while (io->status() < AIO_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println("Connected to Adafruit IO!");
    }
}

// --- Loop ---
void loop()
{
    if (io) io->run();

    if (WiFi.status() != WL_CONNECTED)
    {
        connectToWiFi();
    }

    handleDiscovery();

    WiFiClient client = server.available();
    if (client)
    {
        while (client.connected())
        {
            if (client.available())
            {
                String command = client.readStringUntil('\n');
                command.trim();

                if (command == "ON")
                {
                    autoMode = false;
                    digitalWrite(auxPin, HIGH);
                    auxState = true;
                    client.print("Relay ON\n");
                }
                else if (command == "OFF")
                {
                    autoMode = false;
                    digitalWrite(auxPin, LOW);
                    auxState = false;
                    client.print("Relay OFF\n");
                }
                else if (command == "AUTO_MODE")
                {
                    autoMode = !autoMode;
                    client.print(autoMode ? "AUTO_MODE enabled\n" : "AUTO_MODE disabled\n");
                }
                else if (command == "TEMP")
                {
                    int raw = analogRead(sensorPin);
                    float voltage = (raw / 4095.0) * 3.3;
                    float temperatureC = voltage * 100.0;
                    client.print("Temperature: ");
                    client.print(temperatureC);
                    client.print(" C\n");
                }
                else if (command.startsWith("SETWIFI"))
                {
                    int sep = command.indexOf(':');
                    if (sep > 0)
                    {
                        String creds = command.substring(sep + 1);
                        int comma = creds.indexOf(',');
                        if (comma > 0)
                        {
                            wifiSSID     = creds.substring(0, comma);
                            wifiPassword = creds.substring(comma + 1);

                            saveWiFiCredentials(wifiSSID, wifiPassword);

                            client.println("WiFi credentials saved. Rebooting...");
                            delay(1000);
                            rp2040.reboot();
                        }
                    }
                }
                else if (command.startsWith("SETAIO"))
                {
                   int sep = command.indexOf(':');
                   if (sep > 0)
                   {
                       String creds = command.substring(sep + 1);
                       int comma = creds.indexOf(',');
                       if (comma > 0)
                       {
                           aioUser = creds.substring(0, comma);
                           aioKey  = creds.substring(comma + 1);
                
                           saveAioCredentials(aioUser, aioKey);
                
                           client.println("Adafruit IO credentials saved. Rebooting...");
                           delay(1000);
                           rp2040.reboot();
                       }
                   }
                }
                else
                {
                    client.print("Unknown command\n");
                }
            }
        }
        client.stop();
    }

    unsigned long now = millis();
    if (io && now - lastTempUpdate >= tempInterval)
    {
        lastTempUpdate = now;

        int raw = analogRead(sensorPin);
        float voltage = (raw / 4095.0) * 3.3;
        float temperatureC = voltage * 100.0;

        temperatureFeed->save(temperatureC);
        displayTemp(temperatureC);
    }
}