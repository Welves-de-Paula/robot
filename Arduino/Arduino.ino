// Define o pino do LED
const int ledPin = 13;

void setup()
{
    // Inicializa o LED como saída
    pinMode(ledPin, OUTPUT);

    // Inicia a comunicação serial
    Serial.begin(9600);
}

void loop()
{
    // Aguarda a chegada de dados pela porta serial
    while (Serial.available() == 0)
        ;

    // Lê o dado recebido
    String data = Serial.readStringUntil('\n');

    // Se o dado for "ON", liga o LED
    if (data == "ON")
    {
        digitalWrite(ledPin, HIGH);
    }
    // Se o dado for "OFF", desliga o LED
    else if (data == "OFF")
    {
        digitalWrite(ledPin, LOW);
    }
}
