/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_VERDE_PIN GPIO_PIN_3
#define LED_VERDE_PORT GPIOA
#define LED_ROJO_PIN GPIO_PIN_1
#define LED_ROJO_PORT GPIOA
#define SWITCH_PIN GPIO_PIN_2
#define SWITCH_PORT GPIOA
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
typedef enum {
  VERDE,
  VERDE_PARPADEANTE,
  ROJO,
  ROJO_PARPADEANTE
} EstadoSemaforo;

EstadoSemaforo estado_actual = VERDE;
uint32_t tiempo_cambio = 0;
uint32_t tiempo_parpadeo = 0;
bool estado_led = false;
bool boton_presionado = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

void cambiar_estado(EstadoSemaforo nuevo_estado);
void manejar_temporizador(void);
void manejar_parpadeo(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  // Estado inicial: LED verde encendido
  HAL_GPIO_WritePin(LED_VERDE_PORT, LED_VERDE_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_ROJO_PORT, LED_ROJO_PIN, GPIO_PIN_RESET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* Leer el estado del botón (con debounce simple) */
    if (HAL_GPIO_ReadPin(SWITCH_PORT, SWITCH_PIN) == GPIO_PIN_RESET && !boton_presionado) {
      boton_presionado = true;
      if (estado_actual == VERDE) {
        cambiar_estado(VERDE_PARPADEANTE);
      }
    } else if (HAL_GPIO_ReadPin(SWITCH_PORT, SWITCH_PIN) == GPIO_PIN_SET) {
      boton_presionado = false;
    }

    // Manejo de estados basado en tiempo
    if (HAL_GetTick() - tiempo_cambio >= 1000) {
      manejar_temporizador();
    }

    // Manejo del parpadeo (sin delay)
    if (estado_actual == VERDE_PARPADEANTE || estado_actual == ROJO_PARPADEANTE) {
      manejar_parpadeo();
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    while (1) {}
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
    while (1) {}
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure GPIO pins : LED_VERDE_PIN LED_ROJO_PIN */
  GPIO_InitStruct.Pin = LED_VERDE_PIN | LED_ROJO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure GPIO pin : SWITCH_PIN */
  GPIO_InitStruct.Pin = SWITCH_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
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


/* USER CODE END 4 */
