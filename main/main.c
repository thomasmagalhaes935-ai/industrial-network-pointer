#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"

// Componentes do projeto
#include "e18_sensor.h"
#include "counter.h"
#include "button.h"
#include "Leds.h"
#include "mqtt.h"

static const char *TAG = "MAIN";
static EventGroupHandle_t s_wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0

static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGW(TAG, "Tentando conectar ao Wi-Fi...");
        xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Conectado! IP obtido: " IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

// Task separada para gerenciar a rede em segundo plano
void network_task(void *pvParameters)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = CONFIG_WIFI_SSID,
            .password = CONFIG_WIFI_PASSWORD,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    // Aguarda conexão para iniciar MQTT (não trava o sensor)
    xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    mqtt_app_start();

    vTaskDelete(NULL);
}

void app_main(void)
{
    // 1. Inicializa periféricos locais (funcionalidade garantida de imediato)
    e18_sensor_init();
    counter_init();
    leds_init();
    button_init();

    // 2. Memória NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 3. Cria a Task da rede em segundo plano (paralela)
    xTaskCreate(network_task, "network_task", 4096, NULL, 5, NULL);

    bool sensor_ligado = true;

    while (1)
    {
        if (button_was_pressed())
        {
            sensor_ligado = !sensor_ligado;

            if (sensor_ligado)
            {
                printf("SENSOR LIGADO!\n");
            }
            else
            {
                printf("SENSOR DESLIGADO!\n");
            }
        }

        if (sensor_ligado)
        {
            led_vermelho_off();
            led_verde_on();

            if (e18_sensor_detected())
            {
                counter_process_detection();

                int total_produtos = counter_get_count();
                mqtt_publish_count(total_produtos);

                led_verde_blink();
            }
        }
        else
        {
            led_verde_off();
            led_vermelho_on();
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}