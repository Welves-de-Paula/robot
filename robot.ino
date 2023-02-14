#include <Wire.h>

#define Motor1A 5
#define Motor1B 6
#define Motor2A 7
#define Motor2B 8
#define Motor3A 9
#define Motor3B 10
#define Motor4A 11
#define Motor4B 12

const int buttons[] = {1, 2, 3, 4, 5, 6};

void setup()
{
  // Inicializa a comunicação com o Motor Shield
  Wire.begin();

  // Configura os pinos dos motores como saídas
  pinMode(Motor1A, OUTPUT);
  pinMode(Motor1B, OUTPUT);
  pinMode(Motor2A, OUTPUT);

  pinMode(Motor2B, OUTPUT);
  pinMode(Motor3A, OUTPUT);
  pinMode(Motor3B, OUTPUT);
  pinMode(Motor4A, OUTPUT);
  pinMode(Motor4B, OUTPUT);

  // Configura os pinos dos botões como entradas
  for (int i = 0; i < 6; i++)
  {
    pinMode(buttons[i], INPUT_PULLUP);
  }
}

void loop()
{
  // Controla o motor 1 de acordo com os estados dos botões
  if (digitalRead(buttons[0]) == LOW)
  {
    digitalWrite(Motor1A, HIGH);
    digitalWrite(Motor1B, LOW);
  }
  else if (digitalRead(buttons[1]) == LOW)
  {
    digitalWrite(Motor1A, LOW);
    digitalWrite(Motor1B, HIGH);
  }
  else
  {
    digitalWrite(Motor1A, LOW);
    digitalWrite(Motor1B, LOW);
  }

  // Controla o motor 2 de acordo com os estados dos botões
  if (digitalRead(buttons[2]) == LOW)
  {
    digitalWrite(Motor2A, HIGH);
    digitalWrite(Motor2B, LOW);
  }
  else if (digitalRead(buttons[3]) == LOW)
  {
    digitalWrite(Motor2A, LOW);
    digitalWrite(Motor2B, HIGH);
  }
  else
  {
    digitalWrite(Motor2A, LOW);
    digitalWrite(Motor2B, LOW);
  }

  // Controla o motor 3 de acordo com os estados dos botões
  if (digitalRead(buttons[4]) == LOW)
  {
    digitalWrite(Motor3A, HIGH);
    digitalWrite(Motor3B, LOW);
  }
  else if (digitalRead(buttons[5]) == LOW)
  {
    digitalWrite(Motor3A, LOW);
    digitalWrite(Motor3B, HIGH);
  }
  else
  {
    digitalWrite(Motor3A, LOW);
    digitalWrite(Motor3B, LOW);
  }
}
