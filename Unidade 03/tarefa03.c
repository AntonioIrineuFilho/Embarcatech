#include "pico/stdlib.h"

const uint button = 11; // GP11
const uint ledRed = 4; // GP04
const uint ledYellow = 5; // GP05
const uint ledGreen = 6; // GP06
const uint ledGreenPed = 10; // GP10 


void semaforo(uint red, uint yellow, uint green, uint greenPed, bool r, bool y, bool g, bool gped) {
  gpio_put(red, r);
  gpio_put(yellow, y);
  gpio_put(green, g);
  gpio_put(greenPed, gped);
}

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
  sleep_ms(15000);
}

void setup() {
  // inicialização dos pinos GP com os números das constantes
  gpio_init(button);
  gpio_init(ledRed);
  gpio_init(ledYellow);
  gpio_init(ledGreen);
  gpio_init(ledGreenPed);

  gpio_pull_up(button); // estabiliza a tensão do botão

  // definindo pino do botão como entrada
  gpio_set_dir(button, GPIO_IN);
  // definindo os pinos dos LEDs como saída
  gpio_set_dir(ledRed, GPIO_OUT);
  gpio_set_dir(ledYellow, GPIO_OUT);
  gpio_set_dir(ledGreen, GPIO_OUT);
  gpio_set_dir(ledGreenPed, GPIO_OUT);

  // definindo os LEDs inicialmente com o estado "desligado"
  gpio_put(ledRed, 0);
  gpio_put(ledYellow, 0);
  gpio_put(ledGreen, 0);
  gpio_put(ledGreenPed, 0);
}

int main () {
   setup();

  while (true) {
    semaforoVeiculos(ledRed, ledYellow, ledGreen, ledGreenPed, button);
  }
  return 0;
}
