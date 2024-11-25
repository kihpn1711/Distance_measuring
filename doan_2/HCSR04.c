/*
 * HCSR04.c
 *
 * Created: 9/12/2023 10:23:11 PM
 *  Author: HQLap
 */ 
#include "HCSR04.h"
#define FRE	16
#include <stdio.h>
#include <stdlib.h>
#include "util/delay.h"

#define US_PORT PORTA           // chúng ta ?ã k?t n?i c?m bi?n siêu âm trên c?ng C. ?? s? d?ng siêu âm chúng ta c?n hai chân c?a siêu âm ???c k?t n?i trên c?ng C
#define	US_PIN	PINA            // chúng ta c?n kh?i t?o ?i?n tr? pin khi mu?n l?y ??u vào.
#define US_DDR 	DDRA            // chúng ta c?n ?i?n tr? h??ng d? li?u (DDR) ?? ??t h??ng c?a lu?ng d? li?u. ??u vào ho?c ??u ra. chúng ta s? ??nh ngh?a nó sau, bây gi? chúng ta ch? ??t tên cho nó.

#define US_TRIG_POS	PA1         // chân trigger c?a ultrasonic sound k?t n?i v?i port A pin 1
#define US_ECHO_POS	PA2         // chân echo c?a ultrasonic sound k?t n?i v?i port A pin 2

#define US_ERROR		-1      // we're defining two more variables two know if the ultrasonic sensor is working or not
#define	US_NO_OBSTACLE	-2


void HCSR04Init()
{
	
	// we're setting the trigger pin as output as it will generate ultrasonic sound wave
	US_DDR|=(1<<US_TRIG_POS);
}

void HCSR04Trigger()
{   // this function will generate ultrasonic sound wave for 15 microseconds
	//Send a 10uS pulse on trigger line
	
	US_PORT|=(1<<US_TRIG_POS);	//high
	
	_delay_us(15);				//wait 15uS
	
	US_PORT&=~(1<<US_TRIG_POS);	//low
}

uint16_t GetPulseWidth()
{
	// this function will be used to measure the pulse duration. When the ultra sound echo back after hitting an object
	// the microcontroller will read the pulse using the echo pin of the ultrasonic sensor connected to it.
	
	uint32_t i,result;

	// Section - 1: the following lines of code before the section - 2 is checking if the ultrasonic is working or not
	// it check the echo pin for a certain amount of time. If there is no signal it means the sensor is not working or not connect properly
	for(i=0;i<600000;i++)
	{
		if(!(US_PIN & (1<<US_ECHO_POS)))
		continue;	//Line is still low, so wait
		else
		break;		//High edge detected, so break.
	}

	if(i==600000)
	return US_ERROR;	//Indicates time out
	
	//High Edge Found
	
	// Section -2 : This section is all about preparing the timer for counting the time of the pulse. Timers in microcontrllers is used for timimg operation
	//Setup Timer1
	TCCR1A=0X00;
	TCCR1B=(1<<CS11);	// This line sets the resolution of the timer. Maximum of how much value it should count.
	TCNT1=0x00;			// This line start the counter to start counting time

	// Section -3 : This section checks weather the there is any object or not
	for(i=0;i<600000;i++)                // the 600000 value is used randomly to denote a very small amount of time, almost 40 miliseconds
	{
		if(US_PIN & (1<<US_ECHO_POS))
		{
			if(TCNT1 > 60000) break; else continue;   // if the TCNT1 value gets higher than 60000 it means there is not object in the range of the sensor
		}
		else
		break;
	}

	if(i==600000)
	return US_NO_OBSTACLE;	//Indicates time out

	//Falling edge found

	result=TCNT1;          // microcontroller stores the the value of the counted pulse time in the TCNT1 register. So, we're returning this value to the
	// main function for utilizing it later

	//Stop Timer
	TCCR1B=0x00;

	if(result > 60000)
	return US_NO_OBSTACLE;	//No obstacle
	else
	return (result>>1);
}