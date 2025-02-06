#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "ssd1306.h"

#define I2C_PORT i2c1
#define PINO_SCL 14
#define PINO_SDA 15
#define VRX 26
#define VRY 27
#define ADC_CHANNEL_X 1
#define ADC_CHANNEL_Y 0
#define JOYSTICK_BTN 22

ssd1306_t display;

void setup(){
    stdio_init_all();

    // OLED
    i2c_init(I2C_PORT, 400*1000);// I2C Inicialização. Usando 400Khz.
    gpio_set_function(PINO_SCL, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SCL);
    gpio_pull_up(PINO_SDA);
    display.external_vcc=false;
    ssd1306_init(&display, 128, 64, 0x3C, I2C_PORT);

    //JOYSTICK
    adc_init();
    adc_gpio_init(VRX);
    adc_gpio_init(VRY);
    gpio_init(JOYSTICK_BTN);
    gpio_set_dir(JOYSTICK_BTN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BTN);
}

void draw(char *text, int row, bool clear) {
    if (clear) {
        ssd1306_clear(&display);
    }
    ssd1306_draw_string(&display, 8, row, 1, text);
    ssd1306_show(&display);
}

void clear() {
    ssd1306_clear(&display);
}

int main() {
    setup();
    int selected = 1;
    while(true) {
        adc_select_input(ADC_CHANNEL_Y); // Seleciona o eixo Y do joystick como input
        uint adc_y_raw = adc_read(); // lê o ADC para o eixo Y
        if (adc_y_raw > 4000) {
            if (selected == 1) {
                selected++;
            } else if (selected == 2) {
                selected++;
            } else if (selected == 3) {
                selected = 1;
            }
        } else if (adc_y_raw < 100) {
            if (selected == 2) {
                selected--;
            } else if (selected == 3) {
                selected--;
            }
        }

    }


}
