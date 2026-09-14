#include "e18_sensor.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define E18_BOM_PIN  GPIO_NUM_4
#define E18_RUIM_PIN GPIO_NUM_19

#define SENSOR_BLOCK_TIME_MS 2000

static TickType_t tempo_ultima_deteccao_bom = 0;
static TickType_t tempo_ultima_deteccao_ruim = 0;

void e18_sensor_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << E18_BOM_PIN) |
                        (1ULL << E18_RUIM_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);
}

bool e18_sensor_bom_detected(void)
{
    TickType_t agora = xTaskGetTickCount();

    bool sensor_ativo = (gpio_get_level(E18_BOM_PIN) == 0);

    if (!sensor_ativo) {
        return false;
    }

    if ((agora - tempo_ultima_deteccao_bom) >=
        pdMS_TO_TICKS(SENSOR_BLOCK_TIME_MS))
    {
        tempo_ultima_deteccao_bom = agora;
        return true;
    }

    return false;
}

bool e18_sensor_ruim_detected(void)
{
    TickType_t agora = xTaskGetTickCount();

    bool sensor_ativo = (gpio_get_level(E18_RUIM_PIN) == 0);

    if (!sensor_ativo) {
        return false;
    }

    if ((agora - tempo_ultima_deteccao_ruim) >=
        pdMS_TO_TICKS(SENSOR_BLOCK_TIME_MS))
    {
        tempo_ultima_deteccao_ruim = agora;
        return true;
    }

    return false;
}