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
int readWait = 800;                          // Define o tempo de espera para nova leitura pelo receptor de IR
bool carOn = false;                          // Variável para controlar o estado do carrinho (ligado/desligado)
void (*lastCommand)() = NULL;
void setup() {
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
    // Verifica se o controle remoto enviou um código
    if (receptor.decode(&resultado)) {
        switch (resultado.value) { 
            case tk_enter:                    // Tecla Enter
                if (!carOn) {
                    turnOnOffCar();           // Liga o carrinho
                    frente();                 // Começa a mover o carrinho para frente
                    lastCommand = frente;     // Armazena o comando frente como padrão
                } else {
                    turnOnOffCar();           // Desliga o carrinho
                    lastCommand = NULL;       // Reseta o comando para NULL
                }
                break;             
        } 
        receptor.resume();                    // Prepara o receptor para a próxima leitura
    }

    // Continua executando a última função enquanto o carrinho estiver ligado
    if (carOn && lastCommand != NULL) {
        lastCommand(); // Chama a função armazenada em lastCommand
    } else if (!carOn) {
        pare();  // Para o carrinho se estiver desligado
    }

    delay(100);  // Pequena pausa para evitar leitura excessiva
}

void turnOnOffCar() {
    carOn = !carOn;                           // Alterna o estado do carrinho (ligado/desligado)
    if (carOn) {
        Serial.println("Carrinho Ligado");
        frente();
    } else {
        Serial.println("Carrinho Desligado");
        pare();
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
void pare() {
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    Serial.println("pare");
}
