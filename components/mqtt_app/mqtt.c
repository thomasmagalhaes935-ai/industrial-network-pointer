#include <stdio.h>
#include "esp_log.h"
#include "mqtt_client.h"
#include "mqtt.h"

static const char *TAG = "MQTT_APP";
static esp_mqtt_client_handle_t client = NULL;
static bool is_connected = false;

#define MQTT_BROKER_URI "mqtt://mqtt-dashboard.com:1883"
#define MQTT_TOPIC      "esteira/produtos/contagem"

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "Conectado ao Broker MQTT com sucesso!");
        is_connected = true;
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "Desconectado do Broker MQTT.");
        is_connected = false;
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGE(TAG, "Erro no evento MQTT. Tipo de erro: %d", event->error_handle->error_type);
        break;
    default:
        break;
    }
}

void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URI,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

bool mqtt_is_connected(void)
{
    return is_connected;
}

void mqtt_publish_count(int count)
{
    if (!is_connected || client == NULL) {
        ESP_LOGW(TAG, "MQTT não está conectado. Mensagem não enviada.");
        return;
    }

    // Formato JSON enviado para o broker
    char payload[64];
    snprintf(payload, sizeof(payload), "{\"quantidade\": %d}", count);

    int msg_id = esp_mqtt_client_publish(client, MQTT_TOPIC, payload, 0, 1, 0);
    ESP_LOGI(TAG, "Mensagem publicada (ID: %d): %s", msg_id, payload);
}