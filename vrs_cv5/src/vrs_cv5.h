/*
 * vrs_cv5.h
 *
 *  Created on: 23. 10. 2016
 *      Author: Fredo
 */

#ifndef VRS_CV5_H_
#define VRS_CV5_H_

#include <stddef.h>
#include <math.h>
#include "stm32l1xx.h"

void adc_init(void);
void init_LED(void);
void init_NVIC(void);
void init_USART(void);
void ADC1_IRQHandler(void);
void USART2_IRQHandler(void);
void blikaj(int pocet);
void casovac(uint16_t AD_value);
void posli(char send[]);
void format_3V(float number, char *res);

uint16_t AD_value;
uint16_t In;
char format;
char res[10];

#endif /* VRS_CV5_H_ */
