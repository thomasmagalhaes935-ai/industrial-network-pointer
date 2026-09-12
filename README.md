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

## 1 - Como o Sistema Funciona

graph TD
    %% Sensoriamento e Entrada
    subgraph "Nó 1: Chão de Fábrica e Sensoriamento"
        E1[Esteira 1 + Funil] -->|Produto Conforme| S1(Sensor E18-D80NK)
        E2[Esteira 2 + Funil] -->|Produto Defeituoso| S2(Sensor E18-D80NK)
    end

    %% Processamento
    subgraph "Nó 1: Processamento de Borda (Edge)"
        S1 -. Sinal Digital .-> ESP32{Placa ESP32}
        S2 -. Sinal Digital .-> ESP32
        ESP32 -->|ESP-IDF C/C++| Logica[Filtro Debounce / Contadores]
    end

    %% Conectividade
    subgraph "Rede e Protocolo"
        Logica -->|Conexão Wi-Fi Local| MQTT_Pub[Publicação via Protocolo MQTT]
    end

    %% Software e Aplicação
    subgraph "Nó 2: Nuvem e Software"
        MQTT_Pub == Dados Estruturados ==> Grafana_Ingest(Grafana Cloud MQTT Integration)
        Grafana_Ingest --> TSDB[(Banco de Dados de Séries Temporais)]
        TSDB --> Dashboard([Dashboard Gerencial em Tempo Real])
    end

    %% Estilização para o GitHub
    style ESP32 fill:#ffcc00,stroke:#333,stroke-width:2px
    style Dashboard fill:#0099ff,stroke:#333,stroke-width:2px
    style S1 fill:#00cc66,stroke:#333
    style S2 fill:#ff3333,stroke:#333

**Fluxo da Arquitetura:**

1. **Sensoriamento (Entrada):** Sensores Infravermelhos E18-D80NK detectam a passagem das peças.
3. **Processamento (Edge):** Microcontrolador ESP32 processa os pulsos e aplica lógicas de timeout.
4. **Conectividade:** ESP32 transmite dados estruturados via rede Wi-Fi (Protocolo MQTT).
5. **Aplicação (Saída):** Servidor recebe os dados e renderiza os indicadores em um Dashboard gerencial via Gravana Cloud.

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
(Versão final do esquemático em desenvolvimento)

## 4 - Preparação e Configuração

### 4.1 - Passos Iniciais

#### **4.1.1 - Configuração da IDE**:
Instale o VS Code. Na aba de extensões, procure por "Espressif IDF" e instale. Siga o assistente de configuração (Setup Wizard) nativo da extensão para baixar o toolchain completo do ESP-IDF no seu computador.

#### **4.1.2 - Abertura do Projeto:**
Clone o repositório do projeto e abra a pasta principal no VS Code.

#### **4.1.3 - Seleção de Target e Porta:**
Na barra inferior do VS Code (barra do ESP-IDF), clique no ícone da placa e defina o target como esp32. Em seguida, clique no ícone de tomada para selecionar a porta COM onde sua placa está conectada.

#### **4.1.4 - Credenciais de Rede e Nuvem (Menuconfig):**
Clique no ícone de engrenagem (SDK Configuration Editor / menuconfig) na barra inferior. Navegue até as configurações do projeto (ou edite diretamente o arquivo de cabeçalho designado, como config.h) e insira:

    Wi-Fi: SSID e Senha da rede local.

### **4.2 - Grafana Cloud MQTT:**
Anote as informações de URL (Endpoint), Porta, Username e Password fornecidos pelo painel de integração do Grafana.

### **4.3 - GrafanaCompilação e Gravação:**
Utilize os atalhos da barra do ESP-IDF: clique em Build (ícone de lixeira/fogo) para compilar o código em C, e depois em Flash (ícone de raio) para gravar o firmware no ESP32.

### **4.4 - Configuração do Dashboard:**
Acesse o painel web do Grafana Cloud, crie um novo Dashboard, adicione painéis do tipo Time Series ou Stat e configure as queries para escutar os tópicos MQTT publicados pelo seu ESP32.


