#include "HardwareProfile.h"
void InitMCU(void)             //ϵͳ��ʼ������
{	
	RCON=0;
	//AD��ʼ��	
	ADCON1=0b00111111;  //ȫ��Ϊ���ݿ�

	Si4463_SDN_W=1;//si4432��λ	
	Si4463_SDN_DIR=0;	
	/************************************************************************/
	/* TMR2 ǿ�ư�ť��ʱ��1ms                                               */
	/************************************************************************/
// 	T2CON=0b01111010;//1:256,10us
// 	PR2=196;//50+ms
// 	TMR2IE=1;//ʹ���ж�
// 	TMR2IF=0;//���жϱ�־

	IDLEN=0;
	/************************************************************************/
	/* �ⲿ�ж�1                                                            */
	/************************************************************************/
// 	TRISB1=1;
// 	TRISB0=1;
// 	INTEDG0=0;
// 	INT0IF=0;
// 	INT0IE=0;
	/************************************************************************/
	/* ����1																*/
	/************************************************************************/
// 	TRISC7=1;
// 	TRISC6=1;
// 	TXSTA=0b00100110;//�첽��ʽ��ʹ�ܷ���(bit6=1��9λ����)��λ�ֳ����߲�����(bit2����0)
// 	RCSTA=0b10010000; //ʹ�ܴ��ڡ�ʹ�ܽ��ա��첽��ʽ��(bit6=1��9λ����)��λ�ֳ�
// 	//BAUDCON1=0x40;//�����Զ��������Զ����
// 	SPBRG=0x67;	//Baud Rate = Fosc/(16(SPBRG+1)) = 9600
// 	RCIP=0;//�����ȼ�
// 	RCIE=1;//ʹ���ж�
// 	RCIF=0;//���жϱ�־
	
// 	PEIE=1;
// 	GIE=1;
}
void InitVariable(void)
{
	gFlags.AllFlag=0;	
//	SystemTick=0;
	PickDatas.T1.u16=0xffff;

	if(GetRomData()==0)
	{
		ROMParams.MyAddr=0x01;
		ROMParams.WirelessPowerLevel=4;
		ROMParams.WirelessParamIndex=0;
		//
		ROMParams.SystemState.bWinter=1;
		ROMParams.PickCycle=1;
		SaveRomData();	
	}

// 	ROMParams.MyAddr.u8H=0;
// 	ROMParams.MyAddr.u8L=1;
	
	PickDatas.index=ROMParams.MyAddr;
}

void InitSystem( void )
{
	//unsigned int ii;
	InitMCU();
	InitVariable();		
  	Si4463_Init();
}
