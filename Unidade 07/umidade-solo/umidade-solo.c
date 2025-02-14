#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"
#include "lwip/init.h"
#include "ssd1306.h"

#define LED_R 11
#define LED_B 12
#define LED_G 13

#define I2C_PORT i2c1
#define PINO_SCL 14
#define PINO_SDA 15

#define BUZZER_PIN 21

#define WIFI_SSID "DarciRaul_2G"
#define WIFI_PASS "40710413"
#define THINGSPEAK_HOST "api.thingspeak.com"
#define THINGSPEAK_PORT 80

#define API_KEY "4XQ4WYEW93QDAEKI"

struct tcp_pcb *tcp_client_pcb;
ip_addr_t server_ip;
ssd1306_t display;
int umidade;


// função de inicialização dos componentes do display OLED
void setup_OLED() {
    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(PINO_SCL, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SCL);
    gpio_pull_up(PINO_SDA);
    display.external_vcc=false;
    ssd1306_init(&display, 128, 64, 0x3C, I2C_PORT);
}

// função de inicialização dos pinos do led RGB
void setup_RGB() {
    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_init(LED_B);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_put(LED_R, 0);
    gpio_put(LED_G, 0);
    gpio_put(LED_B, 0);
}

// inicialização do PWM no pino do buzzer
void setup_buzzer() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f); // ajusta o divisor de clock
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(BUZZER_PIN, 0); // desliga o PWM inicialmente
}

// função geral de setup dos componentes
void setup() {
    stdio_init_all();
    setup_OLED();
    setup_RGB();
    setup_buzzer();
}

// função de conexão com o WIFI
bool connect_WIFI() {
    if (cyw43_arch_init()) {
        printf("Falha ao iniciar Wi-Fi\n");
        return false;
    }

    cyw43_arch_enable_sta_mode();
    printf("Conectando ao Wi-Fi...\n");

    if (cyw43_arch_wifi_connect_blocking(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_MIXED_PSK)) {
        printf("Falha ao conectar ao Wi-Fi\n");
        return false;
    }

    printf("Wi-Fi conectado!\n");
    return true;
}

// função que gera valores em porcentagens afim de simular a leitura de um sensor
int read_umidade_solo () {
    srand(time(NULL));
    return rand() % 101; // range de 0 a 100 % de umidade
}

// callback quando recebe resposta do ThingSpeak
static err_t http_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        tcp_close(tpcb);
        return ERR_OK;
    }
    printf("Resposta do ThingSpeak: %.*s\n", p->len, (char *)p->payload);
    pbuf_free(p);
    return ERR_OK;
}

// callback quando a conexão TCP é estabelecida
static err_t http_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {
        printf("Erro na conexão TCP\n");
        return err;
    }

    printf("Conectado ao ThingSpeak!\n");

    umidade = read_umidade_solo();
    char request[256];
    snprintf(request, sizeof(request),
        "GET /update?api_key=%s&field1=%.2f HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "\r\n",
        API_KEY, umidade, THINGSPEAK_HOST);

    tcp_write(tpcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);
    tcp_recv(tpcb, http_recv_callback);

    return ERR_OK;
}

// resolver DNS e conectar ao servidor
static void dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    if (ipaddr) {
        printf("Endereço IP do ThingSpeak: %s\n", ipaddr_ntoa(ipaddr));
        tcp_client_pcb = tcp_new();
        tcp_connect(tcp_client_pcb, ipaddr, THINGSPEAK_PORT, http_connected_callback);
    } else {
        printf("Falha na resolução de DNS\n");
    }
}

// define o led RGB com a cor verde
void set_led_green() {
    gpio_put(LED_R, 0);
    gpio_put(LED_G, 1);
    gpio_put(LED_B, 0);
} 

// define o led RGB com a cor amarela
void set_led_yellow() {
    gpio_put(LED_R, 1);
    gpio_put(LED_G, 1);
    gpio_put(LED_B, 0);
} 

// define o led RGB com a cor vermelha
void set_led_red() {
    gpio_put(LED_R, 1);
    gpio_put(LED_G, 0);
    gpio_put(LED_B, 0);
} 

// função geral para desenhar no OLED
void draw(char *text, bool clear) {
    if (clear) { ssd1306_clear(&display); }
    ssd1306_draw_string(&display, 10, 30, 1, text);
}

// notas musicais para o alerta sonoro
uint alert_notes[] = {
    880, 440, 880, 440, 880, 440, 880, 440,  
    880, 440, 880, 440, 880, 440, 880, 440,  
    880, 440, 880, 440
};

// duração das notas em milissegundos(totalizando 10 segundos)
uint note_duration[] = {
    500, 500, 500, 500, 500, 500, 500, 500,  
    500, 500, 500, 500, 500, 500, 500, 500,  
    500, 500, 500, 500
};

// toca uma nota com a frequência e duração especificadas
void play_tone(uint frequency, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t top = clock_freq / frequency - 1;
    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(BUZZER_PIN, top / 2); // 50% de duty cycle
    sleep_ms(duration_ms);
    pwm_set_gpio_level(BUZZER_PIN, 0); // desliga o som após a duração
    sleep_ms(50); // pausa entre notas
}

// Função principal para tocar a música
void alert_sound() {
    for (int i = 0; i < sizeof(alert_notes) / sizeof(alert_notes[0]); i++) {
        if (alert_notes[i] == 0) {
            sleep_ms(note_duration[i]);
        } else {
            play_tone(alert_notes[i], note_duration[i]);
        }
    }
}













// função para desenhar no OLED conforme a simulação de leitura da umidade
void output() {
    if (umidade <= 40) {
        draw("SOLO SECO", true);
        set_led_red();
        alert_sound();
    } else if (umidade <= 70) {
        draw("SOLO IDEAL", true);
        set_led_green();
    } else {
        draw("SOLO MUITO ÚMIDO", true);
        set_led_yellow();
    }       
}







int main() {

    setup();
    bool connection = false;
    while(connection == false) { connection = connect_WIFI(); }
    while (true) {
        dns_gethostbyname(THINGSPEAK_HOST, &server_ip, dns_callback, NULL);
        output();
        sleep_ms(15000);  // Espera 15 segundos antes de enviar novamente
    }

    return 0;

}