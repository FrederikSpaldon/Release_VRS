/*
 * vrs_cv5.h
 *
 *  Created on: 20. 10. 2016
 *      Author: Fredo
 */

#ifndef VRS_CV5_H_
#define VRS_CV5_H_

#include <stddef.h>
#include "stm32l1xx.h"

void adc_init(void);
void init_LED(void);
void init_NVIC(void);
void ADC1_IRQHandler(void);
void blikaj(int pocet);
void casovac(uint16_t AD_value);

uint16_t AD_value;

#endif /* VRS_CV5_H_ */
