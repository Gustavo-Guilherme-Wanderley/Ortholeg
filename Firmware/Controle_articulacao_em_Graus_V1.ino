// ======================= CONFIGURAÇÕES =========================
#define ENCODER_A 2
#define ENCODER_B 3
#define PULSOS_POR_VOLTA 37000

#define PASSO 23.3               // mm no fuso por volta completa no eixo do motor
#define CURSO_FUSO_MM 60.58       // curso linear total em mm 2.6voltasX23.3 PRECISA AJUSTAR
#define ANGULO_TOTAL_GRAUS 90.0  // rotação correspondente ao curso total do fuso
#define ERRO_PULSOS 200

#define VALOR_PWM_MAX 150
#define VALOR_PWM_MIN 50
#define DELTA_T 50  // ms - intervalo do loop de controle

#define LIMIT_SOMA_ERRO_POS 100000
#define LIMIT_SOMA_ERRO_VEL 50000

// ======================= PINAGEM ===============================
const int RPWM = 5;
const int LPWM = 6;
const int R_EN = 7;
const int L_EN = 8;

// ======================= VARIÁVEIS GLOBAIS =====================
volatile long pulsosEncoder = 0;

float Kp_pos = 0.02;
float Ki_pos = 0.001;

float Kp_vel = 0.153;
float Ki_vel = 0.15;
float Kd_vel = 0.01;

float soma_erro_pos = 0;
float soma_erro_vel = 0;
float erro_vel_anterior = 0;

long pulsosEncoder_anterior = 0;
float vel_atual = 0;

bool movimentoConcluido = true;

#define MAX_MOVIMENTOS 10
float angulosGraus[MAX_MOVIMENTOS];  // agora o comando é em graus
int totalMovimentos = 0;
int indiceMovimento = 0;

long ALVO_PULSOS = 0;

// ======================= SETUP ================================
void setup() {
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B), readEncoder, RISING);

  Serial.begin(115200);
  Serial.println("Aguardando comando serial com ângulos em graus...");
}

// ======================= LOOP ================================
void loop() {
  receberComandoSerial();

  if (indiceMovimento >= totalMovimentos) {
    return;
  }

  if (!movimentoConcluido) {
    long erro_pos = ALVO_PULSOS - pulsosEncoder;

    if (abs(erro_pos) <= ERRO_PULSOS) {
      SetMotor(0, 0);
      movimentoConcluido = true;
      soma_erro_pos = 0;
      soma_erro_vel = 0;
      erro_vel_anterior = 0;

      Serial.println(">>> Movimento concluído <<<");

      indiceMovimento++;
      if (indiceMovimento < totalMovimentos) {
        iniciarNovoMovimento();
      }
    } else {
      // Controle PI de posição → gera velocidade de referência
      soma_erro_pos += erro_pos * (DELTA_T / 1000.0);
      soma_erro_pos = constrain(soma_erro_pos, -LIMIT_SOMA_ERRO_POS, LIMIT_SOMA_ERRO_POS);
      float vel_ref = (Kp_pos * erro_pos) + (Ki_pos * soma_erro_pos);

      // Medição da velocidade atual
      long delta_pulsos = pulsosEncoder - pulsosEncoder_anterior;
      double delta_phi = delta_pulsos * (2 * PI / PULSOS_POR_VOLTA);
      vel_atual = delta_phi / (DELTA_T / 1000.0);  // rad/s
      pulsosEncoder_anterior = pulsosEncoder;

      // Controle PID de velocidade
      float erro_vel = vel_ref - vel_atual;
      soma_erro_vel += erro_vel * (DELTA_T / 1000.0);
      soma_erro_vel = constrain(soma_erro_vel, -LIMIT_SOMA_ERRO_VEL, LIMIT_SOMA_ERRO_VEL);
      float derivada_erro_vel = (erro_vel - erro_vel_anterior) / (DELTA_T / 1000.0);
      erro_vel_anterior = erro_vel;

      float pwm = (Kp_vel * erro_vel) + (Ki_vel * soma_erro_vel) + (Kd_vel * derivada_erro_vel);

      pwm = constrain(abs(pwm), VALOR_PWM_MIN, VALOR_PWM_MAX);
      int direcao = (erro_pos > 0) ? 1 : -1;
      SetMotor(direcao, pwm);

      // Conversões para exibição
      float pos_mm = (pulsosEncoder * PASSO) / PULSOS_POR_VOLTA;
      float alvo_mm = (ALVO_PULSOS * PASSO) / PULSOS_POR_VOLTA;
      float angulo_atual = (pos_mm * ANGULO_TOTAL_GRAUS) / CURSO_FUSO_MM;
      float angulo_alvo = (alvo_mm * ANGULO_TOTAL_GRAUS) / CURSO_FUSO_MM;

      // Saída para plotagem (exemplo: tempo real)
      Serial.print(angulo_atual);
      Serial.print(",");
      Serial.print(angulo_alvo);
      Serial.print(",");
      Serial.print(pwm);
      Serial.print(",");
      Serial.println(vel_atual);
    }

    delay(DELTA_T);
  }
}

// ======================= INICIAR NOVO MOVIMENTO ====================
void iniciarNovoMovimento() {
  float alvo_graus = angulosGraus[indiceMovimento];

  // Converte o alvo em mm e depois para pulsos
  float alvo_mm = (alvo_graus * CURSO_FUSO_MM) / ANGULO_TOTAL_GRAUS;
  long delta_pulsos = (alvo_mm * PULSOS_POR_VOLTA) / PASSO;

  ALVO_PULSOS = pulsosEncoder + delta_pulsos;  // movimento incremental

  soma_erro_pos = 0;
  soma_erro_vel = 0;
  erro_vel_anterior = 0;
  pulsosEncoder_anterior = pulsosEncoder;
  movimentoConcluido = false;

  Serial.print(">>> Iniciando movimento ");
  Serial.print(indiceMovimento + 1);
  Serial.print(": ");
  Serial.print(alvo_graus);
  Serial.println(" graus");
}

// ======================= FUNÇÕES AUXILIARES ========================
void SetMotor(int dir, int pwm) {
  if (dir == 1) {
    analogWrite(RPWM, pwm);
    analogWrite(LPWM, 0);
  } else if (dir == -1) {
    analogWrite(RPWM, 0);
    analogWrite(LPWM, pwm);
  } else {
    analogWrite(RPWM, 0);
    analogWrite(LPWM, 0);
  }
}

void readEncoder() {
  int A = digitalRead(ENCODER_A);
  int B = digitalRead(ENCODER_B);
  if (A == B) pulsosEncoder++;
  else pulsosEncoder--;
}

void receberComandoSerial() {
  static String inputSerial = "";

  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      inputSerial.trim();

      totalMovimentos = 0;
      indiceMovimento = 0;

      char inputCStr[100];
      inputSerial.toCharArray(inputCStr, sizeof(inputCStr));

      char* token = strtok(inputCStr, " ");
      while (token != NULL && totalMovimentos < MAX_MOVIMENTOS) {
        angulosGraus[totalMovimentos] = atof(token);
        totalMovimentos++;
        token = strtok(NULL, " ");
      }

      Serial.print("Comando recebido com ");
      Serial.print(totalMovimentos);
      Serial.println(" ângulos.");

      if (totalMovimentos > 0) iniciarNovoMovimento();

      inputSerial = "";
    } else {
      inputSerial += c;
    }
  }
}

