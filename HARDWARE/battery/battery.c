#include "battery.h"


//更改以下两条线路以匹配您的电池
//以及它的初始电荷状态
volatile double battery_mAh = 2000.00; // 电池的总毫安————>需要根据实际情况调整
volatile double battery_percent = 100.00;  // 充电状态（百分比）（单位：%）————>需要根据实际情调整

//全局变量（“volatile”表示中断可以在幕后更改它们）：
volatile int isrflag;		//只能是1或者0，用来判断中断函数是否触发
volatile long int time, lasttime;
volatile double mA;
double ah_quanta = 0.17067759; // 每INT 毫安时
double percent_quanta; //与下方计算，表示每INT减少或增加的percent

/*******************************************************************************
*函数的原型：void battery_init(void)
*函数的功能：LTC4150初始化引脚，顺便计算persent_quanta
*函数的参数：None
*函数返回值：None
*函数的说明：
*函数编写者: 豪猪
*函数编写日期：2021/7/21
*函数的版本号：V1.0
********************************************************************************/

void battery_init(void)  //初始化引脚
{
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );

    GPIO_InitStructure.GPIO_Pin = SHDN_Pin;//SHDN引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = INT_Pin;//INT引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //上拉输入
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	
	
	GPIO_InitStructure.GPIO_Pin = POL_Pin;//POL引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   //上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	percent_quanta = 1.0 / (battery_mAh / 1000.0 * 5859.0 / 100.0);
	isrflag =0;
	
	SHDN = 1;  //拉高SHDN脚电平，LTC4150默认为工作状态
}


/*******************************************************************************
*函数的原型：int battery_stop(void)
*函数的功能：使LTC4150处与关断状态
*函数的参数：None
*函数返回值：None
*函数的说明：
*函数编写者: 豪猪
*函数编写日期：2021/7/21
*函数的版本号：V1.0
********************************************************************************/
void battery_stop(void)
{
	SHDN = 0;
}


/*******************************************************************************
*函数的原型：int battery_res(void)
*函数的功能：使LTC4150重置
*函数的参数：None
*函数返回值：None
*函数的说明：
*函数编写者: 豪猪
*函数编写日期：2021/7/21
*函数的版本号：V1.0
********************************************************************************/
void battery_res(void)
{
	SHDN = 0;
	delay_ms(3000);
	SHDN = 1;
}

/*******************************************************************************
*函数的原型：void myISR(void)
*函数的功能：用于被中断触发
*函数的参数：None
*函数返回值：None
*函数的说明：
*函数编写者: 豪猪
*函数编写日期：2021/7/21
*函数的版本号：V1.0
********************************************************************************/
void myISR(void)// 当INT为下沿时，被中断触发，计算数据，
{
	static int polarity;//只能是1或者0

	// 确定自上次中断后的延迟（用于mA计算）
	// 请注意，第一次测量将是不正确的（没有以前的时间！）
	lasttime = time;
	time = (millis() * 1000);

	// 获取极性值，用与判断电流方向
	polarity = POL;
	
	if (polarity) // POL输出电平高 = 充电（电流方向IN—>OUT）
	{
		battery_mAh += ah_quanta;
		battery_percent += percent_quanta;
	}
	else // POL输出电平低 = 不充电（电流方向OUT—>IN）
	{
		battery_mAh -= ah_quanta;
		battery_percent -= percent_quanta;
	}
	//以下计算电流可以去除
	//从延时计算mA（可选）

	mA = 614.4 / ((time-lasttime) / 1000000.0);

	// 如果充电，我们会将mA设为负（可选）

	if (polarity) mA = mA * -1.0;

	// 设置isrflag以便主循环知道发生了中断

	isrflag = 1;
}



/*******************************************************************************
*函数的原型：void battery_start(void)
*函数的功能：启动LTC4150的功能
*函数的参数：None
*函数返回值：None
*函数的说明：
*函数编写者: 豪猪
*函数编写日期：2021/7/21
*函数的版本号：V1.0
********************************************************************************/

void battery_start(void)
{
	// 当我们检测到一个INT信号时，myISR（）函数将自动运行。myISR（）将isrflag设置为1
	// 这样我们就知道发生了什么事。
	char data1[8];
	char data2[6];
	char data3[6];
	char data4[7];	
	
	if (isrflag)  //判断INT脚是否触发中断
	{
	// 将标志重置为 0 ，这样每个INT只执行一次
	isrflag = 0;

	// 串口打印当前状态（myISR（）计算的变量）
	printf("mAh: ");
	printf("%f", battery_mAh);
	printf(" soc: ");
	printf("%f", battery_percent);
	printf("%% time: ");
	printf("%f", (time-lasttime)/1000000.0);
	printf("s mA: ");
	printf("%f\n\n", mA);
		
	//OLED屏幕显示当前数据
	sprintf(data1, "%.2f" ,battery_mAh);     //电池目前电量
	sprintf(data2, "%.2f" ,battery_percent);	//电池剩余百分比
	sprintf(data3, "%.2f" ,((time-lasttime)/1000000.0));	//一INT所需时间
	sprintf(data4, "%.2f" ,mA);		//电流大小
	Oled_Display_String(0, 48, data1);
	Oled_Display_String(2, 48, data2);
	Oled_Display_String(4, 56, data3);
	Oled_Display_String(6, 40, data4);
	}
	// 您可以在主循环（）中运行自己的代码
	// myISR（）将自动更新信息
	// 如果需要的话，设置isrflag让你知道有些事情已经改变了。

}

