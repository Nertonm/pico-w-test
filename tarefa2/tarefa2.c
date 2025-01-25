#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/timer.h"

#define BUTTON_PIN  5
#define LED_PIN  13
#define REBOOT_PIN  6

// Variáveis globais
volatile int button_count = 0;
volatile bool blink = false; 

bool timer_callback(repeating_timer_t *rt) {
    static bool led_state = false;
    gpio_put(LED_PIN, led_state);
    led_state = !led_state;
    return blink;
}

int main()
{
    stdio_init_all();
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    gpio_init(REBOOT_PIN);
    gpio_set_dir(REBOOT_PIN, GPIO_IN);
    gpio_pull_up(REBOOT_PIN);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    repeating_timer_t temporizador;

    while (true) {
        if (gpio_get(BUTTON_PIN) == 0) {
            sleep_ms(50);
            if (gpio_get(BUTTON_PIN) == 0) {
                button_count++;
                printf("Button pressed %d times\n", button_count);
                while (gpio_get(BUTTON_PIN) == 0)
                {
                    sleep_ms(50);
                }
            }
        }
        if (button_count == 5){
            blink = true;
            add_repeating_timer_ms(-100, timer_callback, NULL, &temporizador);
            sleep_ms(10000);
            cancel_repeating_timer(&temporizador);
            gpio_put(LED_PIN, 0); 
            button_count = 0;
            blink = false;
        }
        if (gpio_get(REBOOT_PIN) == 0){
            sleep_ms(50);
            if (gpio_get(REBOOT_PIN) == 0)
            {
                printf("Rebooting...\n");
                sleep_ms(1000);
                reset_usb_boot(0, 0);
            }
        }
    }
    return 0;
}

