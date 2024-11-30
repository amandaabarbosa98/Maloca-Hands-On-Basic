#include <Wire.h>                // Biblioteca para comunicação I2C
#include <Adafruit_SSD1306.h>     // Biblioteca para o display SSD1306
#include <TinyGPS++.h>            // Biblioteca para o módulo GPS
#include <WiFi.h>                 // Biblioteca para conectar o ESP32 ao WiFi
#include <HTTPClient.h>           // Biblioteca para fazer requisições HTTP
#include <ArduinoJson.h>          // Biblioteca para manipular JSON

// Configuração do display SSD1306
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Se você não tiver um pino de reset, use -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configuração do GPS
#define GPS_RX_PIN 16
#define GPS_TX_PIN 17

TinyGPSPlus gps;  // Instância do objeto GPS
HardwareSerial mySerial(1);  // Usando a segunda porta serial (Serial1) para o GPS

// Configuração da rede Wi-Fi
const char* ssid = "Nome da rede WiFi";
const char* password = "Senha do WiFi";

// Configuração dos pinos
const int botaoPin = 19;   // Botão no pino 19
const int led1Pin = 23;    // LED1 (não usado mais, mas pode ser útil para testes)
const int led2Pin = 2;     // LED2 (não usado, mas também pode ser útil)

// Configuração do CallMeBot
String phoneNumber = "5595000000000";  // Número de telefone
String apiKey = "Chave API";  // Chave da API fornecida pelo CallMeBot
String mensagem = "SOCORRO!!!! ME AJUDE";  // Mensagem a ser enviada

// Função para conectar ao Wi-Fi
void conectarWifi() {
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);

  // Pisca o LED2 enquanto o ESP32 se conecta
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led2Pin, HIGH);
    delay(500);
    digitalWrite(led2Pin, LOW);
    delay(500);
  }
  Serial.println("Conectado ao Wi-Fi!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  digitalWrite(led2Pin, HIGH);  // LED2 ligado indica conexão bem-sucedida
}

// Função para obter localização do GPS
String obterLocalizacaoGPS() {
  while (mySerial.available() > 0) {
    gps.encode(mySerial.read());
  }

  if (gps.location.isUpdated()) {  // Verifica se a localização foi atualizada
    double latitude = gps.location.lat();
    double longitude = gps.location.lng();

    String loc = String(latitude, 6) + "," + String(longitude, 6);
    return loc;
  } else {
    return "GPS não disponível";
  }
}

// Função para enviar a mensagem via CallMeBot API
void enviarMensagem(String phoneNumber, String apiKey, String message) {
  HTTPClient http;

  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber +
               "&apikey=" + apiKey + "&text=" + urlencode(message);

  http.begin(url);
  int httpResponseCode = http.GET();
  
  if (httpResponseCode == 200) {
    Serial.println("Mensagem enviada com sucesso!");
  } else {
    Serial.println("Erro ao enviar mensagem.");
    Serial.print("Código HTTP: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

// Função de codificação para URL
String urlencode(const String& str) {
  String encoded = "";
  for (unsigned int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      encoded += c;
    } else {
      encoded += '%' + String(c, HEX);
    }
  }
  return encoded;
}

void setup() {
  Serial.begin(115200);  // Inicializa a comunicação serial

  pinMode(botaoPin, INPUT_PULLUP);  // Configura o botão com pull-up interno
  pinMode(led1Pin, OUTPUT);  // Configura o LED1 como saída (não será usado agora)
  pinMode(led2Pin, OUTPUT);  // Configura o LED2 como saída

  // Inicializa o display SSD1306
  if (!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
    Serial.println(F("Erro ao iniciar o display SSD1306"));
    while (true);
  }
  
  display.display();  // Exibe a tela inicial
  delay(2000);  // Espera um pouco para que o display seja visível

  display.clearDisplay();  // Limpa a tela
  
  conectarWifi();  // Conectar ao Wi-Fi

  // Inicializa a comunicação serial com o GPS
  mySerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN); 
}

void loop() {
  static bool flag = true;  // Controle para evitar múltiplos envios

  if (digitalRead(botaoPin) == LOW) {  // Botão pressionado (estado LOW)
    Serial.println("Botão pressionado. Preparando para enviar mensagem.");
    delay(500);  // Debounce para o botão

    display.clearDisplay();  // Limpa o display
    display.setTextSize(1);  // Define o tamanho do texto
    display.setTextColor(SSD1306_WHITE);  // Define a cor do texto (branco)
    display.setCursor(0, 0);  // Define o ponto inicial do texto
    display.print("Enviando mensagem...");

    display.display();  // Atualiza o display

    String localizacao = obterLocalizacaoGPS();  // Obtém a localização do GPS
    mensagem += ", Localização: https://maps.google.com/?q=" + localizacao;

    if (flag) {
      enviarMensagem(phoneNumber, apiKey, mensagem);
      flag = false;  // Evita envio repetido
    }

    // Mensagem enviada
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Mensagem enviada!");
    display.display();
    delay(2000);  // Exibe a confirmação por 2 segundos
  } else {
    flag = true;  // Restaura a flag quando o botão é solto
  }

  delay(100);  // Delay para reduzir o uso da CPU
}
