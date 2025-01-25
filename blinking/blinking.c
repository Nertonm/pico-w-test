#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/pwm.h"

const uint led_pin_red = 12;
const uint button_a = 5;


void set_pwm_duty_cycle(uint slice_num, uint channel, uint16_t duty_cycle) {
    pwm_set_gpio_level(led_pin_red, duty_cycle);
}

int main()
{
    uint a = 100;
    gpio_set_function(led_pin_red, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(led_pin_red);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f); // Ajuste a frequência do PWM
    pwm_init(slice_num, &config, true);
    
    gpio_init(button_a);
    gpio_set_dir(button_a, GPIO_IN);
    gpio_pull_up(button_a); // Habilita o pull-up interno para o botão
    stdio_init_all();

    while (true) {
        a++;
        if (a % 2) {   
            printf("Blinking\n");
            set_pwm_duty_cycle(slice_num, PWM_CHAN_A, 68); // 50% de intensidade
            sleep_ms(1000);
            set_pwm_duty_cycle(slice_num, PWM_CHAN_A, 399); // LED desligado
            sleep_ms(1000);
        }
        if (gpio_get(button_a) == 0) {
            // Entra no modo BOOTSEL
            reset_usb_boot(0, 0);
        }
    }
}