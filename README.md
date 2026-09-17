# Industrial Network Pointer (INP)

O INP é uma solução de IoT desenvolvida para automatizar e otimizar o apontamento de produção em esteiras de indústrias manufatureiras. O objetivo principal é substituir o acompanhamento manual – tradicionalmente feito com pranchetas e planilhas – por um sistema de contagem automatizado e em tempo real, visando eliminar falhas humanas, evitar divergências no controle de estoque e fornecer dados confiáveis para a tomada de decisões gerenciais.

A estrutura do sistema é dividida em dois nós principais, integrados por rede Wi-Fi:

- **Primeiro Nó - Estação de Contagem:** Composto por duas esteiras transportadoras (produtos aprovados e defeituosos) equipadas com sensores infravermelhos E18-D80NK e controladas por um microcontrolador ESP32, responsável pelo processamento local da contagem e pelo envio dos dados.
- **Segundo Nó (Servidor e Dashboard):** Um servidor local que recebe, armazena e processa as informações enviadas pelo ESP32, disponibilizando um painel visual (dashboard) via interface web para o acompanhamento da produção em tempo real. 

## Sumário:
1 - Como o Sistema Funciona
2 - Dependências e Requisitos
3 - Esquemáticos Elétricos
4 - Preparação e Configuração

---

## 1. Como o Sistema Funciona?

![Imagem do fluxograma da arquitetura do projeto](https://raw.githubusercontent.com/thomasmagalhaes935-ai/industrial-network-pointer/refs/heads/main/Docs/Diagrama%20da%20Arquitetura/Diagrama%20da%20Arquitetura.jpg)


## 2 - Dependências e Requisitos

**Hardware Previsto:**
- Placa Microcontroladora ESP32 (DevKitC ou DevKit V1).
- Sensor de Barreira E18-D80NK.
- LEDs e Botões (Push-buttons) industriais.
- Cabo USB tipo C para conectar o ESP32 à sua máquina

**Software e Plataformas**
- **Ambiente de Desenvolvimento:** Visual Studio Code com a extensão ESP-IDF instalada.
- **Framework de Software:** ESP-IDF (Espressif IoT Development Framework) utilizando componentes em linguagem C/C++ para conexão Wi-Fi e cliente MQTT (`sesp_mqtts`).
- **Serviço em Nuvem:** Conta ativa no Grafana Cloud com a integração nativa MQTT habilitada.
- **Git para Windows:** Necessário para que a extensão do ESP-IDF consiga clonar as bibliotecas e frameworks da internet direto para a sua máquina.
- **Python 3:** Versão 3.8 ou superior (como a 3.11). O ESP-IDF depende de scripts em Python para compilar o código em C/C++.
- **Driver USB/Serial CP210x):** Para que a ESP32 receba uma porta "COM" e seja possível gravar o firmware.

**Bicliotecas C++ (ESP32):** 
- **`esp_wifi.h`:** Para conectividade de rede.
- **`PubSubClient.h`:** Para protocolo MQTT.
- **`driver/gpio.h`:** Para configurar os pinos do ESP32 como entrada (leitura dos pulsos do sensor E18-D80NK e botões) ou saída (acionamento dos LEDs).
- **`freertos/FreeRTOS.h` e `freertos/task.h`:** Para executar a lógica em paralelo e criar as rotinas de bloqueio (timeout).
- **`esp_timer.h`**: Para a contagem precisa de milissegundos.
- **`nvs_flash.h`:** Sistema de armazenamento não-volátil (NVS).
- **`esp_event.h`:** Gerenciador de eventos.
- **`mqtt_client.h:`** Cliente MQTT oficial da Espressif. Usado para estabelecer a conexão criptografada, montar o pacote de dados e publicar as métricas de contagem nos tópicos do Grafana Cloud.
- **`esp_log.h`:** Para imprimir mensagens no terminal do VS Code.


## 3 - Esquemáticos Elétricos

![Esquemáticos Elétricos](https://github.com/thomasmagalhaes935-ai/industrial-network-pointer/blob/main/Docs/Esquematico%20Eletrico/esquematico-eletrico.jpg?raw=true)

## 4 - Preparação e Configuração

### 4.1 - Passos Iniciais

#### **4.1.1 - Configuração da IDE**:
Instale o VS Code. Na aba de extensões, procure por "Espressif IDF" e instale. Siga o assistente de configuração (Setup Wizard) nativo da extensão para baixar o toolchain completo do ESP-IDF no seu computador. Também é necessário ter o Git, Python 3 e os drivers USB/serial da placa instalados

#### **4.1.2 - Abertura do Projeto:**
Clone o repositório do projeto e abra a pasta principal no VS Code.

#### **4.1.3 - Seleção de Target e Porta:**
Na barra inferior do VS Code, utilize as opções do ESP-IDF para solucionar o modelo da placa e a porta serial utilizada. Para o protótipo com ESP32, o target deve ser configurado como: idf.py set-target ESP32. Selecione tambem a porta COM correspondente à placa conectada ao computador. 

#### **4.1.4 - Credenciais de Rede e Nuvem (Menuconfig):**
Abra o menu_config no terminal do projeto, e escolha a opção "Component config -> Configuração do Wi-Fi -->" e configure o nome da rede e a senha que será utilizado para fazer a conexão com o ESP32, a conexão com o mqtt falhará se os dados credenciais da rede estiverem incorretos.

### **4.2 - Grafana Cloud MQTT:**
No Grafana Cloud, configure a integração MQTT utilizando os dados fornecidos pelo serviço, o ESP32 atua como Publisher, enviando os dados da produção através do protocolo MQTT. Os tópicos utilizados devem corresponder aos configurados no projeto: 

#define MQTT_BROKER_URI "mqtt://mqtt-dashboard.com:1883"
#define MQTT_TOPIC "esteiras/produtos/contagem"

### **4.3 - GrafanaCompilação e Gravação:**
Após configurar o projeto, compile e grave o firmware no ESP32 utilizando o ESP-IDF. 

idf.py build
idf.py flash

Para acompanhar as mensagens e o funcionamento do sistema pelo monitor serial: 

idf.py monitor

### **4.4 - Configuração do Dashboard:**
No Grafana Cloud, crie o Dashboard responsável pela visualização dos dados recebidos via MQTT. Podem ser utilizados painéis como: Stat, Time Series e Bar chart. Os painéis devem utilizar os dados recebidos pela integração MQTT e permitir a visualização atualizada da produção.


