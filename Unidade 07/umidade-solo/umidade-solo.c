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


#define WIFI_SSID "DarciRaul_2G"
#define WIFI_PASS "40710413"
#define THINGSPEAK_HOST "api.thingspeak.com"
#define THINGSPEAK_PORT 80

#define API_KEY "4XQ4WYEW93QDAEKI"  // Chave de escrita do ThingSpeak

#define I2C_PORT i2c1
#define PINO_SCL 14
#define PINO_SDA 15

struct tcp_pcb *tcp_client_pcb;
ip_addr_t server_ip;
ssd1306_t display;
int umidade;


// função que gera valores em porcentagens afim de simular a leitura de um sensor
int read_umidade_solo () {
    srand(time(NULL));
    return rand() % 101; // range de 0 a 100 % de umidade
}

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

// função de inicialização da conexão com o WIFI
bool setup_connect_WIFI() {
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

void draw(char *text, bool clear) {
    if (clear) {
        ssd1306_clear(&display);
    }
    ssd1306_draw_string(&display, 10, 30, 1, text);
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

// função para desenhar no OLED conforme a simulação de leitura da umidade
display_text(char *text) {
    if (umidade <= 40) {
        text = "SOLO SECO";
        draw(text, true);
    } else if (umidade <= 70) {
        text = "SOLO IDEAL";
        draw(text, true);
    } else {
        text = "SOLO MUITO ÚMIDO";
        draw(text, true);
    }       
}







int main() {
    stdio_init_all();
    setup_OLED();
    bool connection = false;
    while(connection == false) {
        connection = setup_connect_WIFI();
    }
    char *text = "";

    while (true) {
        dns_gethostbyname(THINGSPEAK_HOST, &server_ip, dns_callback, NULL);
        display_text(text);
        sleep_ms(15000);  // Espera 15 segundos antes de enviar novamente
    }

    return 0;
}