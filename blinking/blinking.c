#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"

const uint led_pin_red = 12;
const uint button_a = 5;

int main()
{
    uint a = 100;
    gpio_init(led_pin_red);
    gpio_set_dir(led_pin_red, GPIO_OUT);
    gpio_init(button_a);
    gpio_set_dir(button_a, GPIO_IN);
    gpio_pull_up(button_a); // Habilita o pull-up interno para o botão
    stdio_init_all();
    while (true) {
        a++;
        if (a % 2)
        {   
            printf("Blingking");
            gpio_put(led_pin_red, 1);
            sleep_ms(1000);
            gpio_put(led_pin_red, 0);
            sleep_ms(1000);
        }
        if (gpio_get(button_a) == 0) {
            // Entra no modo BOOTSEL
            reset_usb_boot(0, 0);
        }
    }
}
