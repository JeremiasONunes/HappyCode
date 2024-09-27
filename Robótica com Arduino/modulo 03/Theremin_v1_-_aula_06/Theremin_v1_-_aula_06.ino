// MAPEAMENTO DOS PINOS

#define som  3    // 3 = PINO DE SAIDA DE SOM

#define echo2 9   // 9  = Sensor 2 Echo
#define trig2 8  // 8 = Sensor 2 Trig
#define echo1 7  // 7 = Sensor 1 Echo
#define trig1 6  // 6 = Sensor 1 Trig 

+
#define potFx     A0 // AN0 = POtenciometro de efeitos
#define potLoop   A1 // AN1 = Potenciômetro LOOP (tempo)


// FIM DO MAPEAMENTO DOS PINOS

// Variaveis
int  ciclo      = 0;
int  distValue1 = 30;
int  distValue2 = 30;

// Essa escala pode ser modificada para sintetizar sons diferentes
int steps[] = {100,120,140,160,180,200,220,240};

int pitchval = 1;
int tempo; 
long time;
byte i=0;

void le_sensores() {
  // Código para ler os sensores e atualizar distValue1 e distValue2
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  distValue1 = pulseIn(echo1, HIGH) / 58.2;

  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  distValue2 = pulseIn(echo2, HIGH) / 58.2;
}

void freqout(int freq, int duration) {
  int halfPeriod = 1000000L / freq / 2;
  for (long i = 0; i < duration * 1000L; i += halfPeriod * 2) {
  digitalWrite(som, HIGH);
  delayMicroseconds(halfPeriod);
  digitalWrite(som, LOW);
  delayMicroseconds(halfPeriod);
  }
}


void setup() {
Serial.begin(9600);

pinMode(som,OUTPUT);

pinMode(trig1,OUTPUT); // Pino "trig" (gatilho/disparo) do som.
pinMode(echo1,INPUT);  // Pino "echo" (eco / retorno)   do som.
pinMode(trig2,OUTPUT); // Pino "trig" (gatilho/disparo) do som.
pinMode(echo2,INPUT);  // Pino "echo" (eco / retorno)   do som.

}// fim do setup






void loop(){
le_sensores();

 

 
if(millis() - time > tempo){

tempo    = analogRead(potLoop)/ 8; 


if(distValue1 > 30){distValue1 = 30;}
if(distValue1 < 3){distValue1 = 3;}
distValue1 = map(distValue1,3,30,5,128);
int pwmLed1 = map(distValue1,5,128,0,255);


if(distValue2 > 30){distValue2 = 30;}
if(distValue2 < 3){distValue2 = 3;}
distValue2 = map(distValue2,3,30,100,3000);
int pwmLed2 = map(distValue2,100,3000,0,255);


 
freqout (steps[i] + distValue2, distValue1);


i++;
if(i>=8){i=0;}

time=millis(); 
}


    
}// fim do loop
