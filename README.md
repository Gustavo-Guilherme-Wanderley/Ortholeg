# OrthoLeg - Sistema de Controle de Articulação

O projeto **OrthoLeg** tem a finalidade de criar um dispositivo de auxílio à locomoção para pessoas que necessitam de ajuda quanto a isso. Este repositório contém todo o material necessário para replicar o hardware e o software utilizado no exoesqueleto.

---

## Hardware: Ortholeg Board V1

A **OrthoLeg Board V1** consiste em uma placa que contém:
* Dois Arduinos Nano.
* Um conversor Buck para redução de tensão.
* Dois conectores para conexão dos encoders.
* Conectores para comunicação com os drivers dos motores.

### Modelo 3D da Placa
<img width="600" height="450" alt="Modelo 3D da Placa Ortholeg" src="https://github.com/user-attachments/assets/084ba103-7497-4d97-9403-00b55f111cd1" />

### Capa da Base
<img width="800" height="500" alt="Capa da Base" src="https://github.com/user-attachments/assets/91c561c7-3ecd-4a40-afc4-6c5a2ca0764a" />

### Arquitetura de Hardware
<img width="615" height="439" alt="Arquitetura de Hardware" src="https://github.com/user-attachments/assets/8cbc2e5b-d47a-4639-b1d6-62dcfd773fec" />

---

## Tecnologias e Funcionalidades

* **Firmware:** Escrito em C++ (Arduino), focado em controle PID.
* **Sensores:** Utilização de encoders de alta resolução (**37.000 pulsos/volta**) para precisão de movimento.
* **Mecânica:** Sistema de fuso para conversão de movimento circular em linear.
* **Software de Suporte:** Scripts em Python (Raspberry Pi) para envio de comandos e análise de desempenho dos motores em tempo real.

## Script Raspberry Pi
O script "SerialScript.py" consiste na utilização da biblioteca PySerial para realizar a comunicação bidirecional entre os 4 arduinos e o raspberry pi. No código em python, é possível inserir uma sequência de ângulos a serem atingidos pelo exoesqueleto. Sendo assim, essa sequência é passada para os arduinos e assim o movimento é realizado. Através desse script, é possível também ler tudo o que o arduino manda através do raspberry, tornando a leitura de todas as portas serias de forma mais fácil.

## Firmware Arduino
O Firmware presente no "Controle_articulacao_em_Graus_V1.ino" é o mesmo nos quatro arduinos utilizados. Ele consiste em um algoritmo de controle todo feito em C++ para que os motores de alta precisão funcionem. Sendo assim, ele funciona em conjunto com o código presente no raspberry pi.




