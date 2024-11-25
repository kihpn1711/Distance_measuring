#include <avr/io.h>
#include "delay.h"
#include <util/delay.h>
#include "LCD_4bits.h"
#include "DHT11.h"

void INIT()
{
	//Khoi tao trang thai output cho cac chan noi toi cac LED don
	DDRD |= 0xFF;
	//Khoi tao trang thai logic 1 cho cac chan noi toi cac LED don
	PORTD |= 0xFF;
	//Khoi tao trang thai output cho cac chan noi toi cac LED 7 thanh
	DDRC |= 0xFF;
	//Khoi tao trang thai logic 1 cho cac chan noi toi cac LED 7 thanh
	PORTC |= 0xFF;
}

void PORT()
{
	unsigned char led_shift = 255; // Kh?i t?o giá tr? ban ??u cho LED ??n (t?t t?t c? LED)
	unsigned char led_7_count = 0; // Kh?i t?o giá tr? ban ??u cho LED 7 thanh (s? 0)

	for (;;)
	{
		// LED ??n
		PORTD = led_shift; // B?t LED t?i v? trí hi?n t?i
		_delay_ms(500);    // Ch? 0.5 giây
		led_shift >>= 1;   // Di chuy?n LED sang ph?i
		if (led_shift == 0)
		led_shift = 255; // Reset giá tr? LED ??n khi ?ã sang h?t

		// LED 7 thanh
		LED7_OUT(led_7_count);     // Hi?n th? s? ??m trên LED 7 thanh
		PORTC ^= (1 << PC3);       // ??o tr?ng thái d?u ch?m
		_delay_ms(500);            // Ch? 0.5 giây
		led_7_count = (led_7_count + 1) % 10; // T?ng giá tr? ??m (0-9)
	}
}

void LED7_OUT(unsigned char num)
{
	//Khai bao bien temp luu trang thai cua PORTC
	unsigned char temp = PORTC;
	//Cac chan vi dieu khien  ung voi cac thanh LED
	// a – PC5
	// b – PC4
	// c – PC2
	// d – PC1
	// e – PC0
	// f – PC6
	// g – PC7
	//dot - PC3
	//Tat cac doan LED hien dang sang truoc khi sang cac doan LED moi
	temp &= 0B00001000;
	//Gan muc logic cho 8 bit cua bien temp ung voi gia tri cua bien num
	switch(num)
	{
		case 0: temp |= 0B10000000; break;
		case 1: temp |= 0B11100011; break;
		case 2: temp |= 0B01000100; break;
		case 3: temp |= 0B01000001; break;
		case 4: temp |= 0B00100011; break;
		case 5: temp |= 0B00010001; break;
		case 6: temp |= 0B00010000; break;
		case 7: temp |= 0B11000011; break;
		case 8: temp |= 0B00000000; break;
		case 9: temp |= 0B00000001; break;
	}
	//Xuat gia tri logic moi ra PORTC de lam sang LED 7 thanh
	PORTC = temp;
}

void DELAY_MS(unsigned int mili_count)
{
	//Khai bao hai bien chay cho 2 vong for
	unsigned int i, j;
	//Xung nhip cua he thong cang cao, so vong lap cang tang
	mili_count = mili_count * FRE;
	//Cac vong for gay tre
	for(i = 0; i < mili_count; ++i)
	for(j = 0; j < 53; ++j);
}

