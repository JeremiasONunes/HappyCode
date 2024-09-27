#include <IRremote.h>                        // carrega a biblioteca de controle remoto infravermelho IR
#include "IRcontrol.h"                       // carrega a lista de códigos das Tks do controle remoto
 
IRrecv receptor(9);                          // define a porta 2 do Arduino na qual o receptor de IR será conectado
decode_results resultado;                    // define a variável numberCode, que armazenará o código da Tk pressionada

#define ENA 5
#define ENB 6
#define IN1 3
#define IN2 4
#define IN3 2
#define IN4 7 
bool carOn = false; 

void setup() {
  Serial.begin(9600);         // inicializa a comunicação serial
  receptor.enableIRIn();      // inicializa a recepção de sinais do controle remoto
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

void loop() {
  // vincula as funções criadas mais adinate a cada tecla do controle remoto
  if (receptor.decode(&resultado))   {
    switch (resultado.value) {
      case (tk_right):                    // tecla seta para a direita pressionada
        Serial.println("Tk right");
        direita();                        // move o carro para a direita
        break;
      case (tk_left):                     // tecla seta para a esquerda pressionada
        Serial.println("Tk left");
        esquerda();                       // move o carro para a esquerda
        break; 
      case (tk_up):                       // tecla seta para cima pressionada
        Serial.println("Tk up");
         direita();
         delay(5000);
         esquerda();
         delay(5000);
         pare();
        break;
      case (tk_down):                     // tecla seta para baixo pressionada
        Serial.println("Tk down");
        esquerda();
        delay(5000);
        direita();
        delay(5000);                   
        break;  
      case (tk_enter):                    // tecla Enter pressionada
        Serial.println("Tk Enter");
        turnOnOffCar();                       
        break;
    } 
    receptor.resume(); 
  }
  delay(100);
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

void atras() {
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("atras");
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

void esquerda() {
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("esquerda");
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
