#include "Button.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUTTON_GPIO GPIO_NUM_7

void button_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);
}

bool button_is_pressed(void)
{
    return gpio_get_level(BUTTON_GPIO) == 0;
}

bool button_was_pressed(void)
{
    static bool estado_anterior = false;

    bool estado_atual = button_is_pressed();
    bool novo_clique = false;

    if (estado_atual && !estado_anterior)
    {
        novo_clique = true;
    }

    estado_anterior = estado_atual;

    return novo_clique;
}