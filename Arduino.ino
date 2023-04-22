#include <SoftwareSerial.h>

// configuração da porta serial
const int RXD = 3;
const int TXD = 1;
SoftwareSerial SerialESP8266(RXD, TXD);

// configuração dos pinos do joystick
const int JOY_X_PIN = A0;
const int JOY_Y_PIN = A1;
const int JOY_Z_PIN = 2;
const int JOY_BUTTON_PIN = 3;

void setup()
{
    // inicia a comunicação serial
    Serial.begin(9600);
    SerialESP8266.begin(9600);

    // configura os pinos do joystick
    pinMode(JOY_Z_PIN, INPUT_PULLUP);
    pinMode(JOY_BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
    // leitura dos valores do joystick
    int joy_x = analogRead(JOY_X_PIN);
    int joy_y = analogRead(JOY_Y_PIN);
    int joy_z = digitalRead(JOY_Z_PIN);
    int joy_button = digitalRead(JOY_BUTTON_PIN);

    // envio dos valores do joystick para o ESP8266
    Serial.print(joy_x);
    Serial.print(",");
    Serial.print(joy_y);
    Serial.print(",");
    Serial.print(joy_z);
    Serial.print(",");
    Serial.print(joy_button);
    Serial.println();

    // aguarda um breve intervalo antes de ler novamente o joystick
    delay(10);
}
