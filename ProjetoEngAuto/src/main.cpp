#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Configurações OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Pinos ---
const int PIN_BOTAO = 13;   // Substituiu o PIR
const int PIN_SERVO = 19;
const int PIN_BEEP  = 12;
const int PIN_TRIG  = 5;
const int PIN_ECHO  = 18;

Servo portaServo;
unsigned long tempoUltimaAcao = 0;
unsigned long tempoInicioBloqueio = 0;
const unsigned long ESPERA_PORTA = 10000;    // Tempo que fica aberta
const unsigned long TIMEOUT_SEGURANCA = 12000;

bool portaAberta = false;

long lerDistancia() {
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    long duracao = pulseIn(PIN_ECHO, HIGH, 26000);
    if (duracao == 0) return 999;
    return duracao * 0.034 / 2;
}

void atualizarOLED(String status, int dist) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("CONTROLE DE ACESSO");
    display.drawFastHLine(0, 10, 128, WHITE);
    
    display.setCursor(0, 25);
    display.setTextSize(2);
    display.println(status);
    
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print("Sensor: ");
    display.print(dist);
    display.print("cm");
    display.display();
}

void setup() {
    Serial.begin(115200);
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println("Falha no OLED");
    }
    
    pinMode(PIN_BOTAO, INPUT_PULLUP); // Ativa resistor interno
    pinMode(PIN_BEEP, OUTPUT);
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    ESP32PWM::allocateTimer(0);
    portaServo.setPeriodHertz(50);
    portaServo.attach(PIN_SERVO, 500, 2400);
    portaServo.write(0); // Garante porta fechada ao iniciar
    
    atualizarOLED("PRONTO", 0);
}

void loop() {
    long distancia = lerDistancia();
    bool botaoPressionado = (digitalRead(PIN_BOTAO) == LOW); // LOW quando aperta
    String msgOLED = "";

    // 1. LÓGICA DE SEGURANÇA (OBSTRUÇÃO)
    if (distancia < 25) {
        if (tempoInicioBloqueio == 0) tempoInicioBloqueio = millis();
        if (millis() - tempoInicioBloqueio > TIMEOUT_SEGURANCA) {
            msgOLED = "BLOQUEADO!";
            digitalWrite(PIN_BEEP, HIGH); delay(50); digitalWrite(PIN_BEEP, LOW);
        } else {
            msgOLED = "OBSTRUIDO";
        }
    } else {
        tempoInicioBloqueio = 0;
    }

    // 2. ABERTURA POR BOTÃO
    if (botaoPressionado) {
        if (!portaAberta) {
            // Tenta abrir: checa obstrução primeiro
            if (distancia < 25) {
                // Está obstruído! Não abre e dá 2 bipes
                msgOLED = "ERRO: OBSTR.";
                
                // Bipe 1
                digitalWrite(PIN_BEEP, HIGH); delay(100); digitalWrite(PIN_BEEP, LOW);
                delay(100);
                // Bipe 2
                digitalWrite(PIN_BEEP, HIGH); delay(100); digitalWrite(PIN_BEEP, LOW);
                
                // Pequeno delay extra para evitar que os bipes se repitam sem parar se você segurar o botão
                delay(300); 
            } else {
                // Caminho livre: abre a porta
                portaServo.write(90);
                portaAberta = true;
                digitalWrite(PIN_BEEP, HIGH); delay(100); digitalWrite(PIN_BEEP, LOW);
                if (msgOLED == "") msgOLED = "ABRINDO...";
            }
        }
        // Sempre que apertar o botão, reinicia o tempo para a porta não fechar na cara da pessoa
        tempoUltimaAcao = millis(); 
    }

    // 3. FECHAMENTO AUTOMÁTICO (Só fecha se não houver obstrução)
    if (portaAberta && (millis() - tempoUltimaAcao > ESPERA_PORTA)) {
        if (distancia > 25) { // Verifica se o caminho está livre
            portaServo.write(0);
            portaAberta = false;
            msgOLED = "FECHANDO";
            digitalWrite(PIN_BEEP, HIGH); delay(500); digitalWrite(PIN_BEEP, LOW);
        } else {
            // Se o tempo acabou mas algo está na frente, mantém aberta e avisa
            if (msgOLED == "") msgOLED = "AGUARDANDO";
        }
    }

    // 4. ESTADO DE REPOUSO
    if (msgOLED == "") {
        msgOLED = portaAberta ? "ABERTA" : "AGUARDANDO";
    }

    atualizarOLED(msgOLED, (int)distancia);
    delay(50); // Menor delay para resposta mais rápida do botão
}