
/*
 * LED_VIP.h
 *
 * Created: 6/2/2023 10:02:10 PM
 *  Author: nhatle
 */ 
#ifndef LED_VIP_H_
#define LED_VIP_H_
void INIT();

void PORT();

void LED7_OUT(unsigned char num);

void DELAY_MS(unsigned int mili_count);

void PB_2_LED();

unsigned char PB_CHECK();

#endif /* LED_VIP_H_ */