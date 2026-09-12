#include "e18_sensor.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define E18_PIN GPIO_NUM_4

#define SENSOR_BLOCK_TIME_MS 2000

static TickType_t tempo_ultima_deteccao = 0;

void e18_sensor_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << E18_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);
}

bool e18_sensor_is_active(void)
{
    return gpio_get_level(E18_PIN) == 0;
}

bool e18_sensor_detected(void)
{
    TickType_t agora = xTaskGetTickCount();

    if (!e18_sensor_is_active())
    {
        return false;
    }

    if ((agora - tempo_ultima_deteccao) >=
        pdMS_TO_TICKS(SENSOR_BLOCK_TIME_MS))
    {
        tempo_ultima_deteccao = agora;

        return true;
    }

    return false;
}