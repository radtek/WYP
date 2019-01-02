#include "HardwareProfile.h"
#include "string.h"
#include "stdio.h"
#include <ctype.h>
#include "stdlib.h"
void CallTestSend(void);
void CallTestProc(void);
void TestProc(void)
{
	if(TestMode_R==0)
	{
		__delay_ms(20);
		if(TestMode_R==0)
		{
			LcdDisplayAll(0);
			__delay_20ms(150);
			LcdDisplayAll(1);
			ReadTData();
			if(CurTemp.u16==ErrDS18X20_T)
			{
				LED1_W=0;
				LED2_W=1;
				ErrState=MissT;
				LcdDisplay_Err();
			}
			else
			{
				ErrState=NoErr;
				CallTestProc();	
				if(ErrState!=NoErr)
				{
					LED1_W=0;
					LED2_W=1;
					LcdDisplay_Err();
				}
				else
				{
					LED1_W=1;
					LED2_W=0;
					LcdDisplayAll(2);
				}
			}					
			while(1)
			{
				if(ErrState!=NoErr)
				{
					__delay_20ms(20);
					LED2_W=!LED2_W;
				}

			}
		}
	}
}
void CallTestProc(void)
{
#ifndef NoScreen
	while(WA==0);//���𰴼�
	S01=1;
	while(WA==0);
	S03=1;
	while(WA==0);
	S07=0;
	while(WA==0);
	S08=0;
	if(gFlags.bNotify)
	{
		while(WA==0);//����Ͷ��
		S05=1;
		while(WA==0);
		S06=1;
	}
#endif
	unsigned char retry=2;
	while(retry!=0)
	{
		ErrState=NoErr;	
		//��ʾ�����𰴼���
		if(PowerON()==0)
		{
			ErrState=Miss900a;
		}
		else
		{
			CallTestSend();
			if(ErrState!=NoErr)
			{
				if(STATUS_R==isPowerOFF)//�Զ��ػ�
				{
					ErrState=MissPower;
				}
			}
			PowerDown();
		}
		if(gFlags.bRetry)
		{
			retry--;
			LcdDisplay_CurTemp();
			__delay_20ms(3000);//δ�����Ϸ���������һ��������һ��
		}
		else
			break;
	}
	//ȡ�������𰴼���
#ifndef NoScreen
	while(WA==0);//���𰴼�
	S01=0;
	while(WA==0);
	S03=0;
	while(WA==0);//����Ͷ��
	S05=0;
	while(WA==0);
	S06=0;
	if(ErrState!=NoErr)
	{
		LcdDisplay_Err();
	}
	else
	{
		LcdDisplay_CurTemp();
	}
#endif
}
void CallTestSend(void)
{
	unsigned char ret;
	char *strx=0;		
	InitGPRS();
	if(ErrState!=NoErr)
		return;
	if(ATCommand_SendCmd("AT+NSOCR=STREAM,6,0,1\0")!=ATACK_OK)
	{
		ErrState=MissAT_NSOCR;
		return;
	}
	ATCommand_RetryTimes=1;
	ATCommand_WaitACKTimes=600;
	if(ATCommand_SendCmd("AT+NSOCO=1,\"www.wlwdjcy.com\",12129\0")!=ATACK_OK)
	{
		ErrState=MissAT_NSOCR;
		return;
	}

	__delay_20ms(50);

	ret=InteractServer();
// 	if(ret!=ATACK_OK)
// 	{
// 		if(ErrState==MissAT_ServerClose)
// 		{	
// 			gFlags.bRetry=1;
// 			return;
// 		}
// 		ret=InteractServer();
// 		if(ret!=ATACK_OK)
// 		{
// 			gFlags.bRetry=1;
// 			return;
// 		}
// 	}
	
}

