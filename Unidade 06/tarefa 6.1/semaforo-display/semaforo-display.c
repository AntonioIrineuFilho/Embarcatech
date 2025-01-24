#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

#define I2C_PORT i2c1
#define PINO_SCL 14
#define PINO_SDA 15
#define LED_R_PIN 13
#define LED_G_PIN 11
#define LED_B_PIN 12
#define BTN_A_PIN 5

ssd1306_t display;
int A_state = 0;

void setup() {

    stdio_init_all();
    i2c_init(I2C_PORT, 400*1000);// I2C Inicialização. Usando 400Khz.
    gpio_set_function(PINO_SCL, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SCL);
    gpio_pull_up(PINO_SDA);
    display.external_vcc=false;
    ssd1306_init(&display, 128, 64, 0x3C, I2C_PORT);

    // INICIANDO LEDS
    gpio_init(LED_R_PIN);
    gpio_set_dir(LED_R_PIN, GPIO_OUT);
    gpio_init(LED_G_PIN);
    gpio_set_dir(LED_G_PIN, GPIO_OUT);
    gpio_init(LED_B_PIN);
    gpio_set_dir(LED_B_PIN, GPIO_OUT);

    // INICIANDO BOTÄO
    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN);

}

void printTexto(char *msg, int linha) {
    ssd1306_draw_string(&display, 8, linha, 1, msg); //  desenha o texto
    ssd1306_show(&display); // mostra no display
    }

void limparTexto() {
    ssd1306_clear(&display);
}

void SinalAberto(){
    gpio_put(LED_R_PIN, 0);
    gpio_put(LED_G_PIN, 1);
    gpio_put(LED_B_PIN, 0);   
}

void SinalAtencao(){
    gpio_put(LED_R_PIN, 1);
    gpio_put(LED_G_PIN, 1);
    gpio_put(LED_B_PIN, 0);
}

void SinalFechado(){
    gpio_put(LED_R_PIN, 1);
    gpio_put(LED_G_PIN, 0);
    gpio_put(LED_B_PIN, 0);
}

int WaitWithRead(int timeMS){
    for(int i = 0; i < timeMS; i = i+100){
        A_state = !gpio_get(BTN_A_PIN);
        if(A_state == 1){
            return 1;
        }
        sleep_ms(100);
    }
    return 0;
}


int main() {

    setup();
    char *text = "";

    while(true) {
        
       SinalFechado();
       text = "SINAL FECHADO";
       printTexto(text, 10);
       text = "- AGUARDE";
       printTexto(text, 30);
       A_state = WaitWithRead(8000); // espera com leitura do botäo
       // sleep_ms(8000);
       if (A_state) { 
        limparTexto();
        SinalAtencao();
        text = "SINAL DE ATENCAO";
        printTexto(text, 10);
        text = "- PREPARE-SE";
        printTexto(text, 30);
        sleep_ms(5000);
        limparTexto();
        SinalAberto();
        text = "SINAL ABERTO";
        printTexto(text, 10);
        text = "- ATRAVESSAR";
        printTexto(text, 30);
        text = "COM CUIDADO";
        printTexto(text, 50);
        sleep_ms(10000); 
        limparTexto();
        }
    }

    return 0;
}

