#ifndef __BATTERY_H_
#define __BATTERY_H_

#include "sys.h"
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"

//Modify the following definitions to facilitate the modification of the pins
#define INT_Pin GPIO_Pin_7
#define POL_Pin GPIO_Pin_6
#define SHDN_Pin GPIO_Pin_5

#define INT_in	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
#define POL PBin(6)		// == #define POL GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
#define SHDN PBout(5)

void battery_init(void);
void battery_stop(void);
void battery_res(void);
void battery_start(void);
void myISR(void);

#endif

