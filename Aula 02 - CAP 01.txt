// O que é um Sistema Embarcado?

Sistema computacional projetado para uma função específica.

Exemplo: A placa responsável pelo funcionamento da máquina de lavar, não funciona na geladeira, a função dela é específica para a máquina de lavar.

// Modos de funcionamento

Reativo -> O sistema permanece em estado de espera até que ocorra um evento externo.

Um sistema de controle térmico age conforme a temperatura ambiente, realizando funções com base nesses fatores.

Controle em tempo real -> Cada tarefa possui um tempo determinado para ser executado.

// Tipos de aplicações

Propósito geral -> Sistemas que funcionam como computadores normais, sendo multitarefas.

Sistemas de controle -> Sistemas que monitoram e controlam equipamentos para garantir eficiência e segurança, como o sistema de freios ABS, por exemplo.

Processamento de sinais -> Sistemas que vão realizar o tratamento de sinais para uma finalidade especifica, como um aparelho auditivo que amplifica as ondas sonoras para o usuário ouvir com mais nitidez, por exemplo.

Comunicação e Redes -> Sistemas que realizam troca de dados entre dispositivos e gerenciam a rede, promovendo segurança e eficiência.

// Classificação dos sistemas em tempo real

Soft Real Time -> As tarefas são executadas em tempos específicos, sem um prejuízo grave caso alguma tarefa não consiga ser executada corretamente.

Exemplo: Se num sistema multimídia houver um erro ao reproduzir uma música, basta tentar reproduzi-la novamente.

Hard Real Time -> As tarefas são executadas em tempos específicos, no entanto, em caso de falha, o prejuízo pode ser grave.

Exemplo: Em um sistema de aeronaves, caso ocorra algum erro no piloto automático ou na comunicação do sistema, pode ocorrer um acidente.

// Características

Muitos sistemas embarcados não podem sequer parar de funcionar, portanto, precisam de alta disponibilidade(redundância, mecânicas extras que realizam a mesma função em caso e necessidade).

Esses sistemas buscam ser os menores possíveis.

Utilizam microprocessadores e circuitos com capacidade limitada devido o contexto em que são implementados(baixo consumo de energia, de velocidade de processamento e de memoria).

Executa somente um programa repetidamente(Funcionalidade Única).

Muitos sistemas possuem sensores ou atuadores que possibilitam reagir com o ambiente(Sistemas Reativos de Tempo Real).

O firmware do sistema não pode ser usado em outro sistema sem ser modificado(Firmware Proprietário).

O hardware de um sistema embarcado é feito sob medida para a função que ele exercerá(Hardware Sob Medida).

O programa / firmware e o hardware atuam numa relação de interdependência, ou seja, o programa é feito rigorosamente para funcionar apenas naquele hardware(Interdependência entre Hardware e Firmware).

// Restrições

Baixo custo, interface simples, dimensões reduzidas, segurança, disponibilidade, estabilidade, robustez, baixa potencia e tempo de resposta.

// Restrições da Rede

Baixa taxa de bits / taxa de transferência alcançável.

Alta perda de pacotes e alta oscilação na taxa de entrega.

Características de ligação altamente assimétricas.

Prejuízo para o uso de pacotes maiores.

Limites de acessibilidade ao longo do tempo.

