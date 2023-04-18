#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Define as credenciais da rede Wi-Fi
const char *ssid = "HD FIBRA TP LINK";
const char *password = "14042001";

// Define o número do pino do LED
const int ledPin = 13;

ESP8266WebServer server(80);

void setup()
{
    pinMode(ledPin, OUTPUT);

    // Inicia a comunicação serial
    Serial.begin(115200);

    // Conecta-se à rede Wi-Fi
    WiFi.begin(ssid, password);
    Serial.println("");

    // Espera pela conexão à rede Wi-Fi
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println("Endereço IP: " + WiFi.localIP().toString());

    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();
}

void loop()
{
    server.handleClient();
}

void handleRoot()
{
    // Verifica se foi enviado um pedido POST com o parâmetro "light"
    if (server.method() == HTTP_POST && server.hasArg("light"))
    {
        String lightValue = server.arg("light");

        // Se o valor do parâmetro "light" for "true", liga o LED
        if (lightValue == "true")
        {
            digitalWrite(ledPin, HIGH);
        }
        // Se o valor do parâmetro "light" for "false", desliga o LED
        else if (lightValue == "false")
        {
            digitalWrite(ledPin, LOW);
        }

        // Adiciona o cabeçalho Access-Control-Allow-Origin à resposta
        server.sendHeader("Access-Control-Allow-Origin", "*");

        // Envia uma resposta ao cliente
        server.send(200, "text/plain", "OK");
    }
    // Se não foi enviado um pedido POST com o parâmetro "light", retorna um erro
    else
    {
        server.send(400, "text/plain", "Bad Request");
    }
}

void handleNotFound()
{
    server.send(404, "text/plain", "Not Found");
}