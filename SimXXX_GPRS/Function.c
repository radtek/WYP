#include "main.h"
#include "string.h"
#include "stdio.h"
#include <ctype.h>
#include "stdlib.h"
void CallProc(void)
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
		gFlags.bRetry=0;
		//��ʾ�����𰴼���
		if(PowerON()==0)
		{
			ErrState=Miss900a;
		}
		else
		{
			CallSend();
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
// 			if(ErrState!=NoErr)
// 			{
// 				LcdDisplay_Err();
// 			}
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

void CallSend(void)
{
	unsigned char i,ret;
	char *strx=0;	
	ErrState=NoErr;
	InitGPRS();
	if(ErrState!=NoErr)
	{
		__delay_20ms(3000);
		ErrState=NoErr;
		InitGPRS();
		if(ErrState!=NoErr)
			return;
	}
	ATCommand_SendCmd("AT+CIPSTATUS\0");
	strcpy(ATCommand_OkString,"CONNECT OK");
	ATCommand_RetryTimes=1;
	ATCommand_WaitACKTimes=600;
	//if(ATCommand_SendCmd("AT+CIPSTART=\"TCP\",\"221.208.194.71\",\"12128\"\0")!=ATACK_OK)
#ifdef Port_12129
	if(ATCommand_SendCmd("AT+CIPSTART=\"TCP\",\"www.wlwdjcy.com\",\"12129\"\0")!=ATACK_OK)
#else
	if(ATCommand_SendCmd("AT+CIPSTART=\"TCP\",\"www.wlwdjcy.com\",\"12128\"\0")!=ATACK_OK)
#endif
	{
		ATCommand_SendCmd("AT+CIPSTART=?\0");
		ErrState=MissAT_CIPSTART;
		gFlags.bRetry=1;
		return;
	}

	__delay_20ms(50);

	ret=InteractServer();
	if(ret!=ATACK_OK)
	{
		if(ErrState==MissAT_ServerClose)
		{
			gFlags.bRetry=1;
			return;
		}
		/************************************************************************/
		/* 20180611,�����һ�η��ͣ�δ�յ����أ��򽫳���Ƶ�������������־δ��λ*/
		/************************************************************************/
		ErrState=NoErr;
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		ret=InteractServer();
		if(ret!=ATACK_OK)
		{
			gFlags.bRetry=1;
			return;
		}
	}
	//�������������ص�����
	strx=strstr((const char*)ATCommand_RxList,"#KO$");
	if(strx==0)
	{
		ErrState=MissAT_ErrServerData;
		return;
	}
#ifndef Test
	i=strx[4];
	i=i-'0';
	if(i!=RomParams.bWinter)//�л�����
	{
		RomParams.bWinter=i;
		SaveRomData();
		if(RomParams.bWinter==0)//����
		{
			NeedMeasurementTick=36000;//ÿСʱ����һ��
#ifndef NoScreen
			while(WA==0);//���𰴼�
			S10=1;//��̾��
#endif
		}
		else
		{
			NeedMeasurementTick=6000;//ÿ10���Ӳ���һ��
#ifndef NoScreen
			while(WA==0);//���𰴼�
			S10=0;//��̾��
#endif
		}
	}
#endif	
	//__delay_20ms(300);
	if(ATCommand_SendCmd("AT+CIPCLOSE=1\0")!=ATACK_OK)
	{
// 		ErrState=MissAT_CIPCLOSE;
// 		return;
	}

	if(ATCommand_SendCmd("AT+CIPSHUT\0")!=ATACK_OK)
	{
// 		ErrState=MissAT_CIPSHUT;
// 		return;
	}
}
unsigned char InteractServer()
{
	unsigned char res=0; 
	unsigned char retry,i,j,k,addr;
	u16_wf t;
	retry=1;//ATCommand_RetryTimes;
	res=ATACK_Null;
	while(retry!=0)
	{		
		strcpy(ATCommand_OkString,">");
		ATCommand_WaitACKTimes=400;
		if(ATCommand_SendCmd("AT+CIPSEND\0")!=ATACK_OK)
		{
			ErrState=MissAT_CIPSEND;
			ATCommand_TxByte(0x1b);
			return res;
		}
		if(gFlags.bFirst)//������һ�κ���
		{
			ATCommand_PutString("@#pt#1#\0");	
			ATCommand_PutString(ICCID);
			ATCommand_TxByte('#');			
#ifdef Test12M
			ATCommand_PutString("1#\0");
#else
			if(RomParams.bWinter==0)//����
			{
				ATCommand_PutString("60#\0");//�ļ�60����
			}
			else
			{
				ATCommand_PutString("10#\0");//ÿ10���Ӳ���һ��
			}
#endif	
			MakeTxTemperature(CurTemp);
			for(int i=0;i<4;i++)
			{
				ATCommand_TxByte(TxTemperature[i]);
			}
		}
		else if(gFlags.bNotify)//����
		{
			ATCommand_PutString("@#pt#3#\0");
			ATCommand_PutString(ICCID);
			ATCommand_TxByte('#');
#ifdef Test
			ATCommand_PutString("1#\0");
#else
			if(RomParams.bWinter==0)//����
			{
				ATCommand_PutString("60#\0");//�ļ�60����
			}
			else
			{
				ATCommand_PutString("10#\0");//ÿ10���Ӳ���һ��
			}
#endif
			MakeTxTemperature(CurTemp);
			for(int i=0;i<4;i++)
			{
				ATCommand_TxByte(TxTemperature[i]);
			}
		}
		else//��������
		{
			ATCommand_PutString("@#pt#2#\0");
			ATCommand_PutString(ICCID);
			ATCommand_TxByte('#');
#ifdef Test12M
			ATCommand_PutString("1#\0");
#else
			if(RomParams.bWinter==0)//����
			{
				ATCommand_PutString("60#\0");//�ļ�60����
			}
			else
			{
				ATCommand_PutString("10#\0");//ÿ10���Ӳ���һ��
			}
#endif
			if(TxErrDataIndex!=0xff)//֮ǰ�д�����
			{
				if(gFlags.bErrFull)//�Ѿ��洢����
				{
					for(i=(TxErrDataIndex-1);i<10;i--)
					{
						for(j=0;j<MaxTList;j++)
						{
							addr=(i)*12;
							addr+=TxErrDataPtr;
							addr+=(j<<1);
							t.u8s[0]=eeprom_read(addr);
							t.u8s[1]=eeprom_read(addr+1);
							MakeTxTemperature(t);
							for(k=0;k<4;k++)
							{
								ATCommand_TxByte(TxTemperature[k]);
							}
							ATCommand_TxByte('|');
						}
					}
					for(i=9;i>TxErrDataIndex;i--)
					{
						for(j=0;j<MaxTList;j++)
						{
							addr=(i)*12;
							addr+=TxErrDataPtr;
							addr+=(j<<1);
							t.u8s[0]=eeprom_read(addr);
							t.u8s[1]=eeprom_read(addr+1);
							MakeTxTemperature(t);
							for(k=0;k<4;k++)
							{
								ATCommand_TxByte(TxTemperature[k]);
							}
							ATCommand_TxByte('|');
						}
					}
				}
				else
				{
					for(i=0;i<TxErrDataIndex;i++)
					{

						for(j=0;j<MaxTList;j++)
						{
							addr=i*12;
							addr+=TxErrDataPtr;
							addr+=(j<<1);
							t.u8s[0]=eeprom_read(addr);
							t.u8s[1]=eeprom_read(addr+1);
							MakeTxTemperature(t);
							for(k=0;k<4;k++)
							{
								ATCommand_TxByte(TxTemperature[k]);
							}
							ATCommand_TxByte('|');
						}
					}
				}
			}
			for(i=0;i<MaxTList;i++)
			{
				MakeTxTemperature(TList.T[i]);
				for(int j=0;j<4;j++)
				{
					ATCommand_TxByte(TxTemperature[j]);
				}
				if(i!=(MaxTList-1))
				{
					ATCommand_TxByte('|');
				}
			}			
		}
		ATCommand_TxByte('#');
		ATCommand_TxByte('@');
		ATCommand_TxByte(0x1a);
		ATCommand_WaitACKTimes=400;
		ATCommand_Clear=0x1a;
		strcpy(ATCommand_OkString,"$OK#");	
		strcpy(ATCommand_ErrString,"CLOSE");
		res=ATCommand_WaitData();
		if(res!=ATACK_Null)
			break;
		ATCommand_WaitClientInterval();
		retry--;
	}
	if(res!=ATACK_OK)
	{
		if(res==ATACK_Err)
			ErrState=MissAT_ServerClose;
		else
			ErrState=MissAT_NoServerData;
		ATCommand_TxByte(0x1b);
		return res;
	}
// 	ATCommand_WaitACKTimes=200;//�ȴ����������أ�20s
// 	strcpy(ATCommand_OkString,"$OK#");
// 	res=ATCommand_WaitData();
// 	if(res!=ATACK_OK)
// 		return MissAT_NoServerData;
	//�ȴ��������ظ�
	return res;
}
void MakeTxTemperature(u16_wf T)
{
	unsigned char x;
	x=HIGH_NIBBLE(T.u8H);
	if(x>9)
	{
		x=x-10;
		x=x+0x41;
		TxTemperature[0]=x;
	}
	else
	{
		x=x+'0';
		TxTemperature[0]=x;
	}
	x=LOW_NIBBLE(T.u8H);
	if(x>9)
	{
		x=x-10;
		x=x+0x41;
		TxTemperature[1]=x;
	}
	else
	{
		x=x+'0';
		TxTemperature[1]=x;
	}
	x=HIGH_NIBBLE(T.u8L);
	if(x>9)
	{
		x=x-10;
		x=x+0x41;
		TxTemperature[2]=x;
	}
	else
	{
		x=x+'0';
		TxTemperature[2]=x;
	}
	x=LOW_NIBBLE(T.u8L);
	if(x>9)
	{
		x=x-10;
		x=x+0x41;
		TxTemperature[3]=x;
	}
	else
	{
		x=x+'0';
		TxTemperature[3]=x;
	}
}
void InitGPRS(void)
{
	unsigned char retry,i;
	char *strx=0;
	__delay_20ms(150);
#ifdef Sim800C	
	if(ATCommand_SendCmd("AT\0")!=ATACK_OK)
	{
		ErrState=MissAT;
		return;
	}
	__delay_20ms(1000);
	ATCommand_TxByte(0x1b);
	if(ATCommand_SendCmd("ATH\0")!=ATACK_OK)
	{
		ErrState=MissAT_ATH;
		return;
	}
#else
	__delay_20ms(1000);
	ATCommand_TxByte(0x1b);
	if(ATCommand_SendCmd("ATH0\0")!=ATACK_OK)
	{
		ErrState=MissAT_ATH;
		return;
	}
#endif	
	
	if(ATCommand_SendCmd("ATE1\0")!=ATACK_OK)
	{
		ErrState=MissAT_ATE1;
		return;
	}	
	retry=10;
	while(retry!=0)
	{
		if(ATCommand_SendCmd("AT+CIMI\0")==ATACK_OK)
		{
			break;
		}
		__delay_20ms(150);
		retry--;		
	}	
	if(retry==0)
	{
		ErrState=MissSimCard;//û������
		return;
	}
	retry=10;
	while(retry!=0)
	{
#ifdef Sim800C
		if(ATCommand_SendCmd("AT+ICCID\0")==ATACK_OK)
		{
			strx=strstr((const char*)ATCommand_RxList,"+ICCID: ");
			if(strx!=0)
			{
				for(i=0;i<15;i++)
				{
					ICCID[i]=strx[13+i];
				}
				ICCID[15]='\0';
				break;
			}
			else
			{
				retry--;
				__delay_20ms(150);
			}
		}
#else
		if(ATCommand_SendCmd("AT+CCID\0")==ATACK_OK)
		{
			strx=strstr((const char*)ATCommand_RxList,"8986");
			if(strx!=0)
			{
				for(i=0;i<15;i++)
				{
					ICCID[i]=strx[i+5];
				}
				ICCID[15]='\0';
				break;
			}
			else
			{
				retry--;
				__delay_20ms(150);
			}
		}
#endif
			
	}
	//sim900a_send_cmd("AT+CIPCLOSE=1","CLOSE OK",100);	//�ر�����
	//�رն�����ʾ
	if(ATCommand_SendCmd("AT+CNMI=0,0\0")!=ATACK_OK)
	{
		ErrState=MissAT_CNMI;
		return;
	}	
	//txt��ʽ����
	if(ATCommand_SendCmd("AT+CMGF=1\0")!=ATACK_OK)
	{
		ErrState=MissAT_CMGF;
		return;
	}
	//	while(1)
	//	{
	//		ATCommand_SendCmd("AT+CSQ\0");
	//	}	
	retry=10;
	CSQ=0xff;
	while(retry!=0)
	{
		if(ATCommand_SendCmd("AT+CSQ\0")==ATACK_OK)
		{
			strx=strstr((const char*)ATCommand_RxList,"+CSQ: ");
			if(strx!=0)
			{
				CSQ=atoi(strx+6);
#ifndef NoScreen
				LcdDisplay_CSQ();
#endif
				if(CSQ!=0)
					break;
			}
			else
			{
				retry--;
				__delay_20ms(150);
			}
		}		
	}
	if(retry==0 && CSQ!=0xff)
	{
		ErrState=MissAT_CSQ;
		return;
	}
	//��ѯ�Ƿ�ע��������
// 	retry=10;
// 	while(retry!=0)
// 	{
// 		if(ATCommand_SendCmd("AT+COPS?\0")!=ATACK_OK)
// 		{		
// 			ErrState=MissAT_COPS;
// 			return;
// 		}
// 		strx=strstr((const char*)ATCommand_RxList,"+COPS: 0,0,\"");
// 		if(strx!=0)
// 		{
// 			break;
// 		}
// 		__delay_20ms(150);
// 		retry--;		
// 	}
// 	if(retry==0)
// 	{
// 		ErrState=MissNet;//û������
// 		return;
// 	}
	retry=20;
	while(retry!=0)
	{
		if(ATCommand_SendCmd("AT+CGREG?\0")!=ATACK_OK)
		{		
			ErrState=MissAT_COPS;
			return;
		}
		strx=strstr((const char*)ATCommand_RxList,"+CGREG: 0,1");
		if(strx!=0)
		{
			break;
		}
		strx=strstr((const char*)ATCommand_RxList,"+CGREG: 0,5");
		if(strx!=0)
		{
			break;
		}
		__delay_20ms(150);
		retry--;		
	}
	if(retry==0)
	{
		ErrState=MissNet;//û������
		return;
	}
	if(ATCommand_SendCmd("AT+COLP=1\0")!=ATACK_OK)
	{
		ErrState=MissAT_COLP;
		return;
	}
// 	if(ATCommand_SendCmd("AT+CLCC=1\0")!=ATACK_OK)
// 	{
// 		ErrState=MissAT_CLCC;
// 		return;
// 	}
	//���Թر�ip����
	// 	strcpy(ATCommand_OkString,"CLOSE OK");
	// 	ATCommand_SendCmd("AT+CIPCLOSE\0");
	// 
	// 	strcpy(ATCommand_OkString,"SHUT OK");
	// 	if(ATCommand_SendCmd("AT+CIPSHUT\0")!=ATACK_OK)
	// 	{
	// 		ErrState=MissAT_CIPSHUT;
	// 		return;
	// 	}
	//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ���
// 	if(ATCommand_SendCmd("AT+CGCLASS=\"B\"\0")!=ATACK_OK)
// 	{
// 		ErrState=MissAT_CGCLASS;
// 		gFlags.bRetry=1;
// 		return;
// 	}
	//����PDP������,��������Э��,��������Ϣ
// 	if(ATCommand_SendCmd("AT+CGDCONT=1,\"IP\",\"CMNET\"\0")!=ATACK_OK)
// 	{
// 		ErrState=MissAT_CGDCONT;
// 		gFlags.bRetry=1;
// 		return;
// 	}
	//����GPRSҵ��
// 	if(ATCommand_SendCmd("AT+CGATT=1\0")!=ATACK_OK)
// 	{
// 		ErrState=MissAT_CGATT;
// 		gFlags.bRetry=1;
// 		return;
// 	}
// 	//����ΪGPRS����ģʽ
// 	if(ATCommand_SendCmd("AT+CIPCSGP=1,\"CMNET\"\0")!=ATACK_OK)
// 	{
// 		ErrState=MissAT_CIPCSGP;
// 		gFlags.bRetry=1;
// 		return;
// 	}
	if(ICCID[0]=='6'|| ICCID[0]=='1')
	{
		if(ATCommand_SendCmd("AT+CSTT=\"UNIM2M.NJM2MAPN\",\"\",\"\"\0")!=ATACK_OK)
		{
			ErrState=MissAT_CSTT;
			gFlags.bRetry=1;
			return;
		}
	}
	else
	{
		if(ATCommand_SendCmd("AT+CSTT=\"CMMTM\",\"\",\"\"\0")!=ATACK_OK)
		{
			ErrState=MissAT_CSTT;
			gFlags.bRetry=1;
			return;
		}
	}
	ATCommand_WaitACKTimes=400;
	if(ATCommand_SendCmd("AT+CIICR\0")!=ATACK_OK)
	{
		ErrState=MissAT_CIICR;
		gFlags.bRetry=1;
		return;
	}
	// 	//��ѯ����ip
	ATCommand_SendCmd("AT+CIFSR\0");
}
unsigned char PowerDown(void)
{
	unsigned char i;
	if(STATUS_R==isPowerON)//�Ѿ��������ȹػ�
	{
		POWER_ON_W=1;
		__delay_20ms(70);
		POWER_ON_W=0;
		i=15;
		while(i!=0)
		{
			__delay_20ms(50);
			if(STATUS_R==isPowerOFF)
				break;			
			i--;
		}
	}
	if(i==0)
		return 0;
	return 1;
}
unsigned char PowerON(void)
{
	unsigned char i;
	if(STATUS_R==isPowerON)//�Ѿ��������ȹػ�
	{
		if(PowerDown()==0)
		{
			ErrState=Miss900a;
			return 0;
		}
		__delay_20ms(150);
	}	
	POWER_ON_W=1;	
	i=15;
	while(i!=0)
	{
		ATCommand_RetryTimes=1;
		if(ATCommand_SendCmd("AT\0")==ATACK_OK)
		{
			ErrState=MissAT;
			POWER_ON_W=0;
			return 1;
		}
		i--;
	}
	POWER_ON_W=0;
	return 0;

// 	POWER_ON_W=1;
// 	__delay_20ms(70);
// 	POWER_ON_W=0;
// 	i=15;
// 	while(i!=0)
// 	{
// 		__delay_20ms(50);
// 		if(STATUS_R==isPowerON)
// 			break;
// 		i--;
// 	}
// 	if(i==0)
// 		return 0;
// 	return 1;
}
