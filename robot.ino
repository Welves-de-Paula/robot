// Bibliotecas necessárias
#include <SoftwareSerial.h>
#include <AFMotor.h>

// Define os pinos do ESP01
#define RX 2
#define TX 3

// Inicializa o software serial
SoftwareSerial espSerial(RX, TX);

// Cria o objeto para controlar o Motor Shield
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

void setup() {
  // Inicializa o ESP01
  espSerial.begin(9600);
  espSerial.println("Robo 4WD inicializado");

  // Inicializa o Motor Shield
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  motor4.setSpeed(0);
}

void loop() {
  // Lê o comando enviado pelo ESP01
  if (espSerial.available() > 0) {
    char comando = espSerial.read();

    // Verifica qual comando foi recebido
    if (comando == 'F') {
      // Move o robô para frente
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
    } else if (comando == 'B') {
      // Move o robô para trás
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      motor3.run(BACKWARD);
      motor4.run(BACKWARD);
    } else if (comando == 'L') {
      // Gira o robô para a esquerda
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor3.run(BACKWARD);
      motor4.run(BACKWARD);
    } else if (comando == 'R') {
      // Gira o robô para a direita
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
    } else if (comando == 'S') {
      // Para o robô
      motor1.run(RELEASE);
      motor2.run(RELEASE);
      motor3.run(RELEASE);
      motor4.run(RELEASE);
    }
  }
}
