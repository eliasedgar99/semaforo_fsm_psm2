// estados.c
#include "estados.h"

EstadoSemaforo estado_actual = VERDE;
uint32_t tiempo_cambio = 0;
uint32_t tiempo_parpadeo = 0;
bool estado_led = false;
bool boton_presionado = false;

void cambiar_estado(EstadoSemaforo nuevo_estado) {
    estado_actual = nuevo_estado;
    tiempo_cambio = HAL_GetTick();

    switch (estado_actual) {
        case VERDE:
            HAL_GPIO_WritePin(LED_VERDE_PORT, LED_VERDE_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LED_ROJO_PORT, LED_ROJO_PIN, GPIO_PIN_RESET);
            break;
        case VERDE_PARPADEANTE:
        case ROJO_PARPADEANTE:
            tiempo_parpadeo = HAL_GetTick();
            estado_led = false;
            break;
        case ROJO:
            HAL_GPIO_WritePin(LED_VERDE_PORT, LED_VERDE_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_ROJO_PORT, LED_ROJO_PIN, GPIO_PIN_SET);
            break;
    }
}

void manejar_temporizador(void) {
    switch (estado_actual) {
        case VERDE_PARPADEANTE:
            if (HAL_GetTick() - tiempo_cambio >= 1000) {
                cambiar_estado(ROJO);
            }
            break;
        case ROJO:
            if (HAL_GetTick() - tiempo_cambio >= 3000) {
                cambiar_estado(ROJO_PARPADEANTE);
            }
            break;
        case ROJO_PARPADEANTE:
            if (HAL_GetTick() - tiempo_cambio >= 1000) {
                cambiar_estado(VERDE);
            }
            break;
        default:
            break;
    }
}

void manejar_parpadeo(void) {
    if (HAL_GetTick() - tiempo_parpadeo >= 200) {
        tiempo_parpadeo = HAL_GetTick();
        estado_led = !estado_led;

        if (estado_actual == VERDE_PARPADEANTE) {
            HAL_GPIO_WritePin(LED_VERDE_PORT, LED_VERDE_PIN, estado_led ? GPIO_PIN_SET : GPIO_PIN_RESET);
        } else if (estado_actual == ROJO_PARPADEANTE) {
            HAL_GPIO_WritePin(LED_ROJO_PORT, LED_ROJO_PIN, estado_led ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
    }
}
