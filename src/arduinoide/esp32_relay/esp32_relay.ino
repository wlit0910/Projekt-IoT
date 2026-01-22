#include <WiFi.h>
#include <WiFiServer.h>
#include <Preferences.h>
#include "AdafruitIO_WiFi.h"

Preferences prefs;
WiFiServer server(5000);

const char* apSSID     = "espwifi";
const char* apPassword = "espadmin";
const int auxPin = D10;

String wifiSSID;
String wifiPassword;
String aioUser;
String aioKey;

AdafruitIO_WiFi *io;
AdafruitIO_Feed *relayFeed;
AdafruitIO_Feed *temperatureFeed;

float lastTemperature = 0.0f; // store latest value from AIO

// --- Access Point ---
void startAP()
{
    Serial.println("Starting Access Point...");
    WiFi.softAP(apSSID, apPassword);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}

// --- Load WiFi credentials ---
bool loadWiFiCredentials()
{
    wifiSSID     = prefs.getString("ssid", "");
    wifiPassword = prefs.getString("pass", "");

    if (wifiSSID == "" || wifiPassword == "")
    {
        Serial.println("No stored WiFi credentials.");
        Serial.println("Using fallback.");
        wifiSSID = "Black Messa";
        wifiPassword = "kappa008";
        return true;
    }
    return true;
}

// --- Load AIO credentials ---
bool loadAioCredentials()
{
    aioUser = prefs.getString("aioUser", "");
    aioKey  = prefs.getString("aioKey", "");

    if (aioUser == "" || aioKey == "")
    {
        Serial.println("No stored Adafruit IO credentials.");
        return false;
    }
    return true;
}

// --- Connect to WiFi ---
bool connectToStoredWiFi()
{
    if (wifiSSID == "" || wifiPassword == "")
    {
        return false;
    }

    Serial.printf("Connecting to stored WiFi: %s\n", wifiSSID.c_str());
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
        return true;
    }
    else
    {
        Serial.println("\nFailed to connect.");
        return false;
    }
}

// --- Relay handler from Adafruit IO ---
void handleRelay(AdafruitIO_Data *data)
{
    bool state = data->toBool();
    digitalWrite(auxPin, state ? LOW : HIGH); // active LOW relay
    Serial.println(state ? "Relay ON (from AIO)" : "Relay OFF (from AIO)");
}

// --- Temperature handler from Adafruit IO ---
void handleTemperature(AdafruitIO_Data *data)
{
    lastTemperature = data->toFloat();
    Serial.printf("Temperature update from AIO: %.2f C\n", lastTemperature);
}

// --- Setup ---
void setup()
{
    Serial.begin(9600);
    prefs.begin("storage", false);

    pinMode(auxPin, OUTPUT);
    digitalWrite(auxPin, HIGH); // relay off

    bool connected = false;
    if (loadWiFiCredentials())
    {
        connected = connectToStoredWiFi();
    }
    if (!connected)
    {
        startAP();
    }

    // Load Adafruit IO credentials
    if (!loadAioCredentials())
    {
        aioUser = ""; // fallback defaults
        aioKey  = "";
    }

    // Initialize Adafruit IO if WiFi is connected
    if (WiFi.status() == WL_CONNECTED)
    {
        io = new AdafruitIO_WiFi(aioUser.c_str(), aioKey.c_str(),
                                 wifiSSID.c_str(), wifiPassword.c_str());

        relayFeed = io->feed("relay");
        temperatureFeed = io->feed("temperature");

        io->connect();
        relayFeed->onMessage(handleRelay);
        temperatureFeed->onMessage(handleTemperature);

        while (io->status() < AIO_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println("Connected to Adafruit IO!");
    }

    server.begin();
    Serial.println("TCP server started.");
}

// --- Loop ---
void loop()
{
    if (io) io->run();

    WiFiClient client = server.available();
    if (client)
    {
        Serial.println("Client connected.");
        while (client.connected())
        {
            if (client.available())
            {
                String command = client.readStringUntil('\n');
                command.trim();
                Serial.print("Received: ");
                Serial.println(command);

                if (command.startsWith("SETWIFI"))
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

                            prefs.putString("ssid", wifiSSID);
                            prefs.putString("pass", wifiPassword);

                            client.println("WiFi credentials saved. Rebooting...");
                            ESP.restart();
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

                            prefs.putString("aioUser", aioUser);
                            prefs.putString("aioKey", aioKey);

                            client.println("Adafruit IO credentials saved. Rebooting...");
                            ESP.restart();
                        }
                    }
                }
                else if (command == "STATUS")
                {
                    if (WiFi.status() == WL_CONNECTED)
                    {
                        client.printf("Connected to %s, IP: %s\n",
                                      wifiSSID.c_str(),
                                      WiFi.localIP().toString().c_str());
                    }
                    else
                    {
                        client.println("Not connected.");
                    }
                }
                else if (command == "ON")
                {
                    digitalWrite(auxPin, LOW);
                    if (relayFeed) relayFeed->save(true);
                    client.print("ON\n");
                }
                else if (command == "OFF")
                {
                    digitalWrite(auxPin, HIGH);
                    if (relayFeed) relayFeed->save(false);
                    client.print("OFF\n");
                }
                else if (command == "RELAYSTATE")
                {
                    bool state = (digitalRead(auxPin) == LOW);
                    client.printf("Relay is %s\n", state ? "ON" : "OFF");
                }
                else if (command == "TEMP")
                {
                    if (!isnan(lastTemperature))
                    {
                        client.printf("Temperature from AIO: %.2f C\n", lastTemperature);
                    }
                    else
                    {
                        client.println("No temperature data yet.");
                    }
                }
                else
                {
                    client.println("Unknown command.");
                }
            }
        }
        client.stop();
        Serial.println("Client disconnected.");
    }
}