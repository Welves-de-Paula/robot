/*
 * Arduino UNO - Bridge Example
 *
 * Este código demonstra como o Arduino UNO deve processar
 * os comandos recebidos do ESP8266 via Serial.
 *
 * Protocolo de Comunicação (JSON):
 *
 * RECEBENDO DO ESP8266:
 * {"protocol":"http","method":"GET","uri":"/api/arduino","body":""}
 * {"protocol":"ws","method":"message","uri":"","body":"comando"}
 *
 * ENVIANDO PARA ESP8266:
 * {"success":true,"data":"resposta"}
 * {"broadcast":true,"data":"mensagem para todos"}
 */

#include <ArduinoJson.h>

// Buffer para comunicação serial
#define SERIAL_BUFFER_SIZE 512
char serialBuffer[SERIAL_BUFFER_SIZE];
uint16_t serialBufferPos = 0;

// Estado do sistema
struct SystemState
{
    int ledState = LOW;
    unsigned long lastBlink = 0;
    int sensorValue = 0;
} systemState;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(A0, INPUT);

    // Aguardar um pouco para estabilizar
    delay(1000);

    Serial.println("{\"status\":\"Arduino ready\"}");
}

void loop()
{
    // Processar comandos recebidos do ESP8266
    processSerialCommands();

    // Atualizar sensor (exemplo)
    int newValue = analogRead(A0);
    if (abs(newValue - systemState.sensorValue) > 10)
    {
        systemState.sensorValue = newValue;

        // Enviar broadcast para todos os clientes WebSocket
        StaticJsonDocument<128> doc;
        doc["broadcast"] = true;
        doc["sensor"] = systemState.sensorValue;
        serializeJson(doc, Serial);
        Serial.println();
    }

    // Piscar LED (exemplo)
    if (millis() - systemState.lastBlink > 1000)
    {
        systemState.lastBlink = millis();
        systemState.ledState = !systemState.ledState;
        digitalWrite(LED_BUILTIN, systemState.ledState);
    }
}

void processSerialCommands()
{
    while (Serial.available())
    {
        char c = Serial.read();

        if (c == '\n' || c == '\r')
        {
            if (serialBufferPos > 0)
            {
                serialBuffer[serialBufferPos] = '\0';
                handleCommand(serialBuffer);
                serialBufferPos = 0;
            }
        }
        else if (serialBufferPos < SERIAL_BUFFER_SIZE - 1)
        {
            serialBuffer[serialBufferPos++] = c;
        }
    }
}

void handleCommand(const char *json)
{
    StaticJsonDocument<512> cmd;
    DeserializationError error = deserializeJson(cmd, json);

    if (error)
    {
        sendError("JSON parse error");
        return;
    }

    const char *protocol = cmd["protocol"];
    const char *method = cmd["method"];
    const char *uri = cmd["uri"];
    const char *body = cmd["body"];

    // Processar comando HTTP
    if (strcmp(protocol, "http") == 0)
    {
        handleHttpCommand(method, uri, body);
    }
    // Processar comando WebSocket
    else if (strcmp(protocol, "ws") == 0)
    {
        handleWebSocketCommand(body);
    }
    else
    {
        sendError("Unknown protocol");
    }
}

void handleHttpCommand(const char *method, const char *uri, const char *body)
{
    StaticJsonDocument<512> response;

    // Exemplo: GET /api/arduino/status
    if (strcmp(method, "GET") == 0 && strcmp(uri, "/api/arduino/status") == 0)
    {
        response["success"] = true;
        response["led"] = systemState.ledState ? "ON" : "OFF";
        response["sensor"] = systemState.sensorValue;
        response["uptime"] = millis();
    }
    // Exemplo: POST /api/arduino/led
    else if (strcmp(method, "POST") == 0 && strcmp(uri, "/api/arduino/led") == 0)
    {
        StaticJsonDocument<128> bodyDoc;
        deserializeJson(bodyDoc, body);

        if (bodyDoc.containsKey("state"))
        {
            systemState.ledState = bodyDoc["state"].as<bool>() ? HIGH : LOW;
            digitalWrite(LED_BUILTIN, systemState.ledState);

            response["success"] = true;
            response["led"] = systemState.ledState ? "ON" : "OFF";
        }
        else
        {
            response["success"] = false;
            response["error"] = "Missing 'state' parameter";
        }
    }
    // Rota não encontrada
    else
    {
        response["success"] = false;
        response["error"] = "Route not found";
        response["uri"] = uri;
        response["method"] = method;
    }

    // Enviar resposta de volta ao ESP8266
    serializeJson(response, Serial);
    Serial.println();
}

void handleWebSocketCommand(const char *message)
{
    StaticJsonDocument<256> response;

    // Processar mensagem WebSocket
    StaticJsonDocument<256> msg;
    deserializeJson(msg, message);

    if (msg.containsKey("action"))
    {
        const char *action = msg["action"];

        if (strcmp(action, "toggle") == 0)
        {
            systemState.ledState = !systemState.ledState;
            digitalWrite(LED_BUILTIN, systemState.ledState);

            response["success"] = true;
            response["action"] = "toggle";
            response["led"] = systemState.ledState ? "ON" : "OFF";
        }
        else if (strcmp(action, "status") == 0)
        {
            response["success"] = true;
            response["led"] = systemState.ledState ? "ON" : "OFF";
            response["sensor"] = systemState.sensorValue;
            response["uptime"] = millis();
        }
        else
        {
            response["success"] = false;
            response["error"] = "Unknown action";
        }
    }
    else
    {
        response["success"] = false;
        response["error"] = "No action specified";
    }

    // Enviar resposta
    serializeJson(response, Serial);
    Serial.println();
}

void sendError(const char *message)
{
    StaticJsonDocument<128> error;
    error["success"] = false;
    error["error"] = message;
    serializeJson(error, Serial);
    Serial.println();
}
