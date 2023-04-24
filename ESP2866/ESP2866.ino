#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char *ssid = "HD FIBRA TP LINK";
const char *password = "14042001";

IPAddress server(192, 168, 3, 100); // Insira o endereço IP do Arduino Uno aqui

void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
    }
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFiClient client;
        if (client.connect(server, 115200))
        {
            if (digitalRead(LED_BUILTIN))
            {
                client.print("1");
            }
            else
            {
                client.print("0");
            }
            delay(1000);
        }
        client.stop();
    }
}
