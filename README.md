# OrthoLeg - Sistema de Auxílio a locomoção

O projeto **OrthoLeg** tem a finalidade de criar um dispositivo de auxílio à locomoção para pessoas que necessitam de ajuda quanto a isso. Este repositório contém todo o material necessário para replicar o hardware e o software utilizado no exoesqueleto.

### Foto do exoesqueleto 

<img width="450" height="1000" alt="Image" src="https://github.com/user-attachments/assets/bc33c4d5-1661-427d-85cf-b37730661d6b" />

---

## 📁 Organização do Repositório

| Diretório | Descrição |
| :--- | :--- |
| `/firmware` | Códigos fonte Arduino (`.ino`) e scripts Python (`.py`). |
| `/3D Models` | Arquivos de modelagem técnica e design (`.f3d`). |
| `/Board Files` | Arquivos de projeto da PCB desenvolvidos no EasyEDA. |

---
## Hardware: OrthoLeg Board V1

A **OrthoLeg Board V1** é o cérebro eletrônico do sistema, composta por:
* **Processamento:** 2x Arduinos Nano.
* **Potência:** Conversor Buck para regulação de tensão.
* **Interface:** Conectores dedicados para encoders e drivers de motor.
### Modelo 3D da Placa
<img width="600" height="450" alt="Modelo 3D da Placa Ortholeg" src="https://github.com/user-attachments/assets/084ba103-7497-4d97-9403-00b55f111cd1" />

### Placa Fabricada
<img width="364" height="398" alt="Image" src="https://github.com/user-attachments/assets/b54ddeee-4edd-4f48-b23d-03b400ebf26f" />

### Capa da Base
<img width="800" height="500" alt="Capa da Base" src="https://github.com/user-attachments/assets/91c561c7-3ecd-4a40-afc4-6c5a2ca0764a" />

### Capa Fabricada
<img width="450" height="1000" alt="Capa da Base Fabricada" src="https://github.com/user-attachments/assets/cdd70863-a185-4892-a269-7c55e6efe981" />

---

## Tecnologias e Funcionalidades

* **Firmware:** Escrito em C++ (Arduino), focado em controle PID.
* **Sensores:** Utilização de encoders de alta resolução (**37.000 pulsos/volta**) para precisão de movimento.
* **Mecânica:** Sistema de fuso para conversão de movimento circular em linear.
* **Software de Suporte:** Scripts em Python (Raspberry Pi) para envio de comandos e análise de desempenho dos motores em tempo real.

### Arquitetura de Hardware
<img width="615" height="439" alt="Arquitetura de Hardware" src="https://github.com/user-attachments/assets/8cbc2e5b-d47a-4639-b1d6-62dcfd773fec" />

## Script Raspberry Pi
O script "SerialScript.py" consiste na utilização da biblioteca PySerial para realizar a comunicação bidirecional entre os 4 arduinos e o raspberry pi. No código em python, é possível inserir uma sequência de ângulos a serem atingidos pelo exoesqueleto. Sendo assim, essa sequência é passada para os arduinos e assim o movimento é realizado. Através desse script, é possível também ler tudo o que o arduino manda através do raspberry, tornando a leitura de todas as portas serias de forma mais fácil.

## Firmware Arduino
O Firmware presente no "Controle_articulacao_em_Graus_V1.ino" é o mesmo nos quatro arduinos utilizados. Ele consiste em um algoritmo de controle todo feito em C++ para que os motores de alta precisão funcionem. Sendo assim, ele funciona em conjunto com o código presente no raspberry pi.

## Construção da PCB
O PCB foi feito utilizando o software EasyEDA. Sendo assim, foi feito o esquemático de acordo com a necessidade do projeto e em seguida foi feito o posicionamento dos componentes. A fabricação foi feita pelo Protolab do IMD/NPITI.

## Modelo 3D Case
A case do projeto foi constrúida através do programa Fusion 360. Nele foi feito todo o desenho técnico e extrusão do objeto. A fabricação ocorreu no Protolab do IMD/NPITI.




