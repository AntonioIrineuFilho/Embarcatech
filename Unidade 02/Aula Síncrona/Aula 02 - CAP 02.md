# Arquitetura do SE

## Software Embarcado 
- Composto pelo sistema operacional embarcado, firmware, debug e drivers.

## Hardware Embarcado 
- CPU, memórias, interfaces e circuitos auxiliares.

# Hardware

- Microcontrolador(MCU)
- Memorias externas
- Interfaces
- Sensores
- Atuadores

## MCU
- Possui sessões internas com o microprocessador(MPU), as memorias e as interfaces.

## Arquitetura Harvard
- Em um único ciclo de clock, o MPU acessa a memoria de dados e a memoria de programa(endereços).

## Arquitetura Von Neumann
- Enquanto em Havard as memorias possuem endereços e dados diferentes, mas que são acessados simultaneamente, em Von Neumann as memorias possuem o endereço e os dados em comum, no mesmo espaço, podendo exigir mais de um ciclo de clock para o acesso ser feito em cada memoria.

# Arquitetura de Camadas

- A complexidade do sistema é feita por meio de camadas.

- A primeira camada será uma camada física de hardware, seguida de uma camada de drivers de dispositivo, um núcleo operacional(microkernel), a base do software(protocolos e serviços) e a aplicação de fato.

# Componentes de um Sistema Embarcado

## Field-Programmable Gate Array(FPGA)
- Dispositivos programáveis e reconfiguráveis online(similar a RAM).

### Tipos de programação:

- PROM(programáveis uma única vez)
- EEPROM(reprogramáveis em laboratório)
- RAM(programáveis durante a execução)

## Microcontrolador 
- Incorpora funções em um único chip.

## Atuadores 
- Drivers e etc.

## Componentes digitais
- Microprocessadores, coprocessadores, memorias, barramentos, microcontroladores.

## Conversores
- Conversor Analogico-Digital, Digital-Analogico.

## Software
- SO, Device Driver, Aplicações(GSM-Kernel, MPEG-x).

## Periféricos
- Componenetes que podem ser de entrada ou saída, internos ou externos e acessam os registros do microcontrolador.

## Interfaces
- Realizama ligação entre as MCUs e os periféricos, como interface para teclado, interface LCD, etc.
