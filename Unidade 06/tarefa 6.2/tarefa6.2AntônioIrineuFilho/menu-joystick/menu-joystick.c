#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "ssd1306.h"

#define I2C_PORT i2c1
#define ADC_CHANNEL_X 1
#define ADC_CHANNEL_Y 0
#define LED_G 11
#define LED_B 12
#define LED_R 13
#define PINO_SCL 14
#define PINO_SDA 15
#define BUZZER_PIN 21
#define JOYSTICK_BTN 22
#define VRX 26
#define VRY 27

ssd1306_t display;

void setup_oled_joystick(){
    stdio_init_all();

    // OLED
    i2c_init(I2C_PORT, 400*1000);// I2C Inicialização. Usando 400Khz.
    gpio_set_function(PINO_SCL, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SCL);
    gpio_pull_up(PINO_SDA);
    display.external_vcc=false;
    ssd1306_init(&display, 128, 64, 0x3C, I2C_PORT);

    // JOYSTICK
    adc_init();
    adc_gpio_init(VRX);
    adc_gpio_init(VRY);
    gpio_init(JOYSTICK_BTN);
    gpio_set_dir(JOYSTICK_BTN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BTN);
}

void draw(char *text, int col, int row) {
    ssd1306_draw_string(&display, col, row, 1, text);
    ssd1306_show(&display);
}

void menu(int row, bool clear) {
    if (clear) {
        ssd1306_clear(&display);
    }
    draw("-> ", 5, row);
    draw("JOYSTICK LED", 23, 10);
    draw("TOCAR BUZZER", 23, 30);
    draw("LIGAR RGB", 23, 50);    
}

bool button_state() {
    for (int i = 0; i < 10; i++) {
        int btn = gpio_get(JOYSTICK_BTN);
        if (btn == 0) {
            return true;
        }
        sleep_ms(10);
    }
    return false;
}

// Joystick OLED

float DIVIDER_PWM = 16.0;          // Divisor fracional do clock para o PWM
uint16_t PERIOD = 4096;            // Período do PWM (valor máximo do contador)
uint16_t led_b_level, led_r_level = 100; // Inicialização dos níveis de PWM para os LEDs
uint slice_led_b, slice_led_r;           // Variáveis para armazenar os slices de PWM correspondentes aos LEDs

// Função para configurar o PWM de um LED (genérica para azul e vermelho)
void setup_pwm_led(uint led, uint *slice, uint16_t level) {
  gpio_set_function(led, GPIO_FUNC_PWM); // Configura o pino do LED como saída PWM
  *slice = pwm_gpio_to_slice_num(led);   // Obtém o slice do PWM associado ao pino do LED
  pwm_set_clkdiv(*slice, DIVIDER_PWM);   // Define o divisor de clock do PWM
  pwm_set_wrap(*slice, PERIOD);          // Configura o valor máximo do contador (período do PWM)
  pwm_set_gpio_level(led, level);        // Define o nível inicial do PWM para o LED
  pwm_set_enabled(*slice, true);         // Habilita o PWM no slice correspondente ao LED
}

// Função de configuração geral
void pwm_red_blue() {
  setup_pwm_led(LED_B, &slice_led_b, led_b_level); // Configura o PWM para o LED azul
  setup_pwm_led(LED_R, &slice_led_r, led_r_level); // Configura o PWM para o LED vermelho
}

// Função para ler os valores dos eixos do joystick (X e Y)
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value) {
  // Leitura do valor do eixo X do joystick
  adc_select_input(ADC_CHANNEL_X); // Seleciona o canal ADC para o eixo X
  sleep_us(2);                     // Pequeno delay para estabilidade
  *vrx_value = adc_read();         // Lê o valor do eixo X (0-4095)

  // Leitura do valor do eixo Y do joystick
  adc_select_input(ADC_CHANNEL_Y); // Seleciona o canal ADC para o eixo Y
  sleep_us(2);                     
  *vry_value = adc_read();         
}

void pwm_main() {
    uint16_t vrx_value, vry_value, sw_value; // Variáveis para armazenar os valores do joystick (eixos X e Y) e botão
    pwm_red_blue();                          // Chama a função de configuração
    printf("Joystick-PWM\n");                // Exibe uma mensagem inicial via porta serial

    while (true) {
        if (button_state()) {
            pwm_set_gpio_level(LED_B, 0);
            pwm_set_gpio_level(LED_R, 0);
            return;
        }
        joystick_read_axis(&vrx_value, &vry_value); // Lê os valores dos eixos do joystick
        // Ajusta os níveis PWM dos LEDs de acordo com os valores do joystick
        pwm_set_gpio_level(LED_B, vrx_value); // Ajusta o brilho do LED azul com o valor do eixo X
        pwm_set_gpio_level(LED_R, vry_value); // Ajusta o brilho do LED vermelho com o valor do eixo Y
        // Pequeno delay antes da próxima leitura
        sleep_ms(100);
    }
}


// BUZZER

// Notas musicais para a música tema de Star Wars
uint star_wars_notes[] = {
    330, 330, 330, 262, 392, 523, 330, 262,
    392, 523, 330, 659, 659, 659, 698, 523,
    415, 349, 330, 262, 392, 523, 330, 262,
    392, 523, 330, 659, 659, 659, 698, 523,
    415, 349, 330, 523, 494, 440, 392, 330,
    659, 784, 659, 523, 494, 440, 392, 330,
    659, 659, 330, 784, 880, 698, 784, 659,
    523, 494, 440, 392, 659, 784, 659, 523,
    494, 440, 392, 330, 659, 523, 659, 262,
    330, 294, 247, 262, 220, 262, 330, 262,
    330, 294, 247, 262, 330, 392, 523, 440,
    349, 330, 659, 784, 659, 523, 494, 440,
    392, 659, 784, 659, 523, 494, 440, 392
};

// Duração das notas em milissegundos
uint note_duration[] = {
    500, 500, 500, 350, 150, 300, 500, 350,
    150, 300, 500, 500, 500, 500, 350, 150,
    300, 500, 500, 350, 150, 300, 500, 350,
    150, 300, 650, 500, 150, 300, 500, 350,
    150, 300, 500, 150, 300, 500, 350, 150,
    300, 650, 500, 350, 150, 300, 500, 350,
    150, 300, 500, 500, 500, 500, 350, 150,
    300, 500, 500, 350, 150, 300, 500, 350,
    150, 300, 500, 350, 150, 300, 500, 500,
    350, 150, 300, 500, 500, 350, 150, 300,
};

// Inicializa o PWM no pino do buzzer
void pwm_init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f); // Ajusta divisor de clock
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0); // Desliga o PWM inicialmente
}

// Toca uma nota com a frequência e duração especificadas
void play_tone(uint pin, uint frequency, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t top = clock_freq / frequency - 1;

    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(pin, top / 2); // 50% de duty cycle

    sleep_ms(duration_ms);

    pwm_set_gpio_level(pin, 0); // Desliga o som após a duração
    sleep_ms(50); // Pausa entre notas
}

// Função principal para tocar a música
int play_star_wars(uint pin) {
    for (int i = 0; i < sizeof(star_wars_notes) / sizeof(star_wars_notes[0]); i++) {
        if (button_state()) { return 0; }
        if (star_wars_notes[i] == 0) {
            sleep_ms(note_duration[i]);
        } else {
            play_tone(pin, star_wars_notes[i], note_duration[i]);
        }
    }
    return 1;
}

void buzzer_main() {
    pwm_init_buzzer(BUZZER_PIN);
    while(true) {
        int break_buzzer = play_star_wars(BUZZER_PIN);
        if (break_buzzer == 0) { return; }
    }
}

// LED RGB

uint16_t PERIOD_V2= 2000; // Período do PWM (valor máximo do contador)
uint16_t LED_STEP = 100;  // Passo de incremento/decremento para o duty cycle do LED
uint16_t led_level = 100; // Nível inicial do PWM (duty cycle)

void pwm_led_rgb() {
    uint slice;
    gpio_set_function(LED_B, GPIO_FUNC_PWM); // Configura o pino do LED para função PWM
    slice = pwm_gpio_to_slice_num(LED_B);    // Obtém o slice do PWM associado ao pino do LED
    pwm_set_clkdiv(slice, DIVIDER_PWM);    // Define o divisor de clock do PWM
    pwm_set_wrap(slice, PERIOD_V2);           // Configura o valor máximo do contador (período do PWM)
    pwm_set_gpio_level(LED_B, led_level);    // Define o nível inicial do PWM para o pino do LED
    pwm_set_enabled(slice, true);          // Habilita o PWM no slice correspondente
}

void led_rgb_main() {
    uint up_down = 1; // Variável para controlar se o nível do LED aumenta ou diminui
    pwm_led_rgb();    // Configura o PWM
    while (true) {
        pwm_set_gpio_level(LED_B, led_level); // Define o nível atual do PWM (duty cycle)
        for (int i = 0; i < 30; i++) {
            if (button_state()) {
                pwm_set_gpio_level(LED_B, 0); 
                return; 
            }
            sleep_ms(10);
        }
        if (up_down) {
            led_level += LED_STEP; // Incrementa o nível do LED
            if (led_level >= PERIOD_V2) {
                up_down = 0; // Muda direção para diminuir quando atingir o período máximo
            }
        } else {
            led_level -= LED_STEP; // Decrementa o nível do LED
            if (led_level <= LED_STEP) {
                up_down = 1; // Muda direção para aumentar quando atingir o mínimo
            }
        }
    }
}

int main() {

    setup_oled_joystick();
    menu(10, true);
    int selected = 1;
    while(true) {
        if (button_state()) {
            if (selected == 1) {
                sleep_ms(500); 
                pwm_main(); 
            }
            else if (selected == 2) { 
                sleep_ms(500);
                buzzer_main(); 
            } 
            else if (selected == 3) { 
                sleep_ms(500);
                led_rgb_main(); 
            }
            sleep_ms(500);
        };
        adc_select_input(ADC_CHANNEL_Y); // Seleciona o eixo Y do joystick como input
        uint adc_y_raw = adc_read(); // lê o ADC para o eixo Y
        if (adc_y_raw < 500) {
            if (selected == 1) {
                menu(30, true);
                selected++;
            } else if (selected == 2) {
                menu(50, true);
                selected++;
            } else if (selected == 3) {
                menu(10, true);
                selected = 1;
            }
        } else if (adc_y_raw > 4000) {
            if (selected == 2) {
                menu(10, true);
                selected--;
            } else if (selected == 3) {
                menu(30, true);
                selected--;
            }
        }
    }
    return 0;

}