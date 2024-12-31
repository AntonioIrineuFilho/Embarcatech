# Protocolos IoT

## Arquitetura De Redes Convencionais

Envio -> Encapsulamento(Host A)

Recebimento -> Desencapsulamento(Host B)

### Encapsulamento

Dado(camada de aplicação) -> Segmento(transporte) -> Pacote(rede) -> Frame(enlace) -> Bit (física)

### Desencapsulamento

Bit(camada física) -> Frame(enlace) -> Pacote(rede) -> Segmento(transporte) -> Dado(aplicação)

## MQTT

Protocolo de mensagens usando mecanismos de publicação/assinatura, sob o padrão OASIS. Amplamente utilizado atualmente por ser leve e utilizar pouca largura de banda, além de ser aberto e simples de ser implementado.

O MQTT vai agir sob o TCP, analisando o congestionamento da rede e priorizando a integridade da informação. 

### Estrutura do protocolo MQTT

A arquitetura utilizada é chamada de Publish-Subscribe.

Haverá um cliente publisher, o qual publicará uma informação em um tópico específico de um dispositivo intermediário chamado MQTT Broker, e haverão clientes subscribers desse tópico que receberam essas informações automaticamente via broker.

## Principais funções do Broker

Mediar o fluxo de mensagens entre os publishers e os subscribers.

Gerenciar as solicitações de conexão e desconexão dos clientes.

Gerenciar as assinaturas e cancelamentos de tópicos dos subscribers.

Garantir a entrega das mensagens aos clientes assinantes de acordo com o QoS especificado.

## Níveis de QoS

QoS 0 -> O publisher envia os dados de tempos em tempos sem ter feedback sobre o recebimento.

QoS 1 -> O publisher envia e espera um PUBACK, ou seja, um feedback de recebimento. Caso não receba, ele enviará a informação até receber, o que pode causar duplicação da mensagem.

QoS 2 -> O publisher enviará exatamente uma vez, sem duplicação, no entanto, será mais complexo e lento. Para isso, ocorrem quatro etapas de confimção: o envio, o PUBREC do broker para o publisher, o PUBREL do publisher para o broker e o PUBCOMP do broker para o publisher.

## CoAP

Protocolo de Aplicação Restrita, bom para ambientes com recursos limitados: enlaces com baixa largura de banda, redes congestionadas ou com perdas.

O CoAP vai agir sob o UDP, uma vez que é mais adequado para redes com bandas restritas, no entanto, não garante a integridade.

O CoAP segue um modelo parecido com o HTTP, seguindo um modelo cliente-servidor. Um cliente pode fazer um request ao servidor, que responde com o resultado ou uma mensagem de erro. O request pode conter métodos com informações, como GET, POST, PUT e DELETE, solicitando ações dentro do servidor.

Camadas: Aplicação -> CoAP(Request / Response e Message) -> UDP.

O CoAP é como um HTTP mais leve e adaptado para IoT.


