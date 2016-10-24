/*
 * vrs_cv5.c
 *
 *  Created on: 20. 10. 2016
 *      Author: Fredo
 */

#include "vrs_cv5.h"

void adc_init(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 ADC_InitTypeDef ADC_InitStructure;
 /* Enable GPIO clock */
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

void init_NVIC(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn; //zoznam prerušení nájdete v súbore stm32l1xx.h
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	//ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC || ADC_FLAG_OVR);
}

void ADC1_IRQHandler(void){
	//while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC | ADC_FLAG_OVR)!=RESET){}

		//GPIO_SetBits(GPIOA, GPIO_Pin_5);

		if(ADC1->SR && ADC_SR_EOC){
			AD_value = ADC1->DR;
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
