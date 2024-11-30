# Maloca-Hands-On-Basic (Botão de Pânico com Buzzer)

Este projeto utiliza o microcontrolador ESP32 para criar um sistema de alerta de emergência. O sistema, ao ser ativado por meio de um botão, envia mensagens de socorro via WhatsApp para números predefinidos, utilizando a API do CallMeBot. Além disso, um buzzer é acionado para alertar os usuários locais sobre a emergência, complementando os alertas enviados pelas mensagens.

## Funcionalidades

- **Envio de mensagens de socorro via WhatsApp**: Quando o botão é pressionado, o sistema envia uma mensagem de alerta ("SOCORRO!!! ME AJUDE") para dois números de telefone cadastrados.
- **Alerta sonoro**: Um buzzer é acionado sempre que o botão é pressionado, gerando um som para alertar os usuários locais.
- **Indicação de status de conexão Wi-Fi**: O sistema pisca um LED interno para indicar que está tentando se conectar à rede Wi-Fi e acende quando a conexão é bem-sucedida.
- **Indicação do envio de mensagem**: Um LED externo acende quando a mensagem está sendo enviada, proporcionando feedback visual.

## Hardware: 
- ESP32 (ou qualquer outro microcontrolador compatível com a biblioteca WiFi)
- Botão
- LED externo
- LED interno (do próprio ESP32)
- Buzzer
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
- **Função setup():** Configura os pinos, inicializa a comunicação serial, conecta-se ao Wi-Fi e indica o status da conexão com LEDs.
- **Função loop():** Monitora o botão e, ao ser pressionado, aciona o envio da mensagem de socorro e o alerta sonoro com o buzzer.

## Contribuições

Se você deseja contribuir para este projeto, sinta-se à vontade para criar um fork, fazer alterações e enviar pull requests com suas melhorias.
