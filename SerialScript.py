import serial
import time
import matplotlib.pyplot as plt

# --- CONFIGURAÇÃO DAS PORTAS SERIAIS ---

try:
    # Porta para o motor do Joelho
    ser0 = serial.Serial(
        port='/dev/serial/by-path/platform-fd500000.pcie-pci-0000:01:00.0-usb-0:1.4:1.0-port0',
        baudrate=115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
    )

    # Porta para o motor do Quadril
    ser1 = serial.Serial(
        port='/dev/serial/by-path/platform-fd500000.pcie-pci-0000:01:00.0-usb-0:1.3:1.0-port0',
        baudrate=115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        # Erro corrigido: Removido o fragmento "IGHTBITS," que causava erro de sintaxe
        timeout=1
    )

except serial.SerialException as e:
    print(f"Erro ao abrir a porta serial: {e}")
    print("Verifique se os Arduinos estão conectados e se os caminhos das portas estão corretos.")
    exit()

# --- COMANDOS DE MOVIMENTO ---
string_esperada = "Aguardando comando serial com ângulos em graus..."
anguloJoelho = "10"
anguloQuadril = "0"

num_movimentos_joelho = len(anguloJoelho.split())
num_movimentos_quadril = len(anguloQuadril.split())

# --- LISTAS PARA ARMAZENAR DADOS PARA PLOTAGEM ---
dados_joelho = {'tempo': [], 'vel_ref': [], 'vel_atual': []}
dados_quadril = {'tempo': [], 'vel_ref': [], 'vel_atual': []}

movimentos_concluidos_joelho = 0
movimentos_concluidos_quadril = 0

print("Iniciando comunicação e coleta de dados...")

try:
    start_time = time.time()

    while True:
        if (movimentos_concluidos_joelho >= num_movimentos_joelho and
            movimentos_concluidos_quadril >= num_movimentos_quadril):
            print("\nTodos os movimentos foram concluídos.")
            break

        # --- LEITURA E PROCESSAMENTO DO ARDUINO 0 (JOELHO) ---
        if ser0.in_waiting > 0:
            recebido = ser0.readline().decode('utf-8', errors='ignore').strip()
            print(f"Recebido (Joelho): {recebido}")
           
            if recebido == string_esperada:
                ser0.write((anguloJoelho + "\n").encode(encoding="utf-8"))
                print(f"Enviado (Joelho): {anguloJoelho}")

            if "Vel_ref" in recebido and "Vel_atual" in recebido:
                try:
                    partes = recebido.split('|')
                    # Ajuste de índices baseado no formato comum de strings separadas por '|'
                    vel_ref_str = partes[2].split(':')[1].strip()
                    vel_atual_str = partes[3].split(':')[1].strip()
                   
                    tempo_atual = time.time() - start_time
                    dados_joelho['tempo'].append(tempo_atual)
                    dados_joelho['vel_ref'].append(float(vel_ref_str))
                    dados_joelho['vel_atual'].append(float(vel_atual_str))
                except (IndexError, ValueError) as e:
                    print(f"Aviso: Erro no parse (Joelho): {e}")

            if "Movimento concluído" in recebido:
                movimentos_concluidos_joelho += 1
                print(f"--- Movimento {movimentos_concluidos_joelho}/{num_movimentos_joelho} do Joelho concluído. ---")

        # --- LEITURA E PROCESSAMENTO DO ARDUINO 1 (QUADRIL) ---
        if ser1.in_waiting > 0:
            recebido = ser1.readline().decode('utf-8', errors='ignore').strip()
            print(f"Recebido (Quadril): {recebido}")
           
            if recebido == string_esperada:
                ser1.write((anguloQuadril + "\n").encode(encoding="utf-8"))
                print(f"Enviado (Quadril): {anguloQuadril}")

            if "Vel_ref" in recebido and "Vel_atual" in recebido:
                try:
                    partes = recebido.split('|')
                    vel_ref_str = partes[2].split(':')[1].strip()
                    vel_atual_str = partes[3].split(':')[1].strip()

                    tempo_atual = time.time() - start_time
                    dados_quadril['tempo'].append(tempo_atual)
                    dados_quadril['vel_ref'].append(float(vel_ref_str))
                    dados_quadril['vel_atual'].append(float(vel_atual_str))
                except (IndexError, ValueError) as e:
                    print(f"Aviso: Erro no parse (Quadril): {e}")

            if "Movimento concluído" in recebido:
                movimentos_concluidos_quadril += 1
                print(f"--- Movimento {movimentos_concluidos_quadril}/{num_movimentos_quadril} do Quadril concluído. ---")
       
        time.sleep(0.005)

except KeyboardInterrupt:
    print("\nExecução interrompida pelo usuário.")

finally:
    if 'ser0' in locals() and ser0.is_open:
        ser0.close()
    if 'ser1' in locals() and ser1.is_open:
        ser1.close()
    print("Portas seriais fechadas.")

# --- PLOTAGEM DOS GRÁFICOS ---
print("Gerando gráficos...")
fig, axs = plt.subplots(2, 1, figsize=(12, 10))
fig.suptitle('Desempenho dos Motores', fontsize=16)

# Gráfico Joelho
if dados_joelho['tempo']:
    axs[0].plot(dados_joelho['tempo'], dados_joelho['vel_ref'], label='Vel_ref', color='blue', linestyle='--')
    axs[0].plot(dados_joelho['tempo'], dados_joelho['vel_atual'], label='Vel_atual', color='red', alpha=0.7)
    axs[0].set_title('Motor do Joelho')
    axs[0].set_ylabel('Velocidade (RPS)')
    axs[0].legend()
    axs[0].grid(True)
else:
    axs[0].set_title('Motor do Joelho (Sem dados)')

# Gráfico Quadril (Descomentado para funcionar)
if dados_quadril['tempo']:
    axs[1].plot(dados_quadril['tempo'], dados_quadril['vel_ref'], label='Vel_ref', color='green', linestyle='--')
    axs[1].plot(dados_quadril['tempo'], dados_quadril['vel_atual'], label='Vel_atual', color='orange', alpha=0.7)
    axs[1].set_title('Motor do Quadril')
    axs[1].set_xlabel('Tempo (s)')
    axs[1].set_ylabel('Velocidade (RPS)')
    axs[1].legend()
    axs[1].grid(True)
else:
    axs[1].set_title('Motor do Quadril (Sem dados)')

plt.tight_layout(rect=[0, 0.03, 1, 0.96])
plt.show()
print("Programa finalizado.")
