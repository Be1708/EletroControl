#include <Arduino.h>
#include <math.h>

// Configurações do sensor
const int pinSensor = A0;       // pino onde está conectado o sensor
const int numAmostras = 200;    // número de leituras para cálculo RMS
const float Vref = 5.0;         // tensão de referência do Arduino
const float Rload = 100.0;      // resistor de carga em ohms
const float N = 2000.0;         // relação de transformação: corrente do fio / corrente sensor
const float tolerancia = 0.2;
const float tolerancia1 = 1.0;
float valorArmazenado = 0;

void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  delay(2000);  // espera estabilizar
}

void loop() {
  float somaQuadrados = 0;
  float offset = 0;

  // Passo 1: medir o offset do sensor (tensão em 0 A)
  int somaOffset = 0;
  for (int i = 0; i < 100; i++) {
    somaOffset += analogRead(pinSensor);
    delay(1);
  }
  offset = somaOffset / 100.0;

  // Passo 2: medir corrente RMS
  for (int i = 0; i < numAmostras; i++) {
    int sensorValue = analogRead(pinSensor);
    
    // Corrige offset
    float vAc = sensorValue - offset;
    
    // Converte leitura para tensão
    float V = vAc * Vref / 1023.0;

    // Corrente no sensor
    float I_sensor = V / Rload;

    // Corrente real no fio
    float I_fio = I_sensor * N;

    // Soma dos quadrados para RMS
    somaQuadrados += I_fio * I_fio;
  }

  float correnteRMS = sqrt(somaQuadrados / numAmostras);

  correnteRMS = correnteRMS/1.45;

  if((correnteRMS - valorArmazenado) > tolerancia || (correnteRMS - valorArmazenado) <- tolerancia1) {
      Serial.println("passou da tolerância");
      valorArmazenado = correnteRMS;
  }

  int valorEnviado = valorArmazenado*30;

  if (valorEnviado < 255) {
    analogWrite(3, valorEnviado);
  } else {
    analogWrite(3, 255);
  }
  

  Serial.print("Corrente RMS: ");
  Serial.print(valorArmazenado, 2);
  Serial.println(" A");

  delay(1000);
}