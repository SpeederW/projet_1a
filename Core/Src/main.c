/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "lcd16x2.h"
#include "midi_data_process.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define STATE_PLAY	 1
#define STATE_SELECT 2
#define STATE_WAIT_FOR_DATA 3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
unsigned int counter = 0;
unsigned int current_state = STATE_SELECT;
unsigned int previous_state = STATE_SELECT;
uint8_t rx_buffer[32];
int previous_count;
int data_size_received;
int has_finished_playing = 0;
int is_waiting_for_data = 0;
uint8_t size = 0;
volatile unsigned int LCD_Update_Required;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief Initialise l'affichage LCD avec les informations de séquence.
 *
 * Cette fonction efface l'écran du LCD, place le curseur à la première ligne,
 * affiche le numéro de séquence courant, puis place le curseur à la deuxième ligne
 * pour afficher la séquence des chiffres de 1 à 8.
 *
 * @note La variable globale 'counter' doit être définie et initialisée avant l'appel.
 * @note La sélection des séquences courantes via le codeur rotatif n'est pas utile dans cette implémentation, mais est fonctionnelle.
 */
void LCD_init() {
	lcd16x2_clear();
	lcd16x2_setCursor(0, 0);
	lcd16x2_printf("Sequence: %d", counter);
	lcd16x2_setCursor(1, 0);
	lcd16x2_printf("1 2 3 4 5 6 7 8");
}

/**
 * @brief Met à jour l'affichage de l'écran LCD en fonction de l'état courant du système.
 *
 * Cette fonction gère l'affichage sur un écran LCD 16x2 selon l'état de l'application :
 * - Affiche la valeur du compteur à la position (0, 10).
 * - Si l'état courant est STATE_SELECT, initialise l'écran LCD si nécessaire et positionne le curseur.
 * - Si l'état courant est STATE_PLAY, efface l'écran et affiche "Lecture sequence".
 * - Si l'état courant est STATE_WAIT_FOR_DATA, efface l'écran et affiche "Attente donnees".
 *
 * @note Cette fonction utilise les variables globales `counter`, `current_state` et `previous_state`.
 */
void LCD_draw() {
	lcd16x2_setCursor(0, 10);
	lcd16x2_printf("%d", counter+1);
	if(current_state == STATE_SELECT){
		if(previous_state != STATE_SELECT) {
			LCD_init();
			previous_state = STATE_SELECT;
		}
		lcd16x2_setCursor(1, counter*2);
	} else if(current_state == STATE_PLAY) {
		lcd16x2_clear();
		lcd16x2_printf("Lecture sequence");
	} else if(current_state == STATE_WAIT_FOR_DATA) {
		lcd16x2_clear();
		lcd16x2_printf("Attente donnees");
	}
}

/**
 * @brief Met à jour la valeur du compteur en fonction de l'état de l'encodeur.
 *
 * Cette fonction compare la valeur actuelle de l'encodeur (count) à la valeur précédente (previous_count)
 * pour déterminer le sens de rotation :
 *   - Si la différence est positive, l'encodeur tourne dans le sens horaire et le compteur est incrémenté.
 *   - Si la différence est négative, l'encodeur tourne dans le sens antihoraire et le compteur est décrémenté.
 * La variable previous_count est ensuite mise à jour avec la nouvelle valeur.
 * Un drapeau (LCD_Update_Required) est activé pour indiquer qu'une mise à jour de l'affichage est nécessaire.
 * Enfin, la valeur du compteur est bornée entre 0 et 8 (inclus) pour éviter les débordements.
 *
 * @param count Valeur actuelle lue de l'encodeur.
 */
void encoder_update(int count) {
	if(count - previous_count > 0) { // Clockwise
		counter++;
		previous_count = count;
		LCD_Update_Required = 1;
	} else if(count - previous_count < 0){ // Counter clockwise
		counter--;
		previous_count = count;
		LCD_Update_Required = 1;
	}
	// Clamp
	if(counter < 0) {
		counter = 8;
	} else if(counter > 8) {
		counter = 0;
	}
}

/**
 * @brief  Callback appelée lors d'une interruption externe sur une broche GPIO.
 * 
 * Cette fonction gère les actions à effectuer lorsqu'un événement d'interruption
 * est détecté sur une broche GPIO spécifique (par exemple, un bouton poussoir).
 * - Si l'interruption provient du bouton poussoir et que l'état courant est STATE_PLAY,
 *   la variable has_finished_playing est mise à 1 pour indiquer la fin de la lecture.
 * - Sinon, l'état courant passe à STATE_WAIT_FOR_DATA, la taille des données reçues
 *   est réinitialisée, le système passe en attente de données, et une mise à jour
 *   de l'affichage LCD est demandée.
 * - Pour toute autre broche, la mise à jour de l'affichage LCD est désactivée.
 * 
 * @param  GPIO_Pin  Broche GPIO ayant déclenché l'interruption.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch(GPIO_Pin) {
		case BUTTON_Push_Pin:
			if(current_state == STATE_PLAY) {
				has_finished_playing = 1;
			} else {
				current_state = STATE_WAIT_FOR_DATA;
				data_size_received = 0;
				is_waiting_for_data = 1;
				LCD_Update_Required = 1;
			}
			break;
		default:
			LCD_Update_Required = 0;
			break;
	}
}

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
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  lcd16x2_init_4bits(LCD_RS_GPIO_Port, LCD_RS_Pin, LCD_E_Pin,
  LCD_D4_GPIO_Port, LCD_D4_Pin, LCD_D5_Pin, LCD_D6_Pin, LCD_D7_Pin);
  lcd16x2_cursorShow(1);
  lcd16x2_clear();
  LCD_init();
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	/*
	HAL_GPIO_TogglePin(GPIOA, OUT1_Pin);
	HAL_GPIO_TogglePin(GPIOA, OUT2_Pin);
	HAL_Delay(300);
	*/

  // Met à jour la valeur du compteur selon l'encodeur rotatif
  encoder_update((TIM3->CNT)>>2);

  // Rafraîchit l'affichage LCD si nécessaire
  if(LCD_Update_Required) {
    LCD_draw();
    LCD_Update_Required = 0;
  }

  // Gestion de l'état de lecture de séquence
  if(current_state == STATE_PLAY) {
    // Si la lecture est terminée, envoie un message à l'UART et repasse en mode sélection
    if(has_finished_playing) {
      uint8_t msg[] = "STM - Lecture finie.";
      HAL_UART_Transmit(&huart1, msg, sizeof(msg), 1000);
      previous_state = current_state;
      current_state = STATE_SELECT;
      has_finished_playing = 0;
      LCD_Update_Required = 1;
    }
    // Réception des données à traiter via UART
    HAL_UART_Receive(&huart1, rx_buffer, size, 50);
    // Traite les données reçues, met à jour l'état de lecture
    has_finished_playing = process_data(rx_buffer, size);
    // Attend la taille du prochain paquet de données
    while(HAL_UART_Receive(&huart1, &size, 1, 1000) != HAL_OK)
      HAL_Delay(1);
    HAL_GPIO_TogglePin(GPIOB, LED_Pin);

  // Gestion de l'état d'attente de données
  } else if(current_state == STATE_WAIT_FOR_DATA) {
    // Si plus en attente, passe à l'état de lecture
    if(!is_waiting_for_data) {
      previous_state = current_state;
      current_state = STATE_PLAY;
      LCD_Update_Required = 1;
    }
    // Attend la réception de la taille des données via UART
    if(HAL_UART_Receive(&huart1, &size, 1, 1000) == HAL_OK) {
      is_waiting_for_data = 0;
    }
    HAL_GPIO_TogglePin(GPIOB, LED_Pin);
  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 99;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_RS_Pin|LCD_E_Pin|OUT1_Pin|LCD_D4_Pin
                          |LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin|OUT2_Pin
                          |OUT3_Pin|OUT4_Pin|OUT5_Pin|OUT6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OUT7_GPIO_Port, OUT7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_RS_Pin LCD_E_Pin OUT1_Pin LCD_D4_Pin
                           LCD_D5_Pin LCD_D6_Pin LCD_D7_Pin OUT2_Pin
                           OUT3_Pin OUT4_Pin OUT5_Pin OUT6_Pin */
  GPIO_InitStruct.Pin = LCD_RS_Pin|LCD_E_Pin|OUT1_Pin|LCD_D4_Pin
                          |LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin|OUT2_Pin
                          |OUT3_Pin|OUT4_Pin|OUT5_Pin|OUT6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTTON_Push_Pin */
  GPIO_InitStruct.Pin = BUTTON_Push_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(BUTTON_Push_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_Pin OUT7_Pin */
  GPIO_InitStruct.Pin = LED_Pin|OUT7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
