
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);

void tx_data(void);
void kiem_tra_sim(void);
void send_data_server(void);

char bien_kiem_tra[] = "KT\r\n";
char bien_ok[] = "OK\r\n";
char kt_ok[] = "KT HOAN TAT\r\n";

char tx3_buffer[100];
char tx1_buffer[100];

uint8_t rx3;
uint8_t rx1;

char rx3_buffer_dem[100];
char rx1_buffer_dem[100];

char rx3_buffer[100];
char rx1_buffer[100];

int cnt1=0;
int flag1=0;

int cnt3=0;
int flag3=0;

char check_module[] = "AT\r\n";									// test module
char kt_modun_ok[]= "KT modun OK\r\n";

char check_sim[] = "AT+CPIN?\r\n";				// test sim
char kt_sim_ok[]= "KT sim OK\r\n";

char check_cuong_do[] = "AT+CSQ\r\n";			// cuong do tin hieu
char kt_cuong_do_ok[]= "KT cuong do OK\r\n";

char check_dang_ky[] = "AT+CREG?\r\n";				// kiem tra dang ky mang
char kt_dky_ok[]= "KT dang ky OK\r\n";

char check_GPRS[] = "AT+CGATT?\r\n";					// kiem tra haot dong cuua GPRS
char kt_gprs_ok[]= "KT GPRS OK\r\n";

char set_timeout[] = "AT+CIPTIMEOUT=30000,20000,40000,50000\r\n";    // cai dat thoi gian cho
char set_timeout_ok[]= "Set timeout  OK\r\n";

char set_transmit[] = "AT+CIPMODE=0\r\n";     // cai dat che do truyen
char set_transmit_ok[]= "Set transmit OK\r\n";

char check_net[] ="AT+NETOPEN\r\n";           // kiem tra internet
char kt_net_ok[]= "KT NET OK\r\n";

char check_ip[] ="AT+IPADDR\r\n";							// kiem tra dia chi IP
char kt_ip_ok[]= "KT IP OK\r\n";

char set_receive[] = "AT+CIPRXGET=0,1\r\n";		// cai dat che do nhan
char set_receive_ok[]= "KT receive OK\r\n";

char allow_transmit[] ="AT+CIPSEND=1,4\r\n";
char allow_transmit_ok[] ="Allow transmit OK\r\n";

char allow_receive[] ="AT+CIPRXGET=0,1\r\n";
char allow_receive_ok[] ="Allow receive OK\r\n";

char send_data_sim_dem1[100] = "AT+CIPSEND=1,,,,";
char send_data_sim[100];
char send_data_sim_dem2[100];

char send_data_tx1[100]= "Transmited: ";


/*
char connect_tcp[44]= {'A','T','+','C','I','P','O','P','E','N','=','1',',','"','T','C','P','"',',',
											'"','1','1','7','.','6','.','1','6','3','.','1','7','7','"',',',
											'7','5','7','9',',','0','"', 0x0D, 0x0A};
*/
//  AT+CIPOPEN=1,"TCP","117.6.163.177",7577,0     117.6.163.177
char connect_tcp[100];
char connect_tcp1[] ={'A','T','+','C','I','P','O','P','E','N','=','1',',','"','T','C','P','"',',','"','\0'};
char connect_tcp2[] ="117.6.163.177";
char connect_tcp3[] ={'"',',','\0'};
char connect_tcp4[] ="6666";
char connect_tcp5[] ={',','0', 0x0D, 0x0A,'\0'};
			
char connect_tcp_ok[] ="CONNECT IP OK\r\n";
											
char check_connect[] = "AT+CIPOPQUERY=1\r\n";
char check_connect_ok[] = "CONNECT_OK\r\n";
									
int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();

	HAL_UART_Receive_IT(&huart3,&rx3,1);
	HAL_UART_Receive_IT(&huart1,&rx1,1);
	
	HAL_UART_Transmit_IT(&huart1,(uint8_t *)&tx1_buffer,strlen(tx1_buffer));
	HAL_UART_Transmit_IT(&huart3,(uint8_t *)&tx3_buffer,strlen(tx3_buffer));

	memcpy(connect_tcp+strlen(connect_tcp),connect_tcp1,strlen(connect_tcp1));
	memcpy(connect_tcp+strlen(connect_tcp),connect_tcp2,strlen(connect_tcp2));
	memcpy(connect_tcp+strlen(connect_tcp),connect_tcp3,strlen(connect_tcp3));
	memcpy(connect_tcp+strlen(connect_tcp),connect_tcp4,strlen(connect_tcp4));
	memcpy(connect_tcp+strlen(connect_tcp),connect_tcp5,strlen(connect_tcp5));

	// Turn on modun sim
	HAL_GPIO_WritePin(OF_SIM_GPIO_Port,OF_SIM_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(RESET_SIM_GPIO_Port,RESET_SIM_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PWRKEY_GPIO_Port,PWRKEY_Pin,GPIO_PIN_SET);
	
	// led bao ok
	HAL_GPIO_WritePin(LED_DO_GPIO_Port,LED_DO_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_XANH_GPIO_Port,LED_XANH_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_DO_GPIO_Port,LED_DO_Pin,GPIO_PIN_SET);
	HAL_Delay(15000);
	HAL_GPIO_WritePin(LED_XANH_GPIO_Port,LED_XANH_Pin,GPIO_PIN_SET);
	HAL_Delay(2000);
	
  while (1)
  {
		tx_data();
		kiem_tra_sim();
		//HAL_UART_Transmit(&huart1,(uint8_t *)&kiem_tra_hoan_tat,strlen(kiem_tra_hoan_tat),500);
		//HAL_Delay(1000);
  }
}

int bien=0;
unsigned int td=0;
char rx3_ao[10];

// Transmit "KT\r\n" to check config sim
void kiem_tra_sim(void){ 
	
	if(strcmp(rx1_buffer,bien_kiem_tra) == 0){
		bien=1;
		//HAL_UART_Transmit(&huart3,(uint8_t *)&check_module,strlen(check_module),500);
		//HAL_Delay(1000);
	}
	switch(bien){
		
		case 1:
			memset(rx1_buffer,0x00,100);
			HAL_UART_Transmit(&huart3,(uint8_t *)&check_module,strlen(check_module),500);
			HAL_Delay(500);
			
			if(rx3_buffer[strlen(rx3_buffer)-4] == 79 && rx3_buffer[strlen(rx3_buffer)-3] == 75){
				HAL_UART_Transmit(&huart1,(uint8_t *)&rx3_buffer,strlen(rx3_buffer),500);
				HAL_Delay(200);
				HAL_UART_Transmit(&huart1,(uint8_t *)&kt_modun_ok,strlen(kt_modun_ok),500);
				memset(rx3_buffer,0x00,100);
				bien=2;
				}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)&check_module,strlen(check_module),500);
			}
			break;
		case 2:
			HAL_UART_Transmit(&huart3,(uint8_t *)&check_sim,strlen(check_sim),500);
			HAL_Delay(500);
			if(rx3_buffer[strlen(rx3_buffer)-4] == 79 && rx3_buffer[strlen(rx3_buffer)-3] == 75){
				HAL_UART_Transmit(&huart1,(uint8_t *)&rx3_buffer,strlen(rx3_buffer),500);
				HAL_Delay(200);
				HAL_UART_Transmit(&huart1,(uint8_t *)&kt_sim_ok,strlen(kt_sim_ok),500);
				memset(rx3_buffer,0x00,100);
				bien=3;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)&check_sim,strlen(check_sim),500);
			}
			break;
		case 3:
			HAL_UART_Transmit(&huart3,(uint8_t *)&check_cuong_do,strlen(check_cuong_do),500);
			HAL_Delay(500);
			if(rx3_buffer[strlen(rx3_buffer)-4] == 79 && rx3_buffer[strlen(rx3_buffer)-3] == 75){
				HAL_UART_Transmit(&huart1,(uint8_t *)&rx3_buffer,strlen(rx3_buffer),500);
				HAL_Delay(200);
				HAL_UART_Transmit(&huart1,(uint8_t *)&kt_cuong_do_ok,strlen(kt_cuong_do_ok),500);
				memset(rx3_buffer,0x00,100);
				bien=4;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)&check_cuong_do,strlen(check_cuong_do),500);
			}
			break;
		case 4:
			HAL_UART_Transmit(&huart3,(uint8_t *)&check_dang_ky,strlen(check_dang_ky),500);
			HAL_Delay(500);
			if(rx3_buffer[strlen(rx3_buffer)-4] == 79 && rx3_buffer[strlen(rx3_buffer)-3] == 75){
				HAL_UART_Transmit(&huart1,(uint8_t *)&rx3_buffer,strlen(rx3_buffer),500);
				HAL_Delay(200);
				HAL_UART_Transmit(&huart1,(uint8_t *)&kt_dky_ok,strlen(kt_dky_ok),500);
				memset(rx3_buffer,0x00,100);
				bien=5;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)&check_dang_ky,strlen(check_dang_ky),500);
			}
			break;
		case 5:
			HAL_UART_Transmit(&huart3,(uint8_t *)&check_GPRS,strlen(check_GPRS),500);
			HAL_Delay(500);
			if(rx3_buffer[strlen(rx3_buffer)-4] == 79 && rx3_buffer[strlen(rx3_buffer)-3] == 75){
				HAL_UART_Transmit(&huart1,(uint8_t *)&rx3_buffer,strlen(rx3_buffer),500);
				HAL_Delay(200);
				HAL_UART_Transmit(&huart1,(uint8_t *)&kt_gprs_ok,strlen(kt_gprs_ok),500);
				memset(rx3_buffer,0x00,100);
				bien=6;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)&check_GPRS,strlen(check_GPRS),500);
			}
			break;
		case 6:
			HAL_UART_Transmit(&huart3,(uint8_t *)&set_timeout,strlen(set_timeout),500);
			HAL_Delay(500);
			if(rx3_buffer[strlen(rx3_buffer)-4] == 79 && rx3_buffer[strlen(rx3_buffer)-3] == 75){
				HAL_UART_Transmit(&huart1,(uint8_t *)&rx3_buffer,strlen(rx3_buffer),500);
				HAL_Delay(200);
				HAL_UART_Transmit(&huart1,(uint8_t *)&set_timeout_ok,strlen(set_timeout_ok),500);
				memset(rx3_buffer,0x00,100);
				bien=7;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)&set_timeout,strlen(set_timeout),500);
			}
			break;
		case 7:
			HAL_UART_Transmit(&huart3,(uint8_t *)&set_transmit,strlen(set_transmit),500);
			HAL_Delay(500);
			if(rx3_buffer[strlen(rx3_buffer)-4] == 79 && rx3_buffer[strlen(rx3_buffer)-3] == 75){
				HAL_UART_Transmit(&huart1,(uint8_t *)&rx3_buffer,strlen(rx3_buffer),500);
				HAL_Delay(200);
				HAL_UART_Transmit(&huart1,(uint8_t *)&set_transmit_ok,strlen(set_transmit_ok),500);
				memset(rx3_buffer,0x00,100);
				bien=8;
				}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)&set_transmit,strlen(set_transmit),500);
			}
			break;
		case 8:
			HAL_UART_Transmit(&huart3,(uint8_t *)&check_net,strlen(check_net),500);
			HAL_Delay(500);
			if(rx3_buffer[strlen(rx3_buffer)-3] == 83 || rx3_buffer[strlen(rx3_buffer)-3] == 75){
				HAL_UART_Transmit(&huart1,(uint8_t *)&rx3_buffer,strlen(rx3_buffer),500);
				HAL_Delay(200);
				HAL_UART_Transmit(&huart1,(uint8_t *)&kt_net_ok,strlen(kt_net_ok),500);
				memset(rx3_buffer,0x00,100);
				bien=9;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)&check_net,strlen(check_net),500);
			}
			break;
		case 9:
			HAL_UART_Transmit(&huart3,(uint8_t *)&check_ip,strlen(check_ip),500);
			HAL_Delay(500);
			if(rx3_buffer[strlen(rx3_buffer)-4] == 79 && rx3_buffer[strlen(rx3_buffer)-3] == 75){
				HAL_UART_Transmit(&huart1,(uint8_t *)&rx3_buffer,strlen(rx3_buffer),500);
				HAL_Delay(200);
				HAL_UART_Transmit(&huart1,(uint8_t *)&kt_ip_ok,strlen(kt_ip_ok),500);
				memset(rx3_buffer,0x00,100);
				bien=10;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)&check_ip,strlen(check_ip),500);
			}
			break;
		case 10:
			HAL_UART_Transmit(&huart3,(uint8_t *)&set_receive,strlen(set_receive),500);
			HAL_Delay(500);
			if(rx3_buffer[strlen(rx3_buffer)-4] == 79 && rx3_buffer[strlen(rx3_buffer)-3] == 75){
				HAL_UART_Transmit(&huart1,(uint8_t *)&rx3_buffer,strlen(rx3_buffer),500);
				HAL_Delay(200);
				HAL_UART_Transmit(&huart1,(uint8_t *)&set_receive_ok,strlen(set_receive_ok),500);
				memset(rx3_buffer,0x00,100);
				bien=11;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)&set_receive,strlen(set_receive),500);
			}
			break;
		case 11:
			HAL_UART_Transmit(&huart3,(uint8_t *)&connect_tcp,strlen(connect_tcp),500);
			HAL_Delay(500);
			if(rx3_buffer[strlen(rx3_buffer)-4] == 79 && rx3_buffer[strlen(rx3_buffer)-3] == 75){
				HAL_UART_Transmit(&huart1,(uint8_t *)&rx3_buffer,strlen(rx3_buffer),500);
				HAL_Delay(200);
				HAL_UART_Transmit(&huart1,(uint8_t *)&connect_tcp_ok,strlen(connect_tcp_ok),500);
				memset(rx3_buffer,0x00,100);
				bien=12;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)&connect_tcp,strlen(connect_tcp),500);
			}
			break;
		case 12:
			HAL_UART_Transmit(&huart3,(uint8_t *)&allow_transmit,strlen(allow_transmit),500);
			HAL_Delay(500);
			if(rx3_buffer[strlen(rx3_buffer)-4] == 79 && rx3_buffer[strlen(rx3_buffer)-3] == 75){
				HAL_UART_Transmit(&huart1,(uint8_t *)&rx3_buffer,strlen(rx3_buffer),500);
				HAL_Delay(200);
				HAL_UART_Transmit(&huart1,(uint8_t *)&allow_transmit_ok,strlen(allow_transmit_ok),500);
				memset(rx3_buffer,0x00,100);
				bien=13;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)&connect_tcp,strlen(connect_tcp),500);
			}
			break;
		case 13:
			HAL_UART_Transmit(&huart3,(uint8_t *)&allow_receive,strlen(allow_receive),500);
			HAL_Delay(500);
			if(rx3_buffer[strlen(rx3_buffer)-4] == 79 && rx3_buffer[strlen(rx3_buffer)-3] == 75){
				HAL_UART_Transmit(&huart1,(uint8_t *)&rx3_buffer,strlen(rx3_buffer),500);
				HAL_Delay(200);
				HAL_UART_Transmit(&huart1,(uint8_t *)&allow_receive_ok,strlen(allow_receive_ok),500);
				memset(rx3_buffer,0x00,100);
				bien=14;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)&connect_tcp,strlen(connect_tcp),500);
			}
			break;
		case 14:
			HAL_UART_Transmit(&huart3,(uint8_t *)&check_connect,strlen(check_connect),500);
			HAL_Delay(500);
			if(rx3_buffer[strlen(rx3_buffer)-4] == 79 && rx3_buffer[strlen(rx3_buffer)-3] == 75){
				HAL_UART_Transmit(&huart1,(uint8_t *)&rx3_buffer,strlen(rx3_buffer),500);
				HAL_Delay(200);
				HAL_UART_Transmit(&huart1,(uint8_t *)&check_connect_ok,strlen(check_connect_ok),500);
				HAL_UART_Transmit(&huart1,(uint8_t *)&kt_ok,strlen(kt_ok),500);
				memset(rx3_buffer,0x00,100);
				bien=0;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)&check_connect,strlen(check_connect),500);
			}
			break;
	}
}

// to send data to server, transmit "\<data>",with <data> - is data want to send
void send_data_server()
{
	if(rx1_buffer[0]==47){
		memcpy(send_data_sim,send_data_sim_dem1,100);
		for(int i=0;i<99;i++){
			send_data_sim_dem2[i] = rx1_buffer[i+1];
		}
		memcpy(send_data_sim + strlen(send_data_sim),send_data_sim_dem2,100);
		memcpy(send_data_tx1 + strlen(send_data_tx1),send_data_sim_dem2,100);
		HAL_UART_Transmit(&huart3,(uint8_t *)&send_data_sim,strlen(send_data_sim),500);
		HAL_UART_Transmit(&huart1,(uint8_t *)&send_data_tx1,strlen(send_data_tx1),500);
		memset(send_data_sim,0x00,100);
		memset(rx1_buffer,0x00,100);
	}
}

// transmit data from data received rx
void tx_data(void)
{
	if(flag3==1){
		memcpy(tx1_buffer,rx3_buffer,100);
		HAL_UART_Transmit(&huart1,(uint8_t *)&tx1_buffer,strlen(tx1_buffer),500);
		flag3=0;
	}
	if(flag1==1){
		send_data_server();
		memcpy(tx3_buffer,rx1_buffer,100);
		HAL_UART_Transmit(&huart3,(uint8_t *)&tx3_buffer,strlen(tx3_buffer),500);
		flag1=0;
	}
}

// receive interrupt
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	// nhan du lieu uart 1
	
	if(huart->Instance == huart3.Instance){
		HAL_UART_Receive_IT(&huart3,&rx3,1);
		rx3_buffer_dem[cnt3]=rx3;
		cnt3++;
	}
	if(//rx3_buffer_dem[strlen(rx3_buffer_dem)-4]==79 && rx3_buffer_dem[strlen(rx3_buffer_dem)-3]==75 &&
		rx3_buffer_dem[strlen(rx3_buffer_dem)-2]==13 && rx3_buffer_dem[strlen(rx3_buffer_dem)-1]==10){
		memcpy(rx3_buffer,rx3_buffer_dem,100);
		memset(rx3_buffer_dem,0x00,100);
		cnt3=0;
		flag3=1;
	}
	
	if(huart->Instance == huart1.Instance){
		HAL_UART_Receive_IT(&huart1,&rx1,1);
		rx1_buffer_dem[cnt1]=rx1;
		cnt1++;
	}
	if(rx1_buffer_dem[strlen(rx1_buffer_dem)-2]==13 && rx1_buffer_dem[strlen(rx1_buffer_dem)-1]==10){
		memcpy(rx1_buffer,rx1_buffer_dem,100);
		memset(rx1_buffer_dem,0x00,100);
		cnt1=0;
		flag1=1;
	}	
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

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
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC1 PC3 PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB13 PB14
                           PB3 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB5 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
