// estados.c
#include "estados.h"

EstadoSemaforo estado_actual = VERDE;
uint32_t tiempo_cambio = 0;
uint32_t tiempo_parpadeo = 0;
bool estado_led = false;
bool boton_presionado = false;

// Funciones de condición para las transiciones de estados
static int is_timer_expired(uint32_t last_time, uint32_t duration) {
    return (HAL_GetTick() - last_time) >= duration;
}

int is_button_pressed(void) {
    return HAL_GPIO_ReadPin(SWITCH_PORT, SWITCH_PIN) == GPIO_PIN_RESET;
}

// Acciones para los estados
void accion_verde(void) {
    HAL_GPIO_WritePin(LED_VERDE_PORT, LED_VERDE_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_ROJO_PORT, LED_ROJO_PIN, GPIO_PIN_RESET);
}

void accion_rojo(void) {
    HAL_GPIO_WritePin(LED_VERDE_PORT, LED_VERDE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_ROJO_PORT, LED_ROJO_PIN, GPIO_PIN_SET);
}

void accion_parpadeo_inicial(void) {
    tiempo_parpadeo = HAL_GetTick();
    estado_led = false;
}

// Cambiar estado basado en condiciones
void cambiar_estado(EstadoSemaforo nuevo_estado) {
    estado_actual = nuevo_estado;
    tiempo_cambio = HAL_GetTick();

    if (estado_actual == VERDE) {
        accion_verde();
    } else if (estado_actual == VERDE_PARPADEANTE || estado_actual == ROJO_PARPADEANTE) {
        accion_parpadeo_inicial();
    } else if (estado_actual == ROJO) {
        accion_rojo();
    }
}

void manejar_temporizador(void) {
    if (estado_actual == VERDE_PARPADEANTE && is_timer_expired(tiempo_cambio, 1000)) {
        cambiar_estado(ROJO);
    } else if (estado_actual == ROJO && is_timer_expired(tiempo_cambio, 3000)) {
        cambiar_estado(ROJO_PARPADEANTE);
    } else if (estado_actual == ROJO_PARPADEANTE && is_timer_expired(tiempo_cambio, 1000)) {
        cambiar_estado(VERDE);
    }
}

void manejar_parpadeo(void) {
    if (is_timer_expired(tiempo_parpadeo, 200)) {
        tiempo_parpadeo = HAL_GetTick();
        estado_led = !estado_led;

        if (estado_actual == VERDE_PARPADEANTE) {
            HAL_GPIO_WritePin(LED_VERDE_PORT, LED_VERDE_PIN, estado_led ? GPIO_PIN_SET : GPIO_PIN_RESET);
        } else if (estado_actual == ROJO_PARPADEANTE) {
            HAL_GPIO_WritePin(LED_ROJO_PORT, LED_ROJO_PIN, estado_led ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
    }
}
