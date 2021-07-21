#include "battery.h"


//��������������·��ƥ�����ĵ��
//�Լ����ĳ�ʼ���״̬
volatile double battery_mAh = 2000.00; // ��ص��ܺ�����������>��Ҫ����ʵ���������
volatile double battery_percent = 100.00;  // ���״̬���ٷֱȣ�����λ��%����������>��Ҫ����ʵ�������

//ȫ�ֱ�������volatile����ʾ�жϿ�����Ļ��������ǣ���
volatile int isrflag;		//ֻ����1����0�������ж��жϺ����Ƿ񴥷�
volatile long int time, lasttime;
volatile double mA;
double ah_quanta = 0.17067759; // ÿINT ����ʱ
double percent_quanta; //���·����㣬��ʾÿINT���ٻ����ӵ�percent

/*******************************************************************************
*������ԭ�ͣ�void battery_init(void)
*�����Ĺ��ܣ�LTC4150��ʼ�����ţ�˳�����persent_quanta
*�����Ĳ�����None
*��������ֵ��None
*������˵����
*������д��: ����
*������д���ڣ�2021/7/21
*�����İ汾�ţ�V1.0
********************************************************************************/

void battery_init(void)  //��ʼ������
{
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );

    GPIO_InitStructure.GPIO_Pin = SHDN_Pin;//SHDN����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = INT_Pin;//INT����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //��������
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	
	
	GPIO_InitStructure.GPIO_Pin = POL_Pin;//POL����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   //��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	percent_quanta = 1.0 / (battery_mAh / 1000.0 * 5859.0 / 100.0);
	isrflag =0;
	
	SHDN = 1;  //����SHDN�ŵ�ƽ��LTC4150Ĭ��Ϊ����״̬
}


/*******************************************************************************
*������ԭ�ͣ�int battery_stop(void)
*�����Ĺ��ܣ�ʹLTC4150����ض�״̬
*�����Ĳ�����None
*��������ֵ��None
*������˵����
*������д��: ����
*������д���ڣ�2021/7/21
*�����İ汾�ţ�V1.0
********************************************************************************/
void battery_stop(void)
{
	SHDN = 0;
}


/*******************************************************************************
*������ԭ�ͣ�int battery_res(void)
*�����Ĺ��ܣ�ʹLTC4150����
*�����Ĳ�����None
*��������ֵ��None
*������˵����
*������д��: ����
*������д���ڣ�2021/7/21
*�����İ汾�ţ�V1.0
********************************************************************************/
void battery_res(void)
{
	SHDN = 0;
	delay_ms(3000);
	SHDN = 1;
}

/*******************************************************************************
*������ԭ�ͣ�void myISR(void)
*�����Ĺ��ܣ����ڱ��жϴ���
*�����Ĳ�����None
*��������ֵ��None
*������˵����
*������д��: ����
*������д���ڣ�2021/7/21
*�����İ汾�ţ�V1.0
********************************************************************************/
void myISR(void)// ��INTΪ����ʱ�����жϴ������������ݣ�
{
	static int polarity;//ֻ����1����0

	// ȷ�����ϴ��жϺ���ӳ٣�����mA���㣩
	// ��ע�⣬��һ�β������ǲ���ȷ�ģ�û����ǰ��ʱ�䣡��
	lasttime = time;
	time = (millis() * 1000);

	// ��ȡ����ֵ�������жϵ�������
	polarity = POL;
	
	if (polarity) // POL�����ƽ�� = ��磨��������IN��>OUT��
	{
		battery_mAh += ah_quanta;
		battery_percent += percent_quanta;
	}
	else // POL�����ƽ�� = ����磨��������OUT��>IN��
	{
		battery_mAh -= ah_quanta;
		battery_percent -= percent_quanta;
	}
	//���¼����������ȥ��
	//����ʱ����mA����ѡ��

	mA = 614.4 / ((time-lasttime) / 1000000.0);

	// �����磬���ǻὫmA��Ϊ������ѡ��

	if (polarity) mA = mA * -1.0;

	// ����isrflag�Ա���ѭ��֪���������ж�

	isrflag = 1;
}



/*******************************************************************************
*������ԭ�ͣ�void battery_start(void)
*�����Ĺ��ܣ�����LTC4150�Ĺ���
*�����Ĳ�����None
*��������ֵ��None
*������˵����
*������д��: ����
*������д���ڣ�2021/7/21
*�����İ汾�ţ�V1.0
********************************************************************************/

void battery_start(void)
{
	// �����Ǽ�⵽һ��INT�ź�ʱ��myISR�����������Զ����С�myISR������isrflag����Ϊ1
	// �������Ǿ�֪��������ʲô�¡�
	char data1[8];
	char data2[6];
	char data3[6];
	char data4[7];	
	
	if (isrflag)  //�ж�INT���Ƿ񴥷��ж�
	{
	// ����־����Ϊ 0 ������ÿ��INTִֻ��һ��
	isrflag = 0;

	// ���ڴ�ӡ��ǰ״̬��myISR��������ı�����
	printf("mAh: ");
	printf("%f", battery_mAh);
	printf(" soc: ");
	printf("%f", battery_percent);
	printf("%% time: ");
	printf("%f", (time-lasttime)/1000000.0);
	printf("s mA: ");
	printf("%f\n\n", mA);
		
	//OLED��Ļ��ʾ��ǰ����
	sprintf(data1, "%.2f" ,battery_mAh);     //���Ŀǰ����
	sprintf(data2, "%.2f" ,battery_percent);	//���ʣ��ٷֱ�
	sprintf(data3, "%.2f" ,((time-lasttime)/1000000.0));	//һINT����ʱ��
	sprintf(data4, "%.2f" ,mA);		//������С
	Oled_Display_String(0, 48, data1);
	Oled_Display_String(2, 48, data2);
	Oled_Display_String(4, 56, data3);
	Oled_Display_String(6, 40, data4);
	}
	// ����������ѭ�������������Լ��Ĵ���
	// myISR�������Զ�������Ϣ
	// �����Ҫ�Ļ�������isrflag����֪����Щ�����Ѿ��ı��ˡ�

}

