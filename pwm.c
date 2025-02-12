#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PWM_LED 12 // Pino 22 - Servo (wokwi) e Pino 12 - Led rgb (placa bitdoglab)

const uint16_t WRAP_PERIOD = 50000;     // Valor do WRAP - máxima contagem
const float PWM_DIVISER = 50.0;         // Divisor do clock

// Enum para representar os estados do duty cycle
typedef enum {
    DUTY_CYCLE_LOW = 1250,    // 2.5%
    DUTY_CYCLE_MEDIUM = 3675, // 7.35%
    DUTY_CYCLE_HIGH = 6000    // 12%
} DutyCycle;

// Configuração do PWM
void pwm_setup()
{
    gpio_set_function(PWM_LED, GPIO_FUNC_PWM);      // Função para habilitar o pino escolhido como PWM

    uint slice = pwm_gpio_to_slice_num(PWM_LED);    // Obtenção do canal PWM - Slice

    pwm_set_clkdiv(slice, PWM_DIVISER);             // Definindo o divisor para o slice

    pwm_set_wrap(slice, WRAP_PERIOD);               // Definindo o valor do contador

    pwm_set_enabled(slice, true);                   // Habilitando o PWM no slice

    int freqPWM = 125000000 / ((PWM_DIVISER) * (WRAP_PERIOD)); // Calculando o valor da frequência do PWM

    printf("Frequência do PWM ajustada para %i Hz\n", freqPWM); // Imprimindo o valor da frequência
}

int main()
{
    stdio_init_all();
    
    pwm_setup();

    sleep_ms(1000); // Atraso de 1 segundo para mudança no Duty Cycle
    printf("Sistema inicializado.\n");

    printf("Duty Cycle em 12%%.\n");
    pwm_set_gpio_level(PWM_LED, DUTY_CYCLE_HIGH); // Definindo o duty cycle de 12%

    sleep_ms(5000); // Atraso de 5 segundos para a próxima mudança

    printf("Duty Cycle em 7.35%%.\n");
    pwm_set_gpio_level(PWM_LED, DUTY_CYCLE_MEDIUM); // Definindo o duty cycle de 7.35%

    sleep_ms(5000); // Atraso de 5 segundos para a próxima mudança 

    printf("Duty Cycle em 2.5%%.\n");
    pwm_set_gpio_level(PWM_LED, DUTY_CYCLE_LOW); // Definindo o duty cycle de 2.5%

    sleep_ms(5000); // Atraso de 5 segundos para a próxima mudança

    printf("Variando Duty Cycle entre 2.5%% e 12%%.\n");

    double level = DUTY_CYCLE_LOW;
    int control = 1;

    while (true) { //Loop principal alternando entre 2.5% e 12.5% o Duty Cycle

        if (control == 1) { //Aumentando Duty Cycle
            pwm_set_gpio_level(PWM_LED, level);
            level += 4.75;
            sleep_ms(1);
            
            if (level >= DUTY_CYCLE_HIGH) {
                control = 0;
            }
        } else if (control == 0) { //Diminuindo Duty Cycle
            pwm_set_gpio_level(PWM_LED, level);
            level -= 4.75;
            sleep_ms(1);

            if (level <= DUTY_CYCLE_LOW) {
                control = 1;
            }
        }
    }
}
