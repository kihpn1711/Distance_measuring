#include <avr/io.h>
#include "util/delay.h"
#include "LCD_4bits.h"
#include <stdio.h>
#include <stdlib.h>
#define FRE	16

//define US
#define US_PORT PORTA
#define	US_PIN	PINA
#define US_DDR 	DDRA

#define US_TRIG_POS	PA1
#define US_ECHO_POS	PA2

#define US_ERROR		-1
#define	US_NO_OBSTACLE	-2
#define BUZZER_PORT PORTA  // ??t chân PA0 là chân ?i?u khi?n buzzer
#define BUZZER_PIN PA3
void BuzzerInit() {
	DDRA |= (1 << BUZZER_PIN);  // ??t chân PA0 là chân ra
}

void BuzzerOn() {
	BUZZER_PORT |= (1 << BUZZER_PIN);  // B?t buzzer
}

void BuzzerOff() {
	BUZZER_PORT &= ~(1 << BUZZER_PIN);  // T?t buzzer
}
int distance, previous_distance;

void HCSR04Init();
void HCSR04Trigger();

void lcd_command( unsigned char );

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

int main(void)
{
	DDRD |= 0xFF;
	PORTD |= 0xAA;
	DDRC |= 0xFF;
	PORTC |= 0x00;
	
	BuzzerInit();  // Kh?i t?o buzzer
	LCD_Init();
	char numberString[4];
	while (1)
	{
		uint16_t r;
		_delay_ms(100);
		HCSR04Init();
		while (1)
		{
			//Send a trigger pulse
			HCSR04Trigger();

			//Measure the width of pulse
			r=GetPulseWidth();

			//Handle Errors
			if(r==US_ERROR)
			{
				LCD_Gotoxy(1, 1);
				LCD_Puts("Error!");
				BuzzerOff();
			}
			else
			{
				distance=(r*0.034/2);
				
				if (distance != previous_distance)    // the LCD screen only need to be cleared if the distance is changed otherwise it is not required
				{
					LCD_Clear();	
				}
				if (distance < 10) {
					// Khi kho?ng cách d??i 20cm, phát ti?ng kêu
					BuzzerOn();
					} else {
					BuzzerOff();
				}
			
				if (distance <= 20) {
					LCD_Gotoxy(0, 0);
					LCD_Puts(" WARNING!    "); // Hi?n th? thông báo nguy hi?m
				}
				else {
					LCD_Gotoxy(0, 0);
					LCD_Puts(" MSSV: 20207577 "); // Xóa thông báo n?u kho?ng cách an toàn
				}
			
				LCD_Gotoxy(1, 1);      // set the row and column to display the data
				LCD_Puts("Distance = ");
				LCD_Gotoxy(12, 1);      //lcd_setCursor(column, row)
				itoa(distance, numberString, 10);    // distance is an integer number, we can not display integer directly on the LCD. this line converts integer into array of character
				LCD_Puts(numberString);
				LCD_Gotoxy(14, 1);      //set the row to 1 and and column to 14 to display the data
				LCD_Puts("cm");
				previous_distance = distance;
				_delay_ms(30);
			}
	
		}
	}
}



