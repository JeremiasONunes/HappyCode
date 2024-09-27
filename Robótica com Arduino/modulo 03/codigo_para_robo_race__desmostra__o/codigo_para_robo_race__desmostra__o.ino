#include <IRremote.h>                        // Carrega a biblioteca de controle remoto infravermelho IR
#include "IRcontrol.h"                       // Carrega a lista de códigos das teclas do controle remoto

IRrecv receptor(9);                          // Mudei o receptor para a porta 8 do Arduino para evitar conflito com o motor
decode_results resultado;                    // Define a variável resultado, que armazenará o código da tecla pressionada
#define ENA 5
#define ENB 6
#define IN1 3
#define IN2 4
#define IN3 2
#define IN4 7

bool carOn = true;                          // Variável para controlar o estado do carrinho (ligado/desligado)
void (*lastCommand)() = NULL;
unsigned long lastMillis = 0;
const unsigned long moveDuration = 260; // Tempo de movimento para esquerda/direita em milissegundos

void setup() {
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    Serial.begin(9600);
    receptor.enableIRIn();                   // Inicializa o receptor de IR
}

void loop() {
    if (receptor.decode(&resultado)) {
        switch (resultado.value) { 
            case tk_enter:                    // Tecla Enter
                toggleCar();
                break;

            case tk_left:                     // Tecla Esquerda
                if (carOn) {
                    esquerda();
                    lastCommand = esquerda;
                    lastMillis = millis();
                }
                break;

            case tk_right:                    // Tecla Direita
                if (carOn) {
                    direita();
                    lastCommand = direita;
                    lastMillis = millis();
                }
                break;

            case tk3:                         // Tecla Esquerda alternativa
                if (carOn) {
                    giraEsquerda();
                    lastCommand = giraEsquerda;
                }
                break;

            case tk4:                         // Tecla Direita alternativa
                if (carOn) {
                    giraDireita();
                    lastCommand = giraDireita;
                }
                break;    
                
            case tk_up:                       // Tecla Frente
                if (carOn) {
                    frente();
                    lastCommand = frente;
                }
                break;

            case tk_down:                     // Tecla Trás
                if (carOn) {
                    atras();
                    lastCommand = atras;
                }
                break;

            case tk_stop:                     // Tecla Parar (opcional)
                pare();
                lastCommand = pare;
                break;
        }
        receptor.resume();                    // Prepara o receptor para a próxima leitura
    }

    // Continua executando a última função enquanto o carrinho estiver ligado
    if (carOn && lastCommand != NULL) {
        lastCommand(); // Chama a função armazenada em lastCommand
    }

    // Verifica se o tempo de movimento para esquerda/direita expirou
    if ((lastCommand == esquerda || lastCommand == direita) && millis() - lastMillis >= moveDuration) {
        pare();
        lastCommand = NULL;
    }
}

void toggleCar() {
    carOn = !carOn;                           // Alterna o estado do carrinho (ligado/desligado)
    if (carOn) {
        Serial.println("Carrinho Ligado");
        digitalWrite(10, LOW);
        digitalWrite(11, HIGH);
    } else {
        Serial.println("Carrinho Desligado");
        digitalWrite(11, LOW);
        digitalWrite(10, HIGH);
        pare();
        lastCommand = NULL;
    }
}

void frente() {
    analogWrite(ENA, 200);
    analogWrite(ENB, 200);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("frente");
}

void direita() {
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("direita");
}

void giraEsquerda() {
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("esquerda");
}

void giraDireita() {
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("direita");
}

void esquerda() {
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("esquerda");
}


void atras() {
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("atras");
}

void pare() {
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    Serial.println("pare");
}
