#include <neotimer.h>
#include <MPU6050_tockn.h>
#include <Ultrasonic.h>
#include <SoftwareSerial.h>
#include <HCSR04.h>
#include <Wire.h> //Biblioteca utilizada gerenciar a comunicação entre dispositicos através do protocolo I2C
#include <LiquidCrystal_I2C.h> //Biblioteca controlar display 16x2 através do I2C
#include <SPI.h> //INCLUSÃO DE BIBLIOTECA
#include <MFRC522.h> //INCLUSÃO DE BIBLIOTECA
int flag;
/* -------- TEMPO ----------*/
Neotimer sensor1;
Neotimer trig1;
Neotimer trig2;
Neotimer tbuzzer;

/*--------- BUZZER ---------*/
#define buzzer 7

/* --------- RFID --------------*/
#define SS_PIN 10 //PINO SDA
#define RST_PIN 4 //PINO DE RESET
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
#define in1 5                   
#define in2 6
#define in3 9
#define in4 3
int Speed = 200;
int Speedsec;
int buttonState = 0;
int lastButtonState = 0;
int Turnradius = 0;
int brakeTime = 45;
int brkonoff = 1;

/* ---------- BLUETOOTH -------- */
const int pinoRX = 2;  //PINO DIGITAL 2 (RX)
const int pinoTX = 8;  //PINO DIGITAL 3 (TX)
int dadoBluetooth = 0;
SoftwareSerial bluetooth(pinoRX, pinoTX);

/* ----------- IR ------------ */
const int pinIR = A1;
int IR;

void setup() {
  Serial.begin(9600);
  tbuzzer.set(3000);
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

  delay(1000);
}

void loop() {
  bluetooth.listen();
  if(sensor1.repeat()){
  lcd.clear();
  hcsr04();
  frfid();

  mpu6050.update();

  anguloX = mpu6050.getGyroAngleX();
  anguloY = mpu6050.getGyroAngleY();
  anguloZ = mpu6050.getGyroAngleZ();
  temp = mpu6050.getTemp();
  Serial.print(anguloX);
  Serial.print("   |   ");
  Serial.print(anguloY);
  Serial.print("   |   ");
  Serial.print(anguloZ);
  Serial.print("   |   ");
  Serial.println(temp);

  if(digitalRead(pinIR) == LOW)
  {
    flag = 0;
    IR = 0;
  }

  if(digitalRead(pinIR) == HIGH)
  {
    if(flag==0){findtag();}
    IR = 1;
  }

  lcd.setCursor(0,0);
  lcd.print(result);    //Exibe no display as medidas
  lcd.print("cm");
  lcd.setCursor (5,0);
  lcd.print(IR);
  lcd.setCursor(7,0);
  lcd.print(temp);
  lcd.setCursor(0,1);
  lcd.print(anguloX);
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
    if (dadoBluetooth == '0') { Speed = 50; }
    if (dadoBluetooth == '1') { Speed = 60; }
    if (dadoBluetooth == '2') { Speed = 70; }
    if (dadoBluetooth == '3') { Speed = 80; }
    if (dadoBluetooth == '4') { Speed = 90; }
    if (dadoBluetooth == '5') { Speed = 100; }
    if (dadoBluetooth == '6') { Speed = 130; }
    if (dadoBluetooth == '7') { Speed = 160; }
    if (dadoBluetooth == '8') { Speed = 190; }
    if (dadoBluetooth == '9') { Speed = 210; }
    if (dadoBluetooth == 'q') { Speed = 255; }

  }
}

void rightless(){
  for(Speed = 35; Speed >= 30 ; Speed--)
  {  
    analogWrite(in3, Speed);
    analogWrite(in2, Speed);
  }
}

void forwardless(){
  for(Speed = 80; Speed >= 30 ; Speed--)
  {
    analogWrite(in1, Speed);
    analogWrite(in3, Speed);
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
  analogWrite(in4, Speed);
  analogWrite(in1, Speed);

}

void right() {
  analogWrite(in3, Speed);
  analogWrite(in2, Speed);

}
void forwardleft() {
  analogWrite(in1, Speed);
  analogWrite(in3, Speedsec);

}
void forwardright() {
  analogWrite(in1, Speedsec);
  analogWrite(in3, Speed);

}
void backright() {
  analogWrite(in2, Speedsec);
  analogWrite(in4, Speed);

}
void backleft() {
  analogWrite(in2, Speed);
  analogWrite(in4, Speedsec);

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
  tone(buzzer, 600, 300);  
}

void findtag(){
      float aux_x,aux_z,last_x,last_z,dif_x,dif_z;
      int alvo = 45;
    bluetooth.stopListening();
    flag=1;
    last_z = mpu6050.getGyroAngleZ();
    analogWrite(in1, 30);
    analogWrite(in3, 30);
    delay(1500);
    Stop();
    lcd.clear();
    while(distancia >= alvo){
      rightless();
      hcsr04();
      Stop();
      lcd.setCursor(0,0);
      lcd.print(distancia);
    }
    Stop();
    delay(300);

    while(distancia >= 8){
      analogWrite(in1, 38);
      analogWrite(in3, 30);

      hcsr04();
      mpu6050.update();
      frfid();
    }
    Stop();
    frfid();
    delay(1000);

    analogWrite(in2, 30);
    analogWrite(in4, 30);
    delay(500);
    Stop();
    delay(300);

    analogWrite(in2, 30);
    analogWrite(in4, 30);
    delay(2500);
    Stop();
}
