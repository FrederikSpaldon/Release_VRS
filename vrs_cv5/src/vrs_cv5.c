/*
 * vrs_cv5.c
 *
 *  Created on: 23. 10. 2016
 *      Author: Fredo
 */

#include "vrs_cv5.h"

void adc_init(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 ADC_InitTypeDef ADC_InitStructure;

 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
 /* Configure ADCx Channel 2 as analog input */
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
 GPIO_Init(GPIOC, &GPIO_InitStructure);
/* Enable the HSI oscillator */
 RCC_HSICmd(ENABLE);
/* Check that HSI oscillator is ready */
 while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
 /* Enable ADC clock */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
 /* Initialize ADC structure */
 ADC_StructInit(&ADC_InitStructure);
 /* ADC1 configuration */
 ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
 ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
 ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
 ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
 ADC_InitStructure.ADC_NbrOfConversion = 1;

 ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
 ADC_ITConfig(ADC1, ADC_IT_OVR, ENABLE);

 ADC_Init(ADC1, &ADC_InitStructure);
/* ADCx regular channel8 configuration */
 ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_16Cycles);
 /* Enable the ADC */
 ADC_Cmd(ADC1, ENABLE);
 /* Wait until the ADC1 is ready */
 while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
 {
 }
 /* Start ADC Software Conversion */
 ADC_SoftwareStartConv(ADC1);
}

//init LEDky
void init_LED(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	GPIO_InitTypeDef gpioInitStruc;
	gpioInitStruc.GPIO_Mode = GPIO_Mode_OUT;
	gpioInitStruc.GPIO_OType = GPIO_OType_PP;
	gpioInitStruc.GPIO_Pin = GPIO_Pin_5;
	gpioInitStruc.GPIO_Speed=GPIO_Speed_400KHz;
	GPIO_Init(GPIOA,&gpioInitStruc);
}

//init preruseni z prevodnika
void init_NVIC(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; //priorita nizsia nez pre USART
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}

// init GPIO pre RX a TX, nastavenie USART, nastavenie preruseni
void init_USART(){
	GPIO_InitTypeDef GPIO_usrt;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	//GPIO pre RX a TX
	GPIO_usrt.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_usrt.GPIO_Mode = GPIO_Mode_AF;
	GPIO_usrt.GPIO_OType = GPIO_OType_PP;
	GPIO_usrt.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_usrt.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(GPIOA,&GPIO_usrt);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	//nastavenie USART
	USART_InitStructure.USART_BaudRate = 9600*2;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	//nastavenie preruseni pre USART
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);

}

//handler pre prerusenia prevodnika
void ADC1_IRQHandler(void){
	//while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC | ADC_FLAG_OVR)!=RESET){}
	if(ADC1->SR && ADC_SR_EOC){
		AD_value = ADC1->DR;
	}

}

//handler pre prerusenia USART
void USART2_IRQHandler(void){
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		In = USART_ReceiveData(USART2);
    }
}

//delay funkcia
void blikaj(int pocet){
	int i=0;
	for(i;i<pocet;i++){

	}
}

//na zaklade AN hodnoty urcuje frekvenciu blikania
void casovac(uint16_t AD_value){
	if(AD_value<2200){
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		blikaj(50000);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		blikaj(50000);
	}
	else if(AD_value<3200){
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			blikaj(150000);
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			blikaj(150000);
	}
	else if(AD_value<3600){
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			blikaj(300000);
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			blikaj(300000);
	}
	else if(AD_value<3800){
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			blikaj(500000);
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			blikaj(500000);
		}
	else{ //svieti ak je pusteny button
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
	}
}

//posielanie stringu po znakoch
void posli(char *res){
	int index =0;
	while(res[index] != '\0')
	{
		USART_SendData(USART2, res[index]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		index++;
	}
	USART_SendData(USART2,' ');
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}

//prevod float do stringu
void format_3V(float number, char *res)
{
	int full=(int)number;
	int decimal= (int) ((number-full)*1000);
	sprintf(res,"%d.%dV",full,decimal);
}

