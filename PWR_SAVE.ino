//PROJETO PWR SAVE 2.0
//DATA = 12/11/17
//VERSION 1.3

//BIBLIOTECAS USADAS
#include "EmonLib.h"
#include "SoftwareSerial.h"

//VARIAVEIS DO BLUETOOTH
String readBluetooth;
String envioV, envioI, envioTudo;
int RX = 6;
int TX = 7;
SoftwareSerial bluetooth(RX, TX); // RX, TX

//PARA ENVIO
String conteudo = "";
char caractere;

//VARIAVEIS DE CORRENTE
EnergyMonitor emon1;
int pino_sct = A6; //pino analogico SCT
float Irms = 0.0;
float mediaI=0.0;

//VARIAVEIS DO SENSOR DE TENSAO
int pino = A7; //porta analogica P8
int i = 0;
int leitura = 0;
double soma = 0.0;
double resp = 0.0;

void setup()
{
  Serial.begin(9600);
  Serial.println("--- POWER SAVE 2.0 ---");
  bluetooth.begin(38400); //baud rate para o modulo AT (38400)

  //PINO DIGITAL DO LED
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  delay(300);
  digitalWrite(8, LOW);
  delay(300);
  digitalWrite(8, HIGH);
  delay(300);
  digitalWrite(8, LOW);
  delay(300);
  digitalWrite(8, HIGH);
  delay(300);
  digitalWrite(8, LOW);

  //Pino, calibracao - Cur Const= Ratio/BurdenR. 2000/33 = 60
  emon1.current(pino_sct, 11.1);
  
  Irms = emon1.calcIrms(1480);
  delay(100); //tempo para inicializar e estabilizar
  Irms = emon1.calcIrms(1480);
}

void loop()
{
  delay(5000);//FIM DE LOOPE E RENICIANDO
  
  // Enquanto receber algo pela serial
  while (bluetooth.available() > 0) {
    // LÃª byte da serial
    caractere = bluetooth.read();
    // Ignora caractere de quebra de linha
    if (caractere != '\n') {
      // Concatena valores
      conteudo.concat(caractere);
    }
    // Aguarda buffer serial ler prÃ³ximo caractere
    delay(70);
  }


  delay(100);

  //SENSOR DE CORRENTE

  
  Irms = emon1.calcIrms(1480);
 
  if(Irms<0.15){
    Irms=0.0;
  }
  envioI = "I=" + String(Irms);

  delay(100);//intervalo entre a leitura dos sensores

  //SENSOR DE TENSAO
  for (i = 0; i < 1000; i++) {
    leitura = analogRead(pino);
    soma = soma + leitura;
  }
  soma = soma / 1000.0;
  resp = (log(soma) * 80.761) - 255.94; //calculo baseado em ponto recolhidos por multimetro e sensor
  if(resp < 0){
    resp=0.0;
  }
  envioV = "V=" + String(resp);

  //MONTANDO STRING PARA ENVIO POR BLUETOOTH
  envioTudo = envioV + envioI;
  Serial.println(envioTudo);
  bluetooth.println(envioTudo);
 
  //SE ALGUM APARELHO ESTIVER LIGADO
  if (Irms > 0.15) {
    digitalWrite(8, HIGH);
  }
  else {
    digitalWrite(8, LOW);
  }

  conteudo = "";
  envioI = "";
  envioV = "";
  envioTudo = "";
  soma = 0.0;
  resp = 0.0;
  Irms = 0.0;

  
}

