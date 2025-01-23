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

int A_state = 0;

ssd1306_t disp;

void inicializa(){

    stdio_init_all();
    i2c_init(I2C_PORT, 400*1000);// I2C Inicialização. Usando 400Khz.
    gpio_set_function(PINO_SCL, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SCL);
    gpio_pull_up(PINO_SDA);
    disp.external_vcc=false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);
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

void print_texto(char *msg, int linha, bool q){
    if(q){
    ssd1306_clear(&disp);//Limpa a tela    
    } else {
    ssd1306_draw_string(&disp, 8, linha, 1, msg);//desenha o texto
    ssd1306_show(&disp);//apresenta no Oled
    }
    
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

    inicializa();
    char *text = "";

    while(true) {
        
       SinalFechado();
       text = "SINAL FECHADO";
       print_texto(text,20, true);
       text = "- AGUARDE";
       print_texto(text,40, false);
       A_state = WaitWithRead(8000); //espera com leitura do botäo
       //sleep_ms(8000);
       if(A_state) { 

        SinalAtencao();
        text = "SINAL DE ATENÇÃO";
        print_texto(text, 20, true);
        text = "- PREPARE-SE";
        print_texto(text, 40, true);
        sleep_ms(5000);

        SinalAberto();
        text = "SINAL ABERTO";
        print_texto(text, 20, true);
        text = "- ATRAVESSAR COM CUIDADO";
        print_texto(text, 40, true);
        sleep_ms(10000); 

        }
    }

    return 0;
}
