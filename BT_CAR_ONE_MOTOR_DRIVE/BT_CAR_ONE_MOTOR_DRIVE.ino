#include <Ultrasonic.h>
#include <SoftwareSerial.h>
#include <HCSR04.h>
#include <Wire.h> //Biblioteca utilizada gerenciar a comunicação entre dispositicos através do protocolo I2C
#include <LiquidCrystal_I2C.h> //Biblioteca controlar display 16x2 através do I2C


// const int MPU=0x68;  
// int AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

// /* ---------- DISPLAY E ULTRASSOM ------------- */
// #define col  16 //Define o número de colunas do display utilizado
// #define lin   2 //Define o número de linhas do display utilizado
// #define ende  0x27 //Define o endereço do display
// LiquidCrystal_I2C lcd(ende, col, lin); 
// #define TRIGGER   8
// #define ECHO      9
// Ultrasonic ultrasonic(TRIGGER,ECHO);
// int distancia; 
// String result; 

/* ---------- PONTE H -------- */
#define in1 10                      
#define in2 11
#define in3 12
#define in4 13
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

// /* ----------- IR ------------ */
// const int pinIR = 5;
// int IR;

void setup() {
  // lcd.init(); //Inicializa a comunicação com o display já conectado
  // lcd.setBacklight(HIGH);
  // lcd.clear(); //Limpa a tela do display
  // lcd.setCursor(0, 0);
  // lcd.print("iniciando...");
  // delay(100);
  // lcd.clear();
  // lcd.display();

  Wire.begin();
  // Wire.beginTransmission(MPU);
  // Wire.write(0x6B); 
  // Wire.write(0); 
  // Wire.endTransmission(true);
  
  // pinMode(pinIR, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // pinMode(ECHO, INPUT); 
  // pinMode(TRIGGER, OUTPUT);

  Serial.begin(9600);
  bluetooth.begin(9600);  //INICIALIZA A SERIAL DO BLUETOOTH
  bluetooth.print("$");   //IMPRIME O CARACTERE
  bluetooth.print("$");   //IMPRIME O CARACTERE
  bluetooth.print("$");   //IMPRIME O CARACTERE
}

void loop() {
  // hcsr04();
  // giroscopio();
  // if(digitalRead(pinIR) == LOW){ IR = 0;}
  // if(digitalRead(pinIR) == HIGH){ IR = 1;}
  // lcd.setCursor(0,0);
  // lcd.print(result);    //Exibe no display as medidas
  // lcd.print("cm");
  // lcd.setCursor (7,0);
  // lcd.print(IR);
  // delay(2);
  // lcd.clear();
  
  
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

// void hcsr04(){
//     digitalWrite(TRIGGER, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
//     delayMicroseconds(2); //INTERVALO DE 2 MICROSSEGUNDOS
//     digitalWrite(TRIGGER, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
//     delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
//     digitalWrite(TRIGGER, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
//     //FUNÇÃO RANGING, FAZ A CONVERSÃO DO TEMPO DE
//     //RESPOSTA DO ECHO EM CENTIMETROS, E ARMAZENA
//     //NA VARIAVEL "distancia"
//     distancia = (ultrasonic.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
//     result = String(distancia); //VARIÁVEL GLOBAL DO TIPO STRING RECEBE A DISTÂNCIA(CONVERTIDO DE INTEIRO PARA STRING)
//  }

//  void giroscopio(){
//    Wire.beginTransmission(MPU);
//   Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
//   Wire.endTransmission(false);
//   //Solicita os dados do sensor
//   Wire.requestFrom(MPU,14,true);  
//   //Armazena o valor dos sensores nas variaveis correspondentes
//   AcX=Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
//   AcY=Wire.read()<<8|Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
//   AcZ=Wire.read()<<8|Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
//   Tmp=Wire.read()<<8|Wire.read();  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
//   GyX=Wire.read()<<8|Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
//   GyY=Wire.read()<<8|Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
//   GyZ=Wire.read()<<8|Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
   
//   //Envia valor X do acelerometro para a serial e o LCD
//  // lcd.setCursor(9,0);
// //  lcd.print("X=");
// //  lcd.print(AcX);
   
//   //Envia valor Y do acelerometro para a serial e o LCD
//  // lcd.setCursor(14,0);
//  // lcd.print("Y=");
//  // lcd.print(AcY);
   
//   //Envia valor Z do acelerometro para a serial e o LCD
//  // lcd.setCursor(0,1);
//  // lcd.print("Z=");
//   //lcd.print(AcZ);
   
//   //Envia valor da temperatura para a serial e o LCD
//   //Calcula a temperatura em graus Celsius
//   lcd.setCursor(0,1);
//   lcd.print("T:");
//   lcd.print(Tmp/340.00+36.53);
   
//   //Envia valor X do giroscopio para a serial e o LCD
//   //lcd.setCursor(10,1);
//   //lcd.print("X=");
//   //lcd.print(GyX);
   
//   //Envia valor Y do giroscopio para a serial e o LCD  
//   //lcd.setCursor(14,1);
//   //lcd.print("Y=");
//   //lcd.print(GyY);
   
//   //Envia valor Z do giroscopio para a serial e o LCD
//   //lcd.setCursor(13,3);
//   //lcd.print("Z=");
//   //lcd.print(GyZ);
   
//  }