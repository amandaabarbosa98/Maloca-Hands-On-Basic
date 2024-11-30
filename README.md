# Maloca-Hands-On-Basic (Botão de Pânico)

Este projeto usa o microcontrolador ESP32 para criar um sistema de alerta de emergência. Ao pressionar um botão, o sistema envia uma mensagem de socorro via WhatsApp para um número predefinido, utilizando a API do CallMeBot. O código também obtém a localização do dispositivo usando o IP e adiciona essa informação à mensagem. Um LED externo são usados para alertar os usuários locais.


## Funcionalidades

- **Envio de mensagens de socorro via WhatsApp**: Quando o botão é pressionado, o sistema envia uma mensagem de alerta ("SOCORRO!!! ME AJUDE") para dois números de telefone cadastrados.
- **Informação de localização**: A localização do dispositivo é obtida com base no IP público e adicionada à mensagem enviada. O link para a localização é incluído na mensagem para facilitar o acompanhamento.
- **Indicação de status de conexão Wi-Fi**: O sistema pisca um LED interno para indicar que está tentando se conectar à rede Wi-Fi e acende quando a conexão é bem-sucedida.
- **Indicação do envio de mensagem**: Um LED externo acende quando a mensagem está sendo enviada, proporcionando feedback visual.

## Hardware: 
- ESP32 (ou qualquer outro microcontrolador compatível com a biblioteca WiFi)
- Botão
- LED externo
- LED interno (do próprio ESP32)
- Resistores (para o botão e LEDs)
- Fios de conexão e protoboard

## Instalação:
1. Clone este repositório para o seu computador:

Copiar código
```cpp
git clone https://github.com/amandaabarbosa98/Maloca-Hands-On-Basic.git
```

2. Abra o arquivo codigo.ino no Arduino IDE ou em outro ambiente de desenvolvimento compatível com o ESP32.

3. Instale as bibliotecas necessárias:

- WiFi.h
- HTTPClient.h
- UrlEncode.h
  
4. Configure as credenciais do Wi-Fi no código:
- Modifique as variáveis ssid e password com o nome e a senha da sua rede Wi-Fi.

5. Registre-se no CallMeBot para obter uma chave de API (API Key) e insira os números de telefone e a chave no código.
6. Carregue o código no seu ESP32.

## Estrutura do Código
- **Função conectarWifi()** Conecta o ESP32 à rede Wi-Fi e indica o status com o LED interno.
- **Função obterLocalizacaoIP():** Obtém a localização do dispositivo usando o endereço IP público.
- **Função enviarMensagem():** Envia a mensagem de socorro via API do CallMeBot.
- **Função urlencode():** Codifica a mensagem em formato URL para envio via GET.

## Contribuições

Se você deseja contribuir para este projeto, sinta-se à vontade para criar um fork, fazer alterações e enviar pull requests com suas melhorias.
