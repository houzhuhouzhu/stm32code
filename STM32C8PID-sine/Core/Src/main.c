/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdlib.h"
#ifdef __GNUC__
	#define PUTCHAR_PROTOTYPE int _io_putchar(int ch)
#else 
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__*/
	
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 int sine[]={0,20,41,62,83,104,124,145,166,186,206,226,246,266,286,305,
324,343,362,381,399,417,435,452,469,486,502,518,534,549,564,579,
593,607,620,633,646,658,670,681,691,702,711,721,729,738,745,753,
759,766,771,776,781,785,789,792,794,796,797,798,799,798,797,796,
794,792,789,785,781,776,771,766,759,753,745,738,729,721,711,702,
691,681,670,658,646,633,620,607,593,579,564,549,534,518,502,486,
469,452,435,417,399,381,362,343,324,305,286,266,246,226,206,186,
166,145,124,104,83,62,41,20,0,-20,-41,-62,-83,-104,-124,-145,
-166,-186,-206,-226,-246,-266,-286,-305,-324,-343,-362,-381,-399,-417,-435,-452,
-469,-486,-502,-518,-534,-549,-564,-579,-593,-607,-620,-633,-646,-658,-670,-681,
-691,-702,-711,-721,-729,-738,-745,-753,-759,-766,-771,-776,-781,-785,-789,-792,
-794,-796,-797,-798,-799,-798,-797,-796,-794,-792,-789,-785,-781,-776,-771,-766,
-759,-753,-745,-738,-729,-721,-711,-702,-691,-681,-670,-658,-646,-633,-620,-607,
-593,-579,-564,-549,-534,-518,-502,-486,-469,-452,-435,-417,-399,-381,-362,-343,
-324,-305,-286,-266,-246,-226,-206,-186,-166,-145,-124,-104,-83,-62,-41,-20
 };
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
 
uint32_t getadc1value(uint32_t times)
{
   int sum=0,i,j;
	for(j=0;j<times;j++)
	{
		HAL_ADC_Start(&hadc1);
	  i=HAL_ADC_GetValue(&hadc1);
		sum=sum+i;
	}
 sum=sum/times;
	return 1000*sum*3.3/4096+45;
}

 int i=0,modulationrate=80;
 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
   if(htim==(&htim2))
    {
     TIM1->CCR1 = 800-(sine[i+1]*modulationrate/100);
			 if(i>239)
				 i=0;
			 i++;
    }

}



 struct PID
	{
		float Kp;
		float Ki;
		float Kd;
	  float setvalue;
		float readvalue1;
		float readvalue2;
		float erros;
		float E;
		float C;
		int temp;
	}pid;

	void PidInit()
	{
   pid.Kp=0.024;
   pid.Ki=0.015;
   pid.Kd=0.00;
	 pid.erros=0;
	 pid.C=40.0;
	 pid.E=0;
	 pid.readvalue1=0;
	}
void PidStart(float setting)
	{
	 pid.setvalue=setting;
	 pid.readvalue2=(float)getadc1value(256);
	 pid.erros= pid.setvalue-pid.readvalue2;
	 pid.E=pid.E+pid.erros;
	 if(pid.erros<25)
	 pid.C=pid.C+pid.Kp*pid.erros+pid.Ki*pid.E+pid.Kd*(pid.readvalue2-pid.readvalue1);
	 if(pid.erros>=25)
	 pid.C=pid.C+pid.Kp*pid.erros+pid.Kd*(pid.readvalue2-pid.readvalue1); 
	 	printf("pid.c  =  %d\r\n",(int)(pid.C));
	 printf("pid.readvalue  =  %f\r\n",pid.readvalue1);
	 pid.readvalue1=(float)getadc1value(256);
	}
#define end 1632
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
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init(64,83);
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  int x,i;
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
  TIM1->CCR1 = 800-sine[0]; 
	HAL_TIM_Base_Start_IT(&htim2);
	PidInit();
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  
  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
 
  /* USER CODE END 2 */
//modulationrate=(int)50;
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
   i=getadc1value(256);

		while(0)//abs(i-end)>35
		{
		  PidStart(end);
		  HAL_Delay(50);
		  i=getadc1value(256);
			printf("adc=%d\r\n",i);
			i=getadc1value(256);
			modulationrate=(int)pid.C;
			if((int)pid.C<2)
				modulationrate=10;
			if((int)pid.C>101)
				modulationrate=95;
			HAL_Delay(100);
			
			printf("erros= %f\r\n",pid.erros);
			printf("E= %f\r\n",pid.E);
			printf("modulationrate= %d\r\n",modulationrate);
			  
		}
   //	modulationrate=60;
    /* USER CODE BEGIN 3 */
  }
	
	while(1)
	{
	 i=getadc1value(256);
			printf("adc=  %d\r\n",i-588);
		HAL_Delay(100);
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
