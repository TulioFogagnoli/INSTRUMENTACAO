#include <neotimer.h>
#include <MPU6050_tockn.h>
#include <Ultrasonic.h>
#include <SoftwareSerial.h>
#include <HCSR04.h>
#include <Wire.h> //Biblioteca utilizada gerenciar a comunicação entre dispositicos através do protocolo I2C
#include <LiquidCrystal_I2C.h> //Biblioteca controlar display 16x2 através do I2C
#include <SPI.h> //INCLUSÃO DE BIBLIOTECA
#include <MFRC522.h> //INCLUSÃO DE BIBLIOTECA


Neotimer sensor1;
Neotimer trig1;
Neotimer trig2;

/* --------- RFID --------------*/
#define SS_PIN 10 //PINO SDA
#define RST_PIN 9 //PINO DE RESET
MFRC522 rfid(SS_PIN, RST_PIN); //PASSAGEM DE PARÂMETROS REFERENTE AOS PINOS

/* ---------- GYRO ----------- */
#define MPU6050_ADDR         0x68 // ENDEREÇO QUANDO O PINO AD0 ESTIVER LIGADO AO GND
int AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
#define DEBUG
MPU6050 mpu6050(Wire);
float anguloX;
float anguloY;
float anguloZ;
float temp;
unsigned long controleTempo;

/* ---------- DISPLAY E ULTRASSOM ------------- */
#define col  16 //Define o número de colunas do display utilizado
#define lin   2 //Define o número de linhas do display utilizado
#define ende  0x27 //Define o endereço do display
LiquidCrystal_I2C lcd(ende, col, lin); 
#define TRIGGER   A3
#define ECHO      A2
Ultrasonic ultrasonic(TRIGGER,ECHO);
int distancia; 
String result; 

/* ---------- PONTE H -------- */
#define in1 4                   
#define in2 5
#define in3 6
#define in4 7
int Speed = 200;
int Speedsec;
int buttonState = 0;
int lastButtonState = 0;
int Turnradius = 0;
int brakeTime = 45;
int brkonoff = 1;

/* ---------- BLUETOOTH -------- */
const int pinoRX = 2;  //PINO DIGITAL 2 (RX)
const int pinoTX = 3;  //PINO DIGITAL 3 (TX)
int dadoBluetooth = 0;
SoftwareSerial bluetooth(pinoRX, pinoTX);

/* ----------- IR ------------ */
const int pinIR = 8;
int IR;

void setup() {
    Serial.begin(9600);

  sensor1.set(200);
  trig1.set(2);
  trig2.set(10);
  lcd.init(); //Inicializa a comunicação com o display já conectado
  lcd.setBacklight(HIGH);
  lcd.clear(); //Limpa a tela do display
  lcd.setCursor(0, 0);
  lcd.print("iniciando...");
  lcd.clear();
  lcd.display();

  SPI.begin(); //INICIALIZA O BARRAMENTO SPI
  rfid.PCD_Init(); //INICIALIZA MFRC522

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(false);// MUDAR PARA "true" SE QUISER VISUALIZAR INFORMAÇÕES DE CALIBRAÇÃO NO MONITOR SERIAL

  #ifdef DEBUG
    Serial.println("Fim Setup");
  #endif  

  pinMode(pinIR, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ECHO, INPUT); 
  pinMode(TRIGGER, OUTPUT);

  bluetooth.begin(9600);  //INICIALIZA A SERIAL DO BLUETOOTH
  bluetooth.print("$");   //IMPRIME O CARACTERE
  bluetooth.print("$");   //IMPRIME O CARACTERE
  bluetooth.print("$");   //IMPRIME O CARACTERE

  delay(500);
}

void loop() {
  if(sensor1.repeat()){
  lcd.clear();
  hcsr04();
  mpu6050.update();
  frfid();

  anguloX = mpu6050.getAngleX();
  anguloY = mpu6050.getAngleY();
  anguloZ = mpu6050.getAngleZ();
  temp = mpu6050.getTemp();
  Serial.print(anguloX);
  Serial.print("   |   ");
  Serial.print(anguloY);
  Serial.print("   |   ");
  Serial.print(anguloZ);
  Serial.print("   |   ");
  Serial.println(temp);

  if(digitalRead(pinIR) == LOW){ IR = 0;}
  if(digitalRead(pinIR) == HIGH){ IR = 1;}
  lcd.setCursor(0,0);
  lcd.print(result);    //Exibe no display as medidas
  lcd.print("cm");
  lcd.setCursor (5,0);
  lcd.print(IR);
  lcd.setCursor(7,0);
  lcd.print(temp);
  lcd.setCursor(0,1);
  lcd.print(anguloX);
  //lcd.setCursor(6,1);
  //lcd.print(anguloY);
  lcd.setCursor(7,1);
  lcd.print(anguloZ);
  }
  
  /* ----------------- CONTROLE CARRO ----------------- */
  if (bluetooth.available()) {         //SE O BLUETOOTH ESTIVER HABILITADO, FAZ
    dadoBluetooth = bluetooth.read();  //VARIÁVEL RECEBE O VALOR ENVIADO PELO BLUETOOTH
    Stop();
    if (dadoBluetooth == 'F') {
      Serial.println("forward");
      forward();
    }
    if (dadoBluetooth == 'B') {
      Serial.println("back");
      back();
    }
    if (dadoBluetooth == 'L') {
      Serial.println("left");
      left();
    }
    if (dadoBluetooth == 'R') {
      Serial.println("right");
      right();
    }
    if (dadoBluetooth == 'G') {
      Serial.println("forwardleft");
      forwardleft();
    }
    if (dadoBluetooth == 'I') {
      Serial.println("forwardright");
      forwardright();
    }
    if (dadoBluetooth == 'H') {
      Serial.println("backleft");
      backleft();
    }
    if (dadoBluetooth == 'J') {
      Serial.println("backright");
      backright();
    }
    if (dadoBluetooth == '0') { Speed = 100; }
    if (dadoBluetooth == '1') { Speed = 120; }
    if (dadoBluetooth == '2') { Speed = 140; }
    if (dadoBluetooth == '3') { Speed = 160; }
    if (dadoBluetooth == '4') { Speed = 170; }
    if (dadoBluetooth == '5') { Speed = 180; }
    if (dadoBluetooth == '6') { Speed = 190; }
    if (dadoBluetooth == '7') { Speed = 200; }
    if (dadoBluetooth == '8') { Speed = 215; }
    if (dadoBluetooth == '9') { Speed = 235; }
    if (dadoBluetooth == 'q') { Speed = 255; }

  }

}

void forward() {
  analogWrite(in1, Speed);
  analogWrite(in3, Speed);
}

void back() {
  analogWrite(in2, Speed);
  analogWrite(in4, Speed);
}

void left() {
  analogWrite(in3, Speed);
  analogWrite(in2, Speed);
}

void right() {
  analogWrite(in4, Speed);
  analogWrite(in1, Speed);
}
void forwardleft() {
  analogWrite(in1, Speedsec);
  analogWrite(in3, Speed);
}
void forwardright() {
  analogWrite(in1, Speed);
  analogWrite(in3, Speedsec);
}
void backright() {
  analogWrite(in2, Speed);
  analogWrite(in4, Speedsec);
}
void backleft() {
  analogWrite(in2, Speedsec);
  analogWrite(in4, Speed);
}

void Stop() {
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}

void hcsr04(){
    digitalWrite(TRIGGER, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
    if(trig1.repeat()){
    digitalWrite(TRIGGER, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
    }    
    if(trig2.repeat()){
    digitalWrite(TRIGGER, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
    }
    //FUNÇÃO RANGING, FAZ A CONVERSÃO DO TEMPO DE
    //RESPOSTA DO ECHO EM CENTIMETROS, E ARMAZENA
    //NA VARIAVEL "distancia"
    distancia = (ultrasonic.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
    result = String(distancia); //VARIÁVEL GLOBAL DO TIPO STRING RECEBE A DISTÂNCIA(CONVERTIDO DE INTEIRO PARA STRING)
 }

void frfid(){
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) //VERIFICA SE O CARTÃO PRESENTE NO LEITOR É DIFERENTE DO ÚLTIMO CARTÃO LIDO. CASO NÃO SEJA, FAZ
    return; //RETORNA PARA LER NOVAMENTE

  /***INICIO BLOCO DE CÓDIGO RESPONSÁVEL POR GERAR A TAG RFID LIDA***/
  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();
  /***FIM DO BLOCO DE CÓDIGO RESPONSÁVEL POR GERAR A TAG RFID LIDA***/

  Serial.print("Identificador (UID) da tag: "); //IMPRIME O TEXTO NA SERIAL
  Serial.println(strID); //IMPRIME NA SERIAL O UID DA TAG RFID

  rfid.PICC_HaltA(); //PARADA DA LEITURA DO CARTÃO
  rfid.PCD_StopCrypto1(); //PARADA DA CRIPTOGRAFIA NO PCD
}