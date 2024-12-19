# Insights Eletrônica e Programação

## Eletrônica
Resistores -> trocar grandeza para ohms e deixar 220.

LEDs -> pino reto liga com o ground(negativo) e o pino dobrado com o registor e o pino da placa(positivo).

Pinos reservados -> interfaces de comunicação(I2C, SPI, UART).

Protoboard -> suporte para organização do circuito, nas linhas de a até j se um pino da placa for ligado em qualquer conector, toda aquela coluna fica equivalente ao pino conectado, no entanto, se o pino da placa for ligado nas linhas + ou -, toda a linha ficará equivalente ao pino conectado.

## Programação
#include "pico/stdlib.h" -> biblioteca de propósito geral da placa.

const uint -> constante inteira não negativa.

gpio_init(led/button/etc) -> inicia o pino que gpio que possui o número do parâmetro.

gpio_set_dir(número do pino/variável const uint com o número, GPIO_IN/GPIO_OUT) -> define o pino como entrada ou saída.

gpio_pull_up(número do pino/variável const uint com o número) -> define a tensão como alta para evitar instabilidades, quando o botão é pressionado, o pino é conectado diretamente ao terra e a tensão fica baixa.

gpio_put(número do pino/variável const uint com o número, estado) -> define o estado como ligado ou desligado de um LED, por exemplo.

gpio_get(número do pino/variável const uint com o número) -> se o botão é pressionado, retorna 0.

sleep_ms(tempo em milissegundos) -> pequena pausa.
