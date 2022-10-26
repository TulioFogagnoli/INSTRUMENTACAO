//declaracao dos pinos utilizados para controlar a velocidade de rotacao
//const int PINO_ENA = 5; 
//const int PINO_ENB = 6;

//declaracao dos pinos utilizados para controlar o sentido do motor
const int PINO_IN1 = 11; 
const int PINO_IN2 = 10;
const int PINO_IN3 = 9;
const int PINO_IN4 = 8;

int i = 0; //declaracao da variavel para as rampas

const int TEMPO_ESPERA = 500; //declaracao do intervalo de 1 segundo entre os sentidos de rotacao do motor

const int TEMPO_RAMPA = 20; //declaracao do intervalo de 30 ms para as rampas de aceleracao e desaceleracao

void setup() {

  //configuração dos pinos como saida
  //pinMode(PINO_ENA, OUTPUT); 
  //pinMode(PINO_ENB, OUTPUT);
  pinMode(PINO_IN1, OUTPUT);
  pinMode(PINO_IN2, OUTPUT);
  pinMode(PINO_IN3, OUTPUT);
  pinMode(PINO_IN4, OUTPUT);

  //inicia o codigo com os motores parados
  digitalWrite(PINO_IN1, LOW); 
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, LOW);
  //digitalWrite(PINO_ENA, LOW);
  //digitalWrite(PINO_ENB, LOW);

}

void loop() {

  //configura os motores para o sentido horario
  digitalWrite(PINO_IN1, LOW); 
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);

  //rampa de aceleracao
  /*for (i = 0; i < 150; i=i+10){ 
    analogWrite(PINO_ENA, i);
    analogWrite(PINO_ENB, i);
    delay(TEMPO_RAMPA); //intervalo para incrementar a variavel i
  }*/

  //rampa de desaceleracao
  // for (i = 150; i >= 0; i=i-10){ 
  //   analogWrite(PINO_ENA, i);
  //   analogWrite(PINO_ENB, i);
  //   delay(TEMPO_RAMPA); //intervalo para incrementar a variavel i
  // }

  delay(TEMPO_ESPERA); //intervalo de um segundo

  //configura os motores para o sentido anti-horario
  digitalWrite(PINO_IN1, HIGH); 
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, HIGH);
  digitalWrite(PINO_IN4, LOW);

  //rampa de aceleracao
  // for (i = 0; i < 150; i=i+10){ 
  //   analogWrite(PINO_ENA, i);
  //   analogWrite(PINO_ENB, i);
  //   delay(TEMPO_RAMPA); //intervalo para incrementar a variavel i
  // }

  //rampa de desaceleracao
  // for (i = 150; i >= 0; i=i-10){ 
  //   analogWrite(PINO_ENA, i); 
  //   analogWrite(PINO_ENB, i);
  //   delay(TEMPO_RAMPA); //intervalo para incrementar a variavel i
  // }

  // delay(TEMPO_ESPERA); //intervalo de um segundo
  
}
