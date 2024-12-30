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

### Estrutura do protocolo MQTT

A arquitetura utilizada é chamada de Publish-Subscribe.

Haverá um cliente publisher, o qual publicará uma informação em um tópico específico de um dispositivo intermediário chamado MQTT Broker, e haverão clientes subscribers desse tópico que receberam essas informações automaticamente via broker.

## Principais funções do Broker

Mediar o fluxo de mensagens entre os publishers e os subscribers.

Gerenciar as solicitações de conexão e desconexão dos clientes.

Gerenciar as assinaturas e cancelamentos de tópicos dos subscribers.

Garantir a entrega das mensagens aos clientes assinantes de acordo com o QoS especificado.


