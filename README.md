# Porta Automatizada

## Descrição

Este projeto consiste em um protótipo de porta automatizada, capaz de abrir e fechar sozinha com base na detecção de aproximação de pessoas ou objetos. O sistema utiliza um ESP32 como controlador principal, integrando sensores, atuadores e uma interface visual para demonstração de conceitos de controle e automação.

## Funcionamento

O sensor ultrassônico monitora continuamente a distância do ambiente à sua frente. Quando uma aproximação é detectada dentro de um limite de distância pré-programado, o ESP32 processa a informação dada ao apertar o botao de abertura (simuladando um sensor de presença) e aciona o servomotor, que realiza o movimento de abertura da porta. Simultaneamente, o display OLED atualiza as informações em tempo real, exibindo o status atual do sistema (como a distância medida ou se a porta está "Aberta" / "Fechada").

## Componentes utilizados

* ESP32
* Servomotor SG90
* Sensor Ultrassônico
* Display OLED
* Protoboard e Jumpers de conexão

## 👥 Participantes

<table align="center">
  <tr>
    <td align="center">
      <a href="https://github.com/enzoalvesar"
        <sub>
          <b>Enzo Alves Araújo</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/MarkusVinnicius"
        <sub>
        <b>Markus Vinnicius</b>
        </sub>
          <td align="center">
      <a href="https://github.com/Minerororu"
        <sub>
          <b>Pedro Lucas</b>
        </sub>
      </a>
    </td>
  </tr>
</table>
