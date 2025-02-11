#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

#define PWM_LED 22      // Pino que será configurado como PWM. 22 - Servo (Simulação WokWi); 12 - LED (Placa BitDogLab)
#define LED_PIN 12      // Pino do LED azul

const uint16_t WRAP_PERIOD = 50000;     // Valor do WRAP - máxima contagem
const float PWM_DIVISER = 50.0;         // Divisor do clock

// Enum para representar o estado de controle do servomotor
typedef enum {
    CONTROL_INCREASING,  // Estado: aumentando o duty cycle
    CONTROL_DECREASING   // Estado: diminuindo o duty cycle
} ControlState;

// Configuração do PWM
void pwm_setup() {
    gpio_set_function(PWM_LED, GPIO_FUNC_PWM);      // Função para habilitar o pino escolhido como PWM

    uint slice = pwm_gpio_to_slice_num(PWM_LED);    // Obtenção do canal PWM - Slice

    pwm_set_clkdiv(slice, PWM_DIVISER);             // Definindo o divisor para o slice
    pwm_set_wrap(slice, WRAP_PERIOD);               // Definindo o valor do contador
    pwm_set_enabled(slice, true);                   // Habilitando o PWM no slice

    int freqPWM = 125000000 / ((PWM_DIVISER) * (WRAP_PERIOD)); // Calculando o valor da frequência do PWM
    printf("Frequência do PWM = %i Hz\n", freqPWM); // Imprimindo o valor da frequência
}

// Configuração do LED
void led_setup() {
    gpio_init(LED_PIN);                  // Inicializa o pino do LED
    gpio_set_dir(LED_PIN, GPIO_OUT);     // Configura o pino como saída
    gpio_put(LED_PIN, 0);                // Inicializa o LED desligado
}

// Função para acender o LED por meio segundo
void blink_led() {
    gpio_put(LED_PIN, 1);                // Liga o LED
    sleep_ms(500);                       // Espera 500 ms
    gpio_put(LED_PIN, 0);                // Desliga o LED
}

int main() {
    stdio_init_all();
    
    pwm_setup();
    led_setup();

    printf("Sistema inicializado.\n");

    sleep_ms(1000); // Atraso de 1 segundo para mudança no Duty Cycle

    printf("Duty Cycle em 12%%.\n");
    pwm_set_gpio_level(PWM_LED, 6000); // Definindo o duty cycle de 12%
    blink_led(); // Acende o LED por meio segundo

    sleep_ms(5000); // Atraso de 5 segundos

    printf("Duty Cycle em 7.35%%.\n");
    pwm_set_gpio_level(PWM_LED, 3675); // Definindo o duty cycle de 7.35%
    blink_led(); // Acende o LED por meio segundo

    sleep_ms(5000); // Atraso de 5 segundos

    printf("Duty Cycle em 2.5%%.\n");
    pwm_set_gpio_level(PWM_LED, 1250); // Definindo o duty cycle de 2.5%
    blink_led(); // Acende o LED por meio segundo

    sleep_ms(5000); // Atraso de 5 segundos

    printf("Variação do Duty Cycle de 2.5%% até 12%%.\n");

    double level = 1250;
    ControlState control = CONTROL_INCREASING; // Inicializa o estado como "aumentando"

    while (true) {
        if (control == CONTROL_INCREASING) {
            pwm_set_gpio_level(PWM_LED, (uint16_t)level); // Conversão explícita para uint16_t
            level += 4.75;
            sleep_ms(1);
            
            if (level >= 6000) {
                control = CONTROL_DECREASING; // Muda o estado para "diminuindo"
                blink_led(); // Acende o LED por meio segundo ao atingir o limite superior
            }
        } else if (control == CONTROL_DECREASING) {
            pwm_set_gpio_level(PWM_LED, (uint16_t)level); // Conversão explícita para uint16_t
            level -= 4.75;
            sleep_ms(1);

            if (level <= 1250) {
                control = CONTROL_INCREASING; // Muda o estado para "aumentando"
                blink_led(); // Acende o LED por meio segundo ao atingir o limite inferior
            }
        }
    }
}