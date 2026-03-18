#include <Arduino.h>
#include <ESP32Servo.h> 

// Definição dos Pinos
const int sensorPin = 13;
const int servoPin = 12;
const int ledPin = 2; // LED interno da placa

Servo meuServo;

void setup() {
    Serial.begin(115200);
    
    pinMode(sensorPin, INPUT);
    pinMode(ledPin, OUTPUT);

    // Configuração do Servo
    meuServo.attach(servoPin);
    meuServo.write(0); // Garante que a porta comece fechada
    
    Serial.println("Sistema de Porta Automática Iniciado!");
}

void loop() {
    int presenca = digitalRead(sensorPin);

    // Lógica Inversa: Normalmente sensores IR mandam LOW quando detectam algo
    if (presenca == LOW) {
        Serial.println("Objeto detectado! Abrindo porta...");
        digitalWrite(ledPin, HIGH);
        
        meuServo.write(90); // Abre a porta (ajuste o ângulo conforme necessário)
        delay(2500);        // Mantém aberta por 25 segundos
    } 
    else {
        digitalWrite(ledPin, LOW);
        meuServo.write(0);  // Fecha a porta //INCLUIR CONDICAO (SE NAO TIVER NINGUEM FECHE)
    }

    delay(100); // Pequeno delay para estabilidade
}