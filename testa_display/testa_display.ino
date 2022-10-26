#include <HCSR04.h>
#include <Wire.h> //Biblioteca utilizada gerenciar a comunicação entre dispositicos através do protocolo I2C
#include <LiquidCrystal_I2C.h> //Biblioteca controlar display 16x2 através do I2C

#define col  16 //Define o número de colunas do display utilizado
#define lin   2 //Define o número de linhas do display utilizado
#define ende  0x27 //Define o endereço do display
LiquidCrystal_I2C lcd(ende, col, lin); //Cria o objeto lcd passando como parâmetros o endereço, o nº de colunas e o nº de linhas

#define TRIGGER   12
#define ECHO      13
UltraSonicDistanceSensor distan,
ceSensor(TRIGGER, ECHO); 

void setup() {
  lcd.init(); //Inicializa a comunicação com o display já conectado
  lcd.clear(); //Limpa a tela do display
  lcd.backlight(); //Aciona a luz de fundo do display

  lcd.setCursor(0, 0); //Coloca o cursor do display na coluna 1 e linha 1
  //lcd.print("BLOG DA ROBOTICA"); //Exibe a mensagem na primeira linha do display
  //lcd.setCursor(0, 1); //Coloca o cursor do display na coluna 1 e linha 2
  //lcd.print("TUTORIAL DISPLAY");  //Exibe a mensagem na segunda linha do display*/

  Serial.begin(9600);
  Wire.begin();
  lcd.display();// Liga Display:
}

void loop() {
  int distancia = 0;
  distancia = distanceSensor.measureDistanceCm();
  lcd.print("      ");
  lcd.print(distancia);    //Exibe no display as medidas
  lcd.print(" cm");
  delay(100);
  lcd.clear();                      //Apaga o que foi escrito no display
}
