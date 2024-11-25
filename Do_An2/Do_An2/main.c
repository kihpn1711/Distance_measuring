#include <avr/io.h>
#include "delay.h"
#include "LCD_4bits.h"
#include <stdio.h>
#include "DHT11.h"

#define FRE	8 

float temp = 0;
float humi = 0;
int m, n;

int main(void)
{
	DDRD |= 0xFF;
	PORTD |= 0xAA;
	DDRC |= 0xFF;
	PORTC |= 0x00;
	

	LCD_Init();
	LCD_Gotoxy(0, 0);
	LCD_Puts("Nguyen Chi Phi ");
	LCD_Gotoxy(0, 1);
	LCD_Puts("MSSV - 20207577 ");
	delay_ms(3000);

// 	while (1)
// 	{
// 		DHT11_Read_Data(&humi, &temp);
// 
// 		m = humi * 10;
// 		n = temp * 10;
// 
// 		LCD_Gotoxy(0, 0);
// 		LCD_Puts("HUMI: ");
// 		LCD_PutChar(m / 100 + 48);
// 		LCD_PutChar((m % 100) / 10 + 48);
// 		LCD_PutChar('.');
// 		LCD_PutChar(m % 10 + 48);
// 		LCD_Puts(" %    ");
// 
// 		LCD_Gotoxy(0, 1);
// 		LCD_Puts("TEMP: ");
// 		LCD_PutChar(n / 100 + 48);
// 		LCD_PutChar((n % 100) / 10 + 48);
// 		LCD_PutChar('.');
// 		LCD_PutChar(n % 10 + 48);
// 		LCD_Puts(" *C   ");
// 		delay_ms(500);
// 	}


INIT();

PB_2_LED();

}
