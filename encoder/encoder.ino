#include <RotaryEncoder.h>

// S1   ...  PIN 12 cinza
// S2   ...  PIN 14 roxo
// KEY  ...  PIN 27 azul
// +    ...  3.3V verde
// GND  ...  GND branco

#include <Arduino.h>

#define ROTARY_S1 12   //ROTARY_PINA 2
#define ROTARY_S2 14   //ROTARY_PINB 4
#define ROTARY_KEY 27  //ROTARY_PINSW 5

int estadoAtual, estadoAnterior;
int posicao = 0;
volatile uint32_t debounceTimeout = 0;

portMUX_TYPE gpioMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR encoderShaft() {
  portENTER_CRITICAL_ISR(&gpioMux);
  estadoAtual = digitalRead(ROTARY_S1);

  if (debounceTimeout - xTaskGetTickCount() > 30) {
    if (estadoAtual != estadoAnterior) {
      if (digitalRead(ROTARY_S2) != estadoAtual) {
        posicao++;
        Serial.print("pos:");
        Serial.println(posicao);
        Serial.println("----------------------------");        
      }
      else {
        posicao--;
        Serial.print("pos:");
        Serial.println(posicao);
        Serial.println("----------------------------");
      }
      debounceTimeout = xTaskGetTickCount();
    }
  }
  portEXIT_CRITICAL_ISR(&gpioMux);
}

void IRAM_ATTR encoderButton() {
  portENTER_CRITICAL_ISR(&gpioMux);
  if (debounceTimeout - xTaskGetTickCount() > 100) {
      Serial.println("Enter!");
      debounceTimeout = xTaskGetTickCount();
  }
  portEXIT_CRITICAL_ISR(&gpioMux);
  }

void setup() {
  pinMode(ROTARY_S1, INPUT_PULLUP);
  pinMode(ROTARY_S2, INPUT_PULLUP);
  pinMode(ROTARY_KEY, INPUT_PULLUP);

  attachInterrupt(ROTARY_S1, encoderShaft, RISING);
  attachInterrupt(ROTARY_KEY, encoderButton, RISING);

  estadoAnterior = digitalRead(ROTARY_S1);
  Serial.begin(115200);
}


void loop() {
  delay(1000);
}
