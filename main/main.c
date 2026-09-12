#include "e18_sensor.h"
#include "counter.h"
#include "button.h"
#include "Leds.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

void app_main(void)
{
    e18_sensor_init();
    counter_init();
    leds_init();
    button_init();

    bool sensor_ligado = true;

    while (1)
    {
        // Verifica se houve um novo clique no botão
        if (button_was_pressed())
        {
            sensor_ligado = !sensor_ligado;

            // Mostra no monitor serial o novo estado do sensor
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
            // Sensor ligado
            led_vermelho_off();
            led_verde_on();

            // Verifica se um produto foi detectado
            if (e18_sensor_detected())
            {
                counter_process_detection();

                // Pisca o LED verde
                led_verde_blink();
            }
        }
        else
        {
            // Sensor desligado
            led_verde_off();
            led_vermelho_on();
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}