//Build F:\work_tfs\MCU\WYP\Saving2\Saving2 for device 18F6621
//Using driver C:\Program Files\HI-TECH Software\PICC-18\PRO\9.63\bin\picc18.exe
#include "HardwareProfile.h"
__CONFIG(FOSC_HS&WDTE_ON&PWRTE_ON&MCLRE_ON&BOREN_ON&CLKOUTEN_OFF&CP_ON&CPD_ON);
__CONFIG(PLLEN_OFF&STVREN_ON&WRT_ALL);//���Ź�2s��λ
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
//unsigned char CallDelay;

void main()
{
	unsigned char addr,i;
	InitSystem();
	__delay_20ms(250);
	gFlags.bFirst=1;
	TestProc();	
	ReadTData();
	CallProc();
	gFlags.bFirst=0;
	MeasurementTick=SystemTick;
	while(1)
	{
		if(KeyNotify_R==0)//���м�
		{
			__delay_ms(20);
			if(KeyNotify_R==0)
			{
				gFlags.bNotify=1;
				ReadTData();
				CallProc();
				gFlags.bNotify=0;
			}
		}
		if(GetDeltaTick(RunTick)>20)
		{
			RunTick=SystemTick;
#ifndef NoScreen
			while(WA==0);//���𰴼�
			SA4=!SA4;
			while(WA==0);//���𰴼�
			SD4=!SD4;
#endif
		}
		if(GetDeltaTick(MeasurementTick)>NeedMeasurementTick)
		{
			MeasurementTick=SystemTick;
			ReadTData();
			TList.T[TListIndex++]=CurTemp;
			if(TListIndex>=MaxTList)
			{
				CallProc();
				TListIndex=0;
				if(ErrState!=NoErr)//���͹������д�����
				{
					if(TxErrDataIndex==0xff)//֮ǰû�д���
						TxErrDataIndex=0;
					addr=TxErrDataIndex*12;
					addr+=TxErrDataPtr;
					for(i=0;i<24;i++)
					{
						eeprom_write(addr+i,TList.All[i]);
					}					
					TxErrDataIndex++;
					if(TxErrDataIndex>=MaxTxErrTimes)
					{
						gFlags.bErrFull=1;
						TxErrDataIndex=0;
					}
				}
				else//û�д������ʾ���ݾ��ѷ�����ɣ��������
				{
					TxErrDataIndex=0xff;
					gFlags.bErrFull=0;
				}
			}
		}
		if(WERR==1)
		{
			WERR=0;
		}
	}
}
void interrupt highISR(void)
{		
	unsigned char rx;	
	if(RCIE&&RCIF)
	{
		RCIF=0;
		if(OERR==1)
		{
			CREN=0;
			OERR=0;
			CREN=1;
		}
		rx=RCREG;
		ATCommand_ProcRx(rx);
	}
	if(TMR1IF&& TMR1IE)
	{
		CLRWDT();
		TMR1IF=0;
		WRITETIMER1(T1CYCLE_4M);			
		SystemTick++;
	}	
}
void ReadTData(void)
{
	unsigned char retry;
	if(gFlags.b18B20)
	{
		retry=5;
		while(retry!=0)
		{
			if(DS18X20_StartMeas()!=DS18X20_OK)//18b20�޷���λ
			{
				retry--;
			}
			else
			{
				__delay_20ms(50);//1S
				if(DS18X20_ReadMeas(1)==DS18X20_OK)//��������
				{
					CurTemp.u8L=DS18X20_Scratchpad[0];
					CurTemp.u8H=DS18X20_Scratchpad[1];	
					break;
				}
				else//�¶ȶ�ȡ����
				{
					retry--;												
				}	
			}
		}
		if(retry==0)
			CurTemp.u16=ErrDS18X20_T;	
	}
	else//TCN57
	{
		TCN75_Config.OneShot=1;
		if(TCN75_SetConfig()==1)
		{
			__delay_20ms(25);
			if(TCN75_ReadReg(TCN75_TEMP)==1)//��ȷ��ȡ�¶�
			{
				CurTemp.u16=TCN75_RegReadResualt.u16>>4;				
			}
			else
			{
				CurTemp.u16=ErrDS18X20_T;
			}
		}	
		else
		{
			CurTemp.u16=ErrDS18X20_T;
		}
	}
#ifndef NoScreen
	LcdDisplay_CurTemp();
#endif
}
