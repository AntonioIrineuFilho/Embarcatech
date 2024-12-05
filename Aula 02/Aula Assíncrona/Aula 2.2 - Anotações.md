# Arquitetura do Sistema Embarcado

## Software
- Sistema operacional embarcado, firmware e drivers.
- O sistema operacional e os drivers podem ser desenvolvidos por empresas terceiras especializadas.
- O firmware normalmente é desenvolvido pelo desenvolvedor do sistema embarcado e tem como principal
função a inicialização do sistema e a comunicação com os componentes(tal qual uma BIOS).

## Hardware
- CPU(Unidade de Processamento).
- Mémorias de programa(armazena o firmware(ROM / flash)) e de dados(armazena os dados temporários durante
a execução do software(RAM)).
- Interfaces(realizam a comunicação do sistema com os elementos externos, tais quais sensores e atuadores).
- Circuitos auxiliares / Periféricos(permitem que o sistema embarcado perceba e atue no ambiente ao seu redor).

# Arquitetura do Hardware

- O principal componente hardware de um sistema embarcado é o microcontrolador(MCU).

## Microcontrolador
- Chip composto pela CPU / MPU, memórias e interfaces.
- A CPU vai utilzar barramentos específicos para se comunicar com os componentes.

### Barramentos
- Dados: Usado pela MPU para ler e escrever dados com a memória e a interface.
- Endereços: Usado para endereçar memórias e interfaces.
- Controle / CTRL: Controla as memórias e as interfaces.

## Periféricos
- Sensores e atuadores.
- São externos ao MCU, mas desempenham papeis cruciais promovendo a sensibilidade do sistema ao ambiente ao seu redor.

## Arquitetura Havard
- Na arquitetura Havard, as memórias de programa e de dados vão possuir seus próprios barramentos de dados, controle e endereço,
uma vez que vão ser fisicamente separadas no chip.
- Além disso, vai haver um barramento especifico de instrução e outro de dado.
- Essas memórias vão ser acessadas ao mesmo tempo(no mesmo ciclo de clock), funcionando com rapidez.
- A MPU vai ler uma instrução ao mesmo tempo que acessa os dados.
- Arquitetura comum dos microcontroladores RISC.

## Arquitetura Von Neumann
- Diferente da arquitetura de Havard, a de Von Neumann une fisicamente as duas memórias, o que impede que o MCU acesse elas simultaneamente.
- Microcontroladores com essa arquitetura são mais lentos.
- Comum em microcontroladores CISC.

# Arquitetura de Camadas
- O modelo de camadas é composto por: Hardware, Software Básico e Aplicação.

## Hardware
- Inclui todos os componentes físicos do microcontrolador + os sensores / atuadores.
- Sustenta todo o funcionamento do sistema.
### Tipos de hardware
- Hardwired HW: Principal tipo de hardware, baseado em microcontroladores e microprocessadores, amplamente utilizados e eficientes para funções especificas.
- Softwired HW: Baseado em dispositivos de emulação e prototipação como FPGA, permitem maior flexibilidade pois podem ser reprogramados / customizados para
diferentes tarefas, no entanto, são utilizados com menor frequência.
### Tipos de memórias
- Memória para armazenamento de instrução: flash, não-volatil e mantém o dado de forma permanente.
- Memórias para armazenamento de dados: SRAM(mais rápida e eficiente no armazenamento de dados temporários, mas cara e espaçosa) e
DRAM(armazena maiores quantidades de dados, no entanto, é mais lenta e necessita ser atualizada com mais frequência).
### Periféricos
- Sensores: Capturam dados do ambiente e convertem em sinais elétricos que podem ser processados pelo sistema, cada sensor é feito para medir grandezas especificas,
como luz, som, velocidade, temperatura, etc.
- Atuadores: Executam ações no ambiente com base nas instruções recebidas pelo sistema, podem controlar dispositivos, gerar movimento, acionar luzes, etc.
### Interfaces
- GPIO: Pinos de próposito geral que permitem acesar dispositivos de entrada e saida, como teclados, displays, LEDs, etc.
- Temporizadores e Contadores: Medem o tempo e contam eventos, essenciais para controlar precisão de tempo, geração de sinais de relógios.
- Seriais: Permitem a comunicação com outros sistemas e sensores, como UART, SPI e I2C, facilitando troca de dados entre o MCU e outros dispositivos como sensores, outros MCUs, etc.
- Conversores AD e DA: Analógico-Digital permitem que o sinal analógico do ambiente, como a tensão de um sensor, seja convertido em dados digitais que um microcontrolador possa processar,
enquanto o Digital-Analógico realiza o inverso, converte os sinais digitais em analógicos para controlar dispositivos que recebem sinais analógico, como motores e auto-falantes.
- Modulação de largura de pulso(PWM): Controla a potência fornecida a dispostivos como motores e LEDs.

## Software Básico
- Inclui o firmware, os drivers e (em alguns casos) o sistema operacional.
- Responsável por gerenciar os recursos do hardware e fornecer uma interface para que o software de aplicação possa se comunicar de
forma eficiente e segura com o hardware.
### Device drivers
- Modulos de software que encapsulam componentes de hardware e realizam intermediação entre comandos de alto nível da camada de aplicação e o hardware.
### Núcleo operacional
- Kernel: Gerencia os recursos do sistema, como CPU, memória e dispositivos de entrada/saída.
- RTOS(Sistema Operacional em Tempo Real):  

## Aplicação
- Realiza as funções que o sistema embarcado foi programado para realizar.
