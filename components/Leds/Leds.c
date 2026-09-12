#include "Leds.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_VERDE_GPIO GPIO_NUM_5
#define LED_VERMELHO_GPIO GPIO_NUM_6

void leds_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_VERDE_GPIO) |
                        (1ULL << LED_VERMELHO_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);

    // Estado inicial:
    // sensor ligado → verde aceso
    // vermelho apagado
    gpio_set_level(LED_VERDE_GPIO, 1);
    gpio_set_level(LED_VERMELHO_GPIO, 0);
}

void led_verde_on(void)
{
    gpio_set_level(LED_VERDE_GPIO, 1);
}

void led_verde_off(void)
{
    gpio_set_level(LED_VERDE_GPIO, 0);
}

void led_vermelho_on(void)
{
    gpio_set_level(LED_VERMELHO_GPIO, 1);
}

void led_vermelho_off(void)
{
    gpio_set_level(LED_VERMELHO_GPIO, 0);
}

void led_verde_blink(void)
{
    led_verde_off();

    vTaskDelay(pdMS_TO_TICKS(200));

    led_verde_on();
}