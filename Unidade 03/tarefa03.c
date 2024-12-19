#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

const uint button = 11; // GP11
const uint ledRed = 4; // GP04
const uint ledYellow = 5; // GP05
const uint ledGreen = 6; // GP06
const uint ledGreenPed = 10; // GP10 
const uint buzzer = 12; // GP12
const uint buzzerFreq = 2000; // frequência do buzzer

void  inicializarPWM(uint buzzer, uint buzzerFreq) {
  gpio_set_function(buzzer, GPIO_FUNC_PWM); // definir o GP12 como saída do PWM
  uint valorSlice = pwm_gpio_to_slice_num(buzzer); // pega o número de uma unidade de controle do PWM
  pwm_config padrao = pwm_get_default_config(); // salva a configuração padrão do PWM
  pwm_config_set_clkdiv(&padrao, clock_get_hz(clk_sys) / (buzzerFreq * 4096)); // realiza a divisão do clock, guardando no endereço de memoria padrão a divisão do clock do sistema pelo produto da frquência do buzzer com a resolução do PWM
  pwm_init(valorSlice, &padrao, true); // inicializa o PWM
  pwm_set_gpio_level(buzzer, 0); // inicia o PWM sem som 
}

void som(uint buzzer) {
  uint valorSlice = pwm_gpio_to_slice_num(buzzer);
  pwm_set_gpio_level(buzzer, 2048); // coloca o PWM na metade do nível
  sleep_ms(15000); // define o tempo de duração do som
  pwm_set_gpio_level(buzzer, 0); // encerra o som
}

void semaforo(uint red, uint yellow, uint green, uint greenPed, bool r, bool y, bool g, bool gped) {
  gpio_put(red, r);
  gpio_put(yellow, y);
  gpio_put(green, g);
  gpio_put(greenPed, gped);
}

void semaforoPedestres(uint red, uint yellow, uint green, uint greenPed);

void semaforoVeiculos(uint red, uint yellow, uint green, uint greenPed, uint button) {
  semaforo(red, yellow, green, greenPed, 0, 0, 1, 0);
  for (int i = 0; i < 200; i++) {
    if (gpio_get(button) == 0) {
      semaforoPedestres(red, yellow, green, greenPed);
      return;
    }
    sleep_ms(40);
  }
  semaforo(red, yellow, green, greenPed, 0, 1, 0, 0);
  for (int i = 0; i < 200; i++) {
    if (gpio_get(button) == 0) {
      semaforoPedestres(red, yellow, green, greenPed);
      return;
    }
    sleep_ms(10);
  }
  semaforo(red, yellow, green, greenPed, 1, 0, 0, 0);
  for (int i = 0; i < 200; i++) {
    if (gpio_get(button) == 0) {
      semaforoPedestres(red, yellow, green, greenPed);
      return;
    }
    sleep_ms(50);
  }
}

void semaforoPedestres(uint red, uint yellow, uint green, uint greenPed) {
  semaforo(red, yellow, green, greenPed, 0, 1, 0, 0);
  sleep_ms(5000);
  semaforo(red, yellow, green, greenPed, 1, 0, 0, 1);
  som(buzzer);
}

void setup() {
  // inicialização dos pinos GP com os números das constantes
  gpio_init(button);
  gpio_init(ledRed);
  gpio_init(ledYellow);
  gpio_init(ledGreen);
  gpio_init(ledGreenPed);
  gpio_init(buzzer);

  gpio_pull_up(button); // estabiliza a tensão do botão

  // definindo pino do botão como entrada
  gpio_set_dir(button, GPIO_IN);
  // definindo os pinos dos LEDs como saída
  gpio_set_dir(ledRed, GPIO_OUT);
  gpio_set_dir(ledYellow, GPIO_OUT);
  gpio_set_dir(ledGreen, GPIO_OUT);
  gpio_set_dir(ledGreenPed, GPIO_OUT);
  gpio_set_dir(buzzer, GPIO_OUT);

  // definindo os LEDs inicialmente com o estado "desligado"
  gpio_put(ledRed, 0);
  gpio_put(ledYellow, 0);
  gpio_put(ledGreen, 0);
  gpio_put(ledGreenPed, 0);
}

int main () {
   setup();
   inicializarPWM(buzzer, buzzerFreq);

  while (true) {
    semaforoVeiculos(ledRed, ledYellow, ledGreen, ledGreenPed, button);
  }
  return 0;
}
