#include <WiFi.h>  // Biblioteca para conectar o ESP32 ao WiFi
#include <HTTPClient.h>  // Biblioteca para fazer requisições HTTP
#include <ArduinoJson.h>  // Biblioteca para manipular JSON (precisa ser instalada via Library Manager)

// Configuração da rede Wi-Fi
const char* ssid = "Nome da rede WiF";
const char* password = "Senha do WiFi";

// Configuração dos pinos
const int botaoPin = 21;  // Pino do botão
const int led1Pin = 23;   // LED externo
const int led2Pin = 2;    // LED interno (indicador de conexão)

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

// Função para obter a localização do IP
String obterLocalizacaoIP() {
  HTTPClient http;
  String url = "http://ipinfo.io/json";  // Serviço que fornece a localização por IP

  http.begin(url);  // Inicia a requisição HTTP
  int httpCode = http.GET();  // Realiza a requisição GET

  String payload = "";
  if (httpCode == 200) {  // Se a requisição for bem-sucedida
    payload = http.getString();  // Obtém a resposta
  } else {
    Serial.println("Erro ao obter localização.");
  }
  http.end();  // Fecha a conexão HTTP

  return payload;  // Retorna os dados obtidos em formato JSON
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
  pinMode(led1Pin, OUTPUT);  // Configura o LED1 como saída
  pinMode(led2Pin, OUTPUT);  // Configura o LED2 como saída

  conectarWifi();  // Conectar ao Wi-Fi

  // Obter localização e adicionar ao texto da mensagem
  String localizacao = obterLocalizacaoIP();
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, localizacao);
  
  if (!error) {
    String loc = doc["loc"];  // Obtém a localização
    mensagem += ", Localização: https://maps.google.com/?q=" + loc;
  } else {
    Serial.println("Erro ao processar os dados de localização.");
  }
}

void loop() {
  static bool flag = true;  // Controle para evitar múltiplos envios

  if (digitalRead(botaoPin) == LOW) {  // Botão pressionado (estado LOW)
    Serial.println("Botão pressionado. Preparando para enviar mensagem.");
    delay(500);  // Debounce para o botão

    digitalWrite(led1Pin, HIGH);  // Acende o LED1 enquanto a mensagem é enviada

    if (flag) {
      enviarMensagem(phoneNumber, apiKey, mensagem);
      flag = false;  // Evita envio repetido
    }
  } else {
    flag = true;  // Restaura a flag quando o botão é solto
    digitalWrite(led1Pin, LOW);  // Desliga o LED1
  }

  delay(100);  // Delay para reduzir o uso da CPU
}
