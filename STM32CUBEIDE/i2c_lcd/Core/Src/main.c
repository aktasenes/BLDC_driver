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
#include "stdio.h"
#include <stdbool.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint8_t Buffer[25] = {0};
uint8_t Space[] = " - ";
uint8_t StartMSG[] = "Starting I2C Scanning: \r\n";
uint8_t EndMSG[] = "Done! \r\n\r\n";
int pwm_value=30;
bool HALL1 = false;
bool HALL2 = false;
bool HALL3 = false;
uint32_t step=1;
int pwm_eski;
bool saat_yon = true;
int rpm;
bool rpm_ok=false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_ADC2_Init(void);
/* USER CODE BEGIN PFP */

void uart_gonder(char *deger)
{
	HAL_UART_Transmit(&huart3, (uint8_t*)deger, strlen(deger), 100);
}

void basla (void) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);	//L1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);	//L2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);	//L3
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);	//H1
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,0);	//H2
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);	//H3

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_Delay(950);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_Delay(950);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_Delay(950);
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);	//H3
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,100);	//H1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);	//L2
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);	//L2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1);	//L3
	HAL_Delay(10);
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);	//H1
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,100);	//H2
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);	//L3
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);	//L1
	HAL_Delay(10);
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,0);	//H2
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,100);	//H3
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);	//L1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);	//L2
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

   HALL1=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
   HALL2=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
   HALL3=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
if(saat_yon==true)
{
   if(HALL1&&!HALL2&&HALL3){//101
         step=4;
      }
   if(HALL1&&!HALL2&&!HALL3){//100
         step=5;
      }
   if(HALL1&&HALL2&&!HALL3){//110
         step=6;
      }
   if(!HALL1&&HALL2&&!HALL3){//010
         step=1;
      }
   if(!HALL1&&HALL2&&HALL3){//011
         step=2;
      }
   if(!HALL1&&!HALL2&&HALL3){//001
         step=3;
      }
}
else if(saat_yon == false)
{
	if(HALL1&&!HALL2&&HALL3){//101
			step=1;
		}
	if(HALL1&&!HALL2&&!HALL3){//100
			step=2;
		}
	if(HALL1&&HALL2&&!HALL3){//110
			step=3;
		}
	if(!HALL1&&HALL2&&!HALL3){//010
			step=4;
		}
	if(!HALL1&&HALL2&&HALL3){//011
			step=5;
		}
	if(!HALL1&&!HALL2&&HALL3){//001
			step=6;
		}
}
   switch(step)
   {
      case 1:
           HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);//CL=L
           HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);//AL=L
           __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,0);//BH=L
         __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);//CH=L
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);//BL=H
          __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,pwm_value);//AH=H PWM
      break;
      case 2:
         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);//BL=L
         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);//AL=L
         __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,0);//BH=L
         __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);//CH=L
         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1);//CL=H
         __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,pwm_value);//AH=H PWM
      break;
      case 3:
         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);//BL=L
           HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);//AL=L
           __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);//AH=L
           __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);//CH=L
           HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1);//CL=H
           __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,pwm_value);//BH=H PWM
      break;
      case 4:
         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);//BL=L
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);//CL=L
          __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);//AH=L
         __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);//CH=L
         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);//AL=H
          __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,pwm_value);//BH=H PWM
      break;
      case 5:
         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);//BL=L
         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);//CL=L
         __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);//AH=H
         __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,0);//BH=L
         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);//AL=H
         __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,pwm_value);//CH=H PWM
      break;
      case 6:
         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);//AL
           HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);//CL
         __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);//AH=L
         __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,0);//BH=L
         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);//BL=H
         __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,pwm_value);//CH=H PWM
         rpm++;
		  if(rpm==100)
		  {
			 rpm_ok=true;
			 rpm=0;
		  }
      break;
   }
}


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
	   int sira=1;
	   int i;
	   uint8_t acilis[] = "Sistem Basliyor\r\n";
	   uint8_t hazir[] = "Sistem Hazir\r\n";
	uint8_t Rx_data=0;
	uint8_t MSG2[] = "Sistem Hazir\r\n";
	uint16_t raw;
	uint16_t raw1;
	float volt;
	float akim;
	char buffer[32]={0};
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
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
  printf("\n");
  HAL_UART_Transmit(&huart3, MSG2, sizeof(MSG2), 100);
  HAL_Delay(1000);
  HAL_UART_Transmit(&huart3, MSG2, sizeof(MSG2), 100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  HAL_UART_Transmit(&huart3, acilis, sizeof(acilis), 100);
  HAL_Delay(20);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);//A PWM START
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);// B PWM START
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);// C PWM START
  HAL_Delay(50);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_UART_Transmit(&huart3, hazir, sizeof(hazir), 100);

  basla();

  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  	  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)==1)
  	  {
  		  if(pwm_value==0){
  			pwm_value=pwm_value+10;
  			basla;
  		  }
  		  else{
  			  pwm_value=pwm_value+10;
  		  }
  		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
  		HAL_Delay(10);
  		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
  		  if(pwm_value>290)
  		  {
  			pwm_value=290;
  		  }
  		  HAL_Delay(100);
  	  }
	  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)==1)
	  	  {
		  pwm_value=pwm_value-10;
	  		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
	  		HAL_Delay(10);
	  		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	  		  if(pwm_value<0)
	  		  {
	  			pwm_value=0;
	  		  }
	  		  HAL_Delay(100);
	  	  }

	  	HAL_UART_Receive (&huart3, &Rx_data, 2, 100);
	  	if(Rx_data=='1')
	  	{
	  		  if(pwm_value==0){
	  			pwm_value=pwm_value+10;
	  			basla;
	  		  }
	  		  else{
	  			  pwm_value=pwm_value+10;
	  		  }
	  		  if(pwm_value>290)
	  		  {
	  			pwm_value=290;
	  		  }
	  		  HAL_Delay(100);
	  	}
	  	if(Rx_data=='3')
	  	{
	  		pwm_value=pwm_value-10;
	  		  if(pwm_value<0)
	  		  {
	  			pwm_value=3;
	  		  }
	  		  HAL_Delay(100);
	  	}
	  	if(Rx_data=='0')
	  	{
	  			pwm_eski=pwm_value;
		  		pwm_value=0;
		  		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
		  		  HAL_Delay(100);

	  		}
	  		if(Rx_data=='2'){
		  		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
		  		pwm_value=pwm_eski;
		  		basla;
		  		  HAL_Delay(100);

	  		}
	  		if(Rx_data=='4'){
		  		basla;
		  		saat_yon=false;
		  		  HAL_Delay(100);
	  		}
	  		if(Rx_data=='5'){
		  		basla;
		  		saat_yon=true;
		  		  HAL_Delay(100);
	  		}
	  		if(Rx_data=='6'){
		  		pwm_value=pwm_eski;
		  		pwm_value==0;
		  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
		  	  HAL_Delay(500);
		  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
		  	  HAL_Delay(500);
	  		}

	  	  //__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,PWM_deger);
	  	  //__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,PWM_deger_1);
	  	  //TIM1->CCR1 = PWM_deger;
	  	  //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	  	  //TIM4->CCR4 = PWM_deger;
	  	//HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	  	//htim4.Instance->CCR1 = 20;
	  	  //HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	  	  //TIM1->CCR2 = PWM_deger;
	  	  //HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
		  //HAL_Delay(1000);
	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 100);
	  raw = HAL_ADC_GetValue(&hadc1);
	  //volt=((((raw*3.3)/4096)*53.8)/2.7);
	  HAL_ADC_Stop (&hadc1);
	  HAL_Delay(2);
	  HAL_ADC_Start(&hadc2);
	  HAL_ADC_PollForConversion(&hadc2, 100);
	  raw1 = HAL_ADC_GetValue(&hadc2);
	  //akim=(((raw1*3.3)/2048)/96)*100;
	  HAL_ADC_Stop (&hadc2);
	  if(rpm_ok==true)
	  {
		  uint32_t len=sprintf(buffer, "%d*%d*%d*1\r\n", raw, raw1, pwm_value);
		  rpm_ok=false;
		  HAL_UART_Transmit(&huart3, (uint16_t*)buffer, len, 100);
	  }
	  else{
		  uint32_t len=sprintf(buffer, "%d*%d*%d*0\r\n", raw, raw1, pwm_value);
		  HAL_UART_Transmit(&huart3, (uint16_t*)buffer, len, 100);
	  }
	  //uint32_t len=sprintf(buffer, "%d*%d*%d\r\n", raw, raw1, pwm_value);
	  //uart_gonder(buffer);
	  //HAL_UART_Transmit(&huart3, (uint16_t*)buffer, len, 100);
/*
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);	//H3
		__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,pwm_value);	//H1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);	//L2
		HAL_Delay(3);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);	//L2
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1);	//L3
		HAL_Delay(3);
		__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);	//H1
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,pwm_value);	//H2
		HAL_Delay(3);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);	//L3
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);	//L1
		HAL_Delay(3);
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,0);	//H2
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,pwm_value);	//H3
		HAL_Delay(3);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);	//L1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);	//L2
		HAL_Delay(3);
		*/
	  /*
	     if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)==1)
	          {
	             pwm_value=pwm_value-10;
	             if(pwm_value<0)
	             {
	                pwm_value=0;
	             }
	             HAL_Delay(500);
	          }
	          if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)==1)
	          {
	             pwm_value=pwm_value+10;
	             if(pwm_value>300)
	             {
	                pwm_value=300;
	             }
	             HAL_Delay(500);
	          }
*/
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 11;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 299;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 11;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 299;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();

  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Buzzer_Pin|L1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, L2_Pin|L3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Buzzer_Pin */
  GPIO_InitStruct.Pin = Buzzer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Buzzer_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PWM__Pin PWM_B1_Pin */
  GPIO_InitStruct.Pin = PWM__Pin|PWM_B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : L1_Pin */
  GPIO_InitStruct.Pin = L1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(L1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : L2_Pin L3_Pin */
  GPIO_InitStruct.Pin = L2_Pin|L3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
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
