#include "parametros.h"
const int led_A_Pin =  2;
const int led_R_Pin = 32;
const int led_V_Pin = 33;

const int llave_Pin = 13;
const int pul_Arriba_Pin = 34;
const int pul_Abajo_Pin = 35;

const int PWMFreq = 1;
const int PWMChannel = 0;
const int PWMResolution = 16;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);
/*
 Para 16 bits de resolucion el PWM se puede mover entre 0-65535.
 El control del motor utiliza un ciclo de 20ms (50Hz) y asigna
 una velocidad = Min para 0.5ms, velocidad = 0 para 1ms y
 una velocidad = Max para 1.5ms. Eso significa que mover en 1 el PWM
 equivale a mover 0,0003 ms.
 Por seguridad voy a utilizar Vmin => 0.51ms y Vmax => 1.49ms.
 Entonces tenemos que Vmin => 1700, V0 => 3333 y Vmax => 4966.
 Esto da un rango de ajuste de 4966-1700 = 3266. 
 */

int v_min = 1700;
int V_0 = 3333;
int v_max = 4966;
int dutyCycle;
int estado_pulsador_arriba = 0;
int estado_pulsador_abajo = 0;
int velocidad = V_0;

unsigned int leer_pulsador(int pulsador_Pin){
int cantidad_lecturas = 4;
int tiempo_espera = 20;
int contador = 0;
int lectura;
int estado_pulsador_arriba;
int estado = 0;
int salida = 0;
  
  for(lectura = 0; lectura < cantidad_lecturas; lectura++){
    estado = digitalRead(pulsador_Pin);  
    contador = contador + estado;
    delay(tiempo_espera);
  }
  if (contador == 0){
    salida = 1;
    }
  
  return salida;
  }
  

void setup() {
  // put your setup code here, to run once:
  pinMode(led_A_Pin, OUTPUT);
  pinMode(led_R_Pin, OUTPUT);
  pinMode(led_V_Pin, OUTPUT);
  pinMode(llave_Pin, INPUT);
  pinMode(pul_Arriba_Pin, INPUT);
  pinMode(pul_Abajo_Pin, INPUT);
  Serial.begin(115200);
  ledcSetup(PWMChannel, PWMFreq, PWMResolution);
  /* Attach the LED PWM Channel to the GPIO Pin */
  ledcAttachPin(led_R_Pin, PWMChannel);
}

void loop() {

  digitalWrite(led_V_Pin, LOW);
  estado_pulsador_arriba = leer_pulsador(pul_Arriba_Pin);
  estado_pulsador_abajo = leer_pulsador(pul_Abajo_Pin);
  
  if (estado_pulsador_arriba == 1){
    velocidad = velocidad + 10;
    digitalWrite(led_V_Pin, HIGH);
    delay(100);
    }
  if (estado_pulsador_abajo == 1){
    velocidad = velocidad - 10;
    digitalWrite(led_V_Pin, HIGH);
    delay(100);
    }
  
  
  Serial.println(velocidad);
  Serial.println(numerito);
  
  if (velocidad <= v_max && velocidad >= v_min){
  ledcWrite(PWMChannel, velocidad);
  digitalWrite(led_A_Pin, LOW);
  }
  else {
  digitalWrite(led_A_Pin, HIGH);
  }
  
}
