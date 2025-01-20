# Manual Embarcação de Códigos - Raspberry Pi Pico W

## Portas GPIO - BitDogLab

### Led RGB

Vermelho -> GPIO 13

Azul -> GPIO 12

Verde -> GPIO 11

### Comunicação SPI

Tx -> GPIO 19

Rx -> GPIO 16

CSn -> GPIO 17

SCK -> GPIO 18

### Display OLED

SDA -> GPIO 14

SCL -> GPIO 15

### Botões

A -> GPIO 5

B -> GPIO 6

### Buzzer

GPIO 10 e GPIO 21

### Matriz de Leds  Coloridos

GPIO 7 (5 linhas por 5 colunas)

### Microfone

GP 28

### Joystick

Vry -> GPIO 28

Vrx -> GPIO 27

Sw -> GPIO 22

## Configurar Visual Studio Code

### Extensões

C/C++ Compile and Run

C/C++ Extension Pack

Raspberry Pi Pico

### Driver do MCU

Instalar Driver Zadig(já instalado)

## Criação do Projeto

1º -> Criar novo projeto C/C++

2º -> Nome do projeto sem caracteres especiais ou espaços, além de começar sempre com letras

3º -> Em "Stdio support" sempre marcar "Console over USB"

4º -> Compilar projeto e rodar com "Run Project(USB)"

5º -> Antes de rodar, a placa deve estar em modo bootsel, para isso, basta conectar a placa via USB e rodar o comando "picotool reboot -f -u" no terminal(dar enter duas vezes caso o terminal já esteja rodando outro processo)
