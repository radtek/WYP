#include "HardwareProfile.h"
void InitMCU(void)             //ϵͳ��ʼ������
{	
	//AD��ʼ��	
	ADCON1=0b00111111;  //ȫ��Ϊ���ݿ�
	
	ANSELA=0;
	ANSELB=0;
	ANSELD=0;
	ANSELE=0;

	POWER_ON_W=0;
	POWER_ON_IO=0;	
	STATUS_IO=1;

	nWPUEN=0;
	TestMode_DIR=1;
	TestMode_WPUB=1;

	LED1_W=0;
	LED2_W=0;

	LED1_DIR=0;
	LED2_DIR=0;

	KeyNotify_IO=1;
	/************************************************************************/
	/* TMR2 ATָ�20ms                                                 */
	/************************************************************************/
	T2CON=0b00111011;//1:128
	PR2=227;
	TMR2ON=0;
	TMR2IE=0;//ʹ���ж�
	TMR2IF=0;//���жϱ�־
	/************************************************************************/
	/* TMR1 ϵͳ���ģ�100ms                                                 */
	/************************************************************************/
	T1CON=0b01110000;//1:1	
	WRITETIMER1(T1CYCLE_4M);
	TMR1ON=1;
	TMR1IE=1;//ʹ���ж�
	TMR1IF=0;//���жϱ�־

	
	/************************************************************************/
	/* ����1																*/
	/************************************************************************/
 	TRISC7=1;
 	TRISC6=1;
 	TXSTA=0b00100110;//�첽��ʽ��ʹ�ܷ���(bit6=1��9λ����)��λ�ֳ����߲�����(bit2����0)
 	RCSTA=0b10010000; //ʹ�ܴ��ڡ�ʹ�ܽ��ա��첽��ʽ��(bit6=1��9λ����)��λ�ֳ�
 	SPBRG=25;//0x67;	//Baud Rate = Fosc/(16(SPBRG+1)) = 9600
 	RCIE=1;//ʹ���ж�
 	RCIF=0;//���жϱ�־
	
	PEIE=1;
	GIE=1;
}
void InitVariable(void)
{
	if(GetRomData()==0)
	{
		RomParams.bWinter=1;
		SaveRomData();
	}
	gFlags.AllFlag=0;		
	SystemTick=0;	
#ifdef Test12M
	NeedMeasurementTick=600;
#elif defined(Test2M)
	NeedMeasurementTick=100;
#elif defined(Test)
	NeedMeasurementTick=1;
#else
	if(RomParams.bWinter==0)//����
	{
		NeedMeasurementTick=36000;//ÿСʱ����һ��
	}
	else
	{
		NeedMeasurementTick=6000;//ÿ10���Ӳ���һ��
	}
#endif	
	TxErrDataIndex=0;
}

void InitSystem( void )
{
	unsigned char x;
	InitMCU();	
	InitVariable();		
#ifndef NoScreen
	DisplayInit();
	if(RomParams.bWinter==0)//����
	{
		while(WA==0);//���𰴼�
		S10=1;//��̾��
	}
	else
	{
		while(WA==0);//���𰴼�
		S10=0;//��̾��
	}
#endif
	ATCommand_Init();	

	x=10;
	while(x!=0)
	{
		if(OneWire_Reset()==0) //�����߸�λ�ɹ�
		{
			gFlags.b18B20=1;
			break;
		}
		x--;
	}
	if(x==0)//û��18b20
	{
		gFlags.b18B20=0;
		TCN75_Init();
		TCN75_Config.bShutdown=1;
		TCN75_Config.COMP_INT=0;
		TCN75_Config.Polarity=0;
		TCN75_Config.FaultQueue=0b00;
		TCN75_Config.Resolution=0b11;
		TCN75_Config.OneShot=0;	
		TCN75_SetConfig();
	}
	__delay_20ms(50);
	ReadTData();
	__delay_20ms(250);
}
