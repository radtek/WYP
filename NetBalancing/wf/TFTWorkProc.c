#include "TFTWorkProc.h"
#include "..\VisualTFT\hmi_driver.h"
#include "..\WirelessProc.h"
#include "FlagDefine.h"
#include "DS18x20.h"
#include "FRAM.h"
#include "WorkMode_RealTime.h"
#include "cmd_process.h"
#include "cmd_queue.h"
#include "wfDefine.h"
#include "WorkMode_DataAnalysis.h"
#include "WorkMode_IntervalAnalysis.h"

uint32_t autoReturnSID;
_RTCData RTCData;
uint32_t WaitTFTTick;
_TFTNotifyCommandFlags TFTNotifyCommandFlags; 
_TFTWaitCommandFlags TFTWaitCommandFlags;
extern uint32_t LEDTick;
void myRecordClear(unsigned char sID,unsigned char cID,unsigned char count)
{
	uint8_t i;
	for(i=0;i<count;i++)
		Record_DeleteRow(sID,cID,0);
}
uint32_t ReadTextbox(unsigned char sID,unsigned char cID)
{
	uint32_t retry=3;
	uint8_t i;
	while(retry!=0)
	{
		TFTWaitCommandFlags.bWaitControlValue=1;
		TFTNotifyCommandFlags.bRxControlValue=0;
		waitSID=sID;
		waitCID=cID;
		GetControlValue(sID,cID);
		WaitTFTTick=HAL_GetTick();
		while(1)
		{			
			if(GetDeltaTick(WaitTFTTick)>1000)
				break;
			TFTRxCheck();
			if(TFTNotifyCommandFlags.bRxControlValue)
			{
					return 1;
			}
		}
		retry--;
	}
	return 0;
}

void ReadRTC(void)
{
	uint32_t i,retry=3;
	while(retry!=0)
	{
		TFTWaitCommandFlags.bWaitRTC=1;
		TFTNotifyCommandFlags.bRxRTC=0;
		GetRTC();
		WaitTFTTick=HAL_GetTick();
		while(1)
		{
			if(GetDeltaTick(WaitTFTTick)>1000)
				break;
			TFTRxCheck();
			if(TFTNotifyCommandFlags.bRxRTC)
			{
				for(i=0;i<7;i++)
					RTCData.All[i]=BCD2Char(RTCData.All[i]);
				return;
			}
		}
		retry--;
	}
}
void ResetTFT(void)
{
	while(1)
	{
		TFTWaitCommandFlags.bWaitReset=1;
		TFTNotifyCommandFlags.bRxReset=0;
		ResetDevice();
		WaitTFTTick=HAL_GetTick();
		while(1)
		{
			if(GetDeltaTick(WaitTFTTick)>1000)
				break;
			TFTRxCheck();
			if(TFTNotifyCommandFlags.bRxReset)
			{
				wfDelay_ms(1000);
				queue_reset();
				return;
			}
		}
	}
}
void TFTRxCheck(void)
{
	qsize  size = 0;
	size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE); //�ӻ������л�ȡһ��ָ��        
	if(size>0)//���յ�ָ��
	{
		ProcessMessage((PCTRL_MSG)cmd_buffer, size);//ָ���
		queue_reset();
	}	
}
void ShowFailDialog(uint32_t returnSID,uint8_t* str)
{
	autoReturnSID=returnSID;
	SeTimer(sID_Fail,cID_Timer,5);
	StartTimer(sID_Fail,cID_Timer);
	SetTextValue(sID_Fail,50,str);
	SetScreen(sID_Fail);
}
void ShowSuccessDialog(uint32_t returnSID)
{
	autoReturnSID=returnSID;
	SeTimer(sID_Success,cID_Timer,5);
	StartTimer(sID_Success,cID_Timer);
	SetScreen(sID_Success);
}
uint32_t JumpButtonProc(uint16_t control_id)
{
	switch(control_id)
	{
	case 1://����
		WirelessOff();		
		break;
	case 4://ʵʱ����
		gFlags.bRealTime=1;
		LEDTick=HAL_GetTick();
		HAL_GPIO_WritePin(RUN_LED_C_GPIO_Port,RUN_LED_C_Pin,GPIO_PIN_SET);
		SetScreen(2);//�л���ʾ��ʵʱ���ݲɼ�ҳ��
		Wireless_RxInit();//��������		
		break;
	case 5://��ʷ����
		WirelessOff();
		break;
	case 6://�������
		WirelessOff();
		DataAnalysisModeInit();
		break;
	case 7://�������
		WirelessOff();
		IntervalAnalysisModeInit();
		break;
	case 8://ϵͳ����
		WirelessOff();
		break;
	case 11://����˵��
		WirelessOff();
		break;
	default:
		return 0;
	}
	return 1;
}
