#include <SoftwareSerial.h> //INCLUSÃO DE BIBLIOTECA
#define in1 11 //L298n Motor Driver pins.
#define in2 10
#define in3 9
#define in4 8
int dadoBluetooth = 0; //Int to store app command state.
int Speed = 204; // 0 - 255.
int Speedsec;
int buttonState = 0;
int lastButtonState = 0;
int Turnradius = 0; //Set the radius of a turn, 0 - 255 Note:the robot will malfunction if this is higher than int Speed.
int brakeTime = 45;
int brkonoff = 1; //1 for the electronic braking system, 0 for normal.
const int pinoRX = 3; //PINO DIGITAL 2 (RX)
const int pinoTX = 2; //PINO DIGITAL 3 (TX)
const int pinoLed = 13; //PINO DIGITAL UTILIZADO PELO LED
boolean lightFront = false; 
boolean loopLED = false;
SoftwareSerial bluetooth(pinoRX, pinoTX); //PINOS QUE EMULAM A SERIAL, ONDE
//O PINO 2 É O RX E O PINO 3 É O TX
 
void setup(){
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600); //INICIALIZA A SERIAL
  bluetooth.begin(9600); //INICIALIZA A SERIAL DO BLUETOOTH
  bluetooth.print("$"); //IMPRIME O CARACTERE
  bluetooth.print("$"); //IMPRIME O CARACTERE
  bluetooth.print("$"); //IMPRIME O CARACTERE
  delay(100); //INTERVALO DE 100 MILISSEGUNDOS
  pinMode(pinoLed, OUTPUT); //DEFINE O PINO COMO SAÍDA
}
 
void loop(){
  if(bluetooth.available()){ //SE O BLUETOOTH ESTIVER HABILITADO, FAZ
    dadoBluetooth = bluetooth.read(); //VARIÁVEL RECEBE O VALOR ENVIADO PELO BLUETOOTH
    Stop();
    if(dadoBluetooth == 'F'){ 
      Serial.println("forward"); 
      forward();
    }
    if(dadoBluetooth == 'B'){ 
      Serial.println("back");
      back();
    }
    if(dadoBluetooth == 'L'){ 
      Serial.println("left");
      left();
    }
    if(dadoBluetooth == 'R'){ 
      Serial.println("right");
      right();
    }
    if(dadoBluetooth == 'G'){ 
      Serial.println("fowardleft"); 
      forwardleft();
    }
    if(dadoBluetooth == 'I'){ 
      Serial.println("forwardright"); 
      forwardright();
    }
    if(dadoBluetooth == 'H'){ 
      Serial.println("backleft"); 
      backleft();
    }
    if(dadoBluetooth == 'I'){ 
      Serial.println("backright"); 
      backright();
    }
    if(dadoBluetooth == '0'){
      Serial.println("SPEED 100"); 
      Speed = 100;
    }
    if(dadoBluetooth == '1'){ 
      Serial.println("SPEED 140"); 
      Speed = 140;
    }
    if(dadoBluetooth == '2'){ 
      Serial.println("SPEED 150"); 
      Speed = 150;
    }
    if(dadoBluetooth == '3'){ 
      Serial.println("SPEED 160"); 
      Speed = 160;
    }
    if(dadoBluetooth == '4'){ 
      Serial.println("SPEED 170");
      Speed = 170;
    }
    if(dadoBluetooth == '5'){
      Serial.println("SPEED 180"); 
      Speed = 180;
    }
    if(dadoBluetooth == '6'){ 
      Serial.println("SPEED 190"); 
      Speed = 190;
    }
    if(dadoBluetooth == '7'){ 
      Serial.println("SPEED 200");
      Speed = 200;
    }
    if(dadoBluetooth == '8'){ 
      Serial.println("SPEED 210"); 
      Speed = 210;
    }
    if(dadoBluetooth == '9'){ 
      Serial.println("SPEED 240"); 
      Speed = 240;
    }
    if(dadoBluetooth == 'q'){ 
      Serial.println("SPEED 255"); 
      Speed = 255;
    }
    }else{ //SENÃO, FAZ
      loopLED = false; //VARIÁVEL RECEBE falso
    }
  
  //MÉTODO RESPONSÁVEL PELO LOOP (LIGA / DESLIGA) DO LED
  if(loopLED){
    digitalWrite(pinoLed, HIGH); //LIGA O LED
    Serial.println("LOOP (LED LIGADO)"); //IMPRIME O TEXTO NA SERIAL
    delay(500); //INTERVALO DE 500 MILISSEGUNDOS
    digitalWrite(pinoLed, LOW); //DESLIGA O LED
    Serial.println("LOOP (LED DESLIGADO)"); //IMPRIME O TEXTO NA SERIAL
    delay(500); //INTERVALO DE 500 MILISSEGUNDOS
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

void brakeOn() {
  //Here's the future use: an electronic braking system!
  // read the pushbutton input pin:
  buttonState = dadoBluetooth;
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == 'S') {
      if (lastButtonState != buttonState) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, HIGH);
        delay(brakeTime);
        Stop();
      }
    }
    // save the current state as the last state,
    //for next time through the loop
    lastButtonState = buttonState;
  }
}
void brakeOff() {

}