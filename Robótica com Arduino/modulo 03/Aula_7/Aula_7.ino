#include <Ethernet.h>
#include <Servo.h>  // Inclua a biblioteca do servo

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 110);
EthernetServer server(80);

const int led1 = 2;
const int led2 = 3;
const int led3 = 4;

Servo myServo;  // Declare o objeto servo
const int servoPin = 5;  // Defina o pino para o servo

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  myServo.attach(servoPin);  // Anexe o servo ao pino definido
  myServo.write(0);  // Inicialize o servo na posição 0

  Ethernet.begin(mac, ip);
  server.begin();

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println(F("Ethernet shield não encontrada."));
    while (true);
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println(F("Cabo Ethernet não está conectado."));
  } else {
    Serial.print(F("Conectado com o IP: "));
    Serial.println(Ethernet.localIP());
  }
}

void loop() {
  EthernetClient client = server.available();

  if (client) {
    Serial.println(F("Cliente conectado"));
    char c;
    bool currentLineIsBlank = true;
    char httpRequest[128];
    int index = 0;

    while (client.connected()) {
      if (client.available()) {
        c = client.read();
        
        if (index < sizeof(httpRequest) - 1) {
          httpRequest[index++] = c;
        }

        if (c == '\n' && currentLineIsBlank) {
          httpRequest[index] = '\0';
          Serial.println(F("Requisição HTTP recebida:"));
          Serial.println(httpRequest);

          handleRequest(client, httpRequest);

          break;
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }

    delay(1);
    client.stop();
    Serial.println(F("Conexão encerrada"));
  }
}

void handleRequest(EthernetClient client, char* httpRequest) {
  if (strstr(httpRequest, "GET /LED1/ON") != NULL) {
    digitalWrite(led1, HIGH);
    Serial.println(F("LED 1 ligado"));
  } else if (strstr(httpRequest, "GET /LED1/OFF") != NULL) {
    digitalWrite(led1, LOW);
    Serial.println(F("LED 1 desligado"));
  } else if (strstr(httpRequest, "GET /LED2/ON") != NULL) {
    digitalWrite(led2, HIGH);
    Serial.println(F("LED 2 ligado"));
  } else if (strstr(httpRequest, "GET /LED2/OFF") != NULL) {
    digitalWrite(led2, LOW);
    Serial.println(F("LED 2 desligado"));
  } else if (strstr(httpRequest, "GET /LED3/ON") != NULL) {
    digitalWrite(led3, HIGH);
    Serial.println(F("LED 3 ligado"));
  } else if (strstr(httpRequest, "GET /LED3/OFF") != NULL) {
    digitalWrite(led3, LOW);
    Serial.println(F("LED 3 desligado"));
  } else if (strstr(httpRequest, "GET /SERVO/0") != NULL) {
    myServo.write(0);  // Move o servo para 0 graus
    Serial.println(F("Servo movido para 0 graus"));
  } else if (strstr(httpRequest, "GET /SERVO/90") != NULL) {
    myServo.write(90);  // Move o servo para 90 graus
    Serial.println(F("Servo movido para 90 graus"));
  } else {
    handleNotFound(client);
    return;
  }

  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html; charset=utf-8")); // Definir charset UTF-8
  client.println(F("Connection: close"));
  client.println();

  client.println(F("<!DOCTYPE html>"));
  client.println(F("<html>"));
  client.println(F("<head>"));
  client.println(F("<title>Controle de LEDs e Servo</title>"));
  client.println(F("<meta name='viewport' content='width=device-width, initial-scale=1.0'>")); // Responsivo
  client.println(F("<meta charset='UTF-8'>")); // Definir charset UTF-8
  client.println(F("<style>"));
  
  // Estilos responsivos com media queries
  client.println(F("body { text-align: center; background-color: #f2f2f2; margin: 0; font-family: Arial, sans-serif; }"));
  client.println(F("h1 { color: #333; text-align: center; }")); // Alinhamento do título
  client.println(F("p { color: #666; text-align: center; }")); // Alinhamento dos parágrafos
  client.println(F("button { margin: 10px; padding: 10px 20px; font-size: 16px; background-color: #4CAF50; color: white; border: none; border-radius: 4px; cursor: pointer; }"));
  
  // Estilos para dispositivos móveis
  client.println(F("@media (max-width: 600px) { button { width: 100%; padding: 15px; font-size: 18px; } }"));
  
  client.println(F("</style>"));
  client.println(F("</head>"));
  client.println(F("<body>"));
  client.println(F("<h1>Controle de LEDs e Servo</h1>"));
  client.println(F("<p>Clique nos botões abaixo para ligar ou desligar os LEDs e mover o Servo.</p>"));
  client.println(F("<button onclick=\"location.href='/LED1/ON'\">Ligar LED 1</button>"));
  client.println(F("<button onclick=\"location.href='/LED1/OFF'\">Desligar LED 1</button>"));
  client.println(F("<button onclick=\"location.href='/LED2/ON'\">Ligar LED 2</button>"));
  client.println(F("<button onclick=\"location.href='/LED2/OFF'\">Desligar LED 2</button>"));
  client.println(F("<button onclick=\"location.href='/LED3/ON'\">Ligar LED 3</button>"));
  client.println(F("<button onclick=\"location.href='/LED3/OFF'\">Desligar LED 3</button>"));
  client.println(F("<button onclick=\"location.href='/SERVO/0'\">Mover Servo para 0°</button>"));
  client.println(F("<button onclick=\"location.href='/SERVO/90'\">Mover Servo para 90°</button>"));
  client.println(F("</body>"));
  client.println(F("</html>"));
}

void handleNotFound(EthernetClient client) {
  client.println(F("HTTP/1.1 404 Not Found"));
  client.println(F("Content-Type: text/html; charset=utf-8")); // Definir charset UTF-8
  client.println(F("Connection: close"));
  client.println();

  client.println(F("<!DOCTYPE html>"));
  client.println(F("<html>"));
  client.println(F("<head>"));
  client.println(F("<title>Página não encontrada</title>"));
  client.println(F("<meta name='viewport' content='width=device-width, initial-scale=1.0'>"));
  client.println(F("<meta charset='UTF-8'>")); // Definir charset UTF-8
  client.println(F("<style>"));
  client.println(F("body { text-align: center; background-color: #f2f2f2; margin: 0; font-family: Arial, sans-serif; }"));
  client.println(F("h1 { color: #333; text-align: center; }"));
  client.println(F("p { color: #666; text-align: center; }"));
  client.println(F("</style>"));
  client.println(F("</head>"));
  client.println(F("<body>"));
  client.println(F("<h1>Página não encontrada</h1>"));
  client.println(F("<p>A página que você está tentando acessar não foi encontrada.</p>"));
  client.println(F("</body>"));
  client.println(F("</html>"));
}
