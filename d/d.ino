#include <AFMotor.h>

AF_DCMotor MotorFR(1);   // Front right motor
AF_DCMotor MotorFL(2);   // Front left motor
AF_DCMotor MotorBL(3);   // Back left motor
AF_DCMotor MotorBR(4);   // Back right motor

const int buzPin = 2;  // Buzzer pin
const int ledPin = A5; // LED pin
int valSpeed = 255;    // Default motor speed
unsigned long lastCommand = 0;

void setup() {
    Serial.begin(9600);
    pinMode(buzPin, OUTPUT);
    pinMode(ledPin, OUTPUT);

    MotorFL.setSpeed(valSpeed);
    MotorFR.setSpeed(valSpeed);
    MotorBL.setSpeed(valSpeed);
    MotorBR.setSpeed(valSpeed);

    MotorFL.run(RELEASE);
    MotorFR.run(RELEASE);
    MotorBL.run(RELEASE);
    MotorBR.run(RELEASE);
}

void loop() {
    if (millis() - lastCommand > 1000) {
        MotorFL.run(RELEASE);
        MotorFR.run(RELEASE);
        MotorBL.run(RELEASE);
        MotorBR.run(RELEASE);
    }

    while (Serial.available() > 0) {
        char command = Serial.read();
        lastCommand = millis();
        //Serial.println("BAT:75,SPEED:25");

        switch(command) {
            case 'F':
                SetSpeed(valSpeed);
                MotorFL.run(FORWARD);
                MotorFR.run(FORWARD);
                MotorBL.run(FORWARD);
                MotorBR.run(FORWARD);
                break;

            case 'B':
                SetSpeed(valSpeed);
                MotorFL.run(BACKWARD);
                MotorFR.run(BACKWARD);
                MotorBL.run(BACKWARD);
                MotorBR.run(BACKWARD);
                break;

            case 'R':
                SetSpeed(valSpeed);
                MotorFL.run(FORWARD);
                MotorFR.run(BACKWARD);
                MotorBL.run(FORWARD);
                MotorBR.run(BACKWARD);
                break;

            case 'L':
                SetSpeed(valSpeed);
                MotorFL.run(BACKWARD);
                MotorFR.run(FORWARD);
                MotorBL.run(BACKWARD);
                MotorBR.run(FORWARD);
                break;

            case 'G':   // Forward left
                MotorFL.setSpeed(valSpeed / 4);
                MotorBL.setSpeed(valSpeed / 4);
                MotorFL.run(FORWARD);
                MotorFR.run(FORWARD);
                MotorBL.run(FORWARD);
                MotorBR.run(FORWARD);
                break;

            case 'H':   // Forward right
                MotorFR.setSpeed(valSpeed / 4);
                MotorBR.setSpeed(valSpeed / 4);
                MotorFL.run(FORWARD);
                MotorFR.run(FORWARD);
                MotorBL.run(FORWARD);
                MotorBR.run(FORWARD);
                break;

            case 'I':   // Backward left
                MotorFL.setSpeed(valSpeed / 4);
                MotorBL.setSpeed(valSpeed / 4);
                MotorFL.run(BACKWARD);
                MotorFR.run(BACKWARD);
                MotorBL.run(BACKWARD);
                MotorBR.run(BACKWARD);
                break;

            case 'J':   // Backward right
                MotorFR.setSpeed(valSpeed / 4);
                MotorBR.setSpeed(valSpeed / 4);
                MotorFL.run(BACKWARD);
                MotorFR.run(BACKWARD);
                MotorBL.run(BACKWARD);
                MotorBR.run(BACKWARD);
                break;

            case 'S':
                MotorFL.run(RELEASE);
                MotorFR.run(RELEASE);
                MotorBL.run(RELEASE);
                MotorBR.run(RELEASE);
                break;

            case 'Y':   // Buzina
                digitalWrite(buzPin, HIGH);
                delay(200);
                digitalWrite(buzPin, LOW);
                delay(80);
                digitalWrite(buzPin, HIGH);
                delay(300);
                digitalWrite(buzPin, LOW);
                break;

            case 'U': digitalWrite(ledPin, HIGH); break;
            case 'u': digitalWrite(ledPin, LOW);  break;

            case '1': SetSpeed(65);  break;
            case '2': SetSpeed(130); break;
            case '3': SetSpeed(195); break;
            case '4': SetSpeed(255); break;

            case 'T':   // Testa rodas da frente
                MotorFL.setSpeed(150);
                MotorFR.setSpeed(150);
                MotorFL.run(FORWARD);
                MotorFR.run(FORWARD);
                break;

            case 'K':   // Testa rodas de trás
                MotorBL.setSpeed(150);
                MotorBR.setSpeed(150);
                MotorBL.run(FORWARD);
                MotorBR.run(FORWARD);
                break;

            default: break;
        }
    }
}

void SetSpeed(int val) {
    valSpeed = val;
    MotorFL.setSpeed(val);
    MotorFR.setSpeed(val);
    MotorBL.setSpeed(val);
    MotorBR.setSpeed(val);
}
