/*
 * DHT11.h
 *
 * Created: 6/2/2023 10:02:10 PM
 *  Author: nhatle
 */ 


#ifndef DHT11_H_
#define DHT11_H_
#define FRE 8

void INIT();
void PORT();
void LED7_OUT(unsigned char num);
void DELAY_MS(unsigned int mili_count);

#endif /* DHT11_H_ */