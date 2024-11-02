// estados.h
#ifndef ESTADOS_H
#define ESTADOS_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>

// Definicion de los estados
typedef enum {
    VERDE,
    VERDE_PARPADEANTE,
    ROJO,
    ROJO_PARPADEANTE
} EstadoSemaforo;

extern EstadoSemaforo estado_actual;
extern uint32_t tiempo_cambio;
extern uint32_t tiempo_parpadeo;
extern bool estado_led;
extern bool boton_presionado;

#define LED_VERDE_PORT GPIOA
#define LED_VERDE_PIN GPIO_PIN_3
#define LED_ROJO_PORT GPIOA
#define LED_ROJO_PIN GPIO_PIN_1
#define SWITCH_PORT GPIOA
#define SWITCH_PIN GPIO_PIN_2

void cambiar_estado(EstadoSemaforo nuevo_estado);
void manejar_temporizador(void);
void manejar_parpadeo(void);

#endif // ESTADOS_H
