#include "stm32f10x.h"
#include "usart.h"
#include "oled.h"
#include "word.h"
#include "delay.h"
#include "battery.h"
#include "IIC.h"
#include "exti.h"

int main()
{
	uart_init(115200);
	delay_init();
	MILLIS_Init();
	IIC_Init();
	Oled_Init();
	EXTI7_Init();
	//OLEDœ‘ æ
	Oled_Display_String(0, 0, "mAh: ");
	Oled_Display_String(2, 0, "soc: ");
	Oled_Display_String(4, 0, "time: ");
	Oled_Display_String(6, 0, "mA: ");
	Oled_Display_String(2, 88, "%");
	Oled_Display_String(4, 88, "s");
	
	while(1)
	{
		battery_start();
	}	
}

