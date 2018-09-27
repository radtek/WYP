#include "TFTWorkProc.h"
#include "..\VisualTFT\hmi_driver.h"
#include "..\WirelessProc.h"
#include "FlagDefine.h"
#include "DS18x20.h"
#include "FRAM.h"
#include "WorkMode_RealTime.h"

_RTCData RTCData;
uint32_t WaitTFTTick;
void ReadRTC(void)
{
	uint32_t retry=3;
	while(retry!=0)
	{
		gFlags.bRxRTC=0;
		GetRTC();
		WaitTFTTick=HAL_GetTick();
		while(1)
		{
			if(GetDeltaTick(WaitTFTTick)>1000)
				break;
			if(gFlags.bRxRTC)
				return;
		}
	}
}
void TFT_NewSensorDataProc(void)
{		
	uint8_t rData[50];
	uint8_t t1,t2;
	t1=(PickDatas.SensorData&0x0fff)>>4;
	t2=PickDatas.SensorData&0x000f;
	if(PickDatas.StateBits.bTemperature)//�¶�
	{
		FRAM_GetSensorDataInfo(PickDatas.index);
		sprintf(rData,"%u;%03u;%02u:%02u:%02u;%u.%u;%s",
			rtDataIndex,PickDatas.index,
			RTCData.hour,RTCData.minute,RTCData.second,
			t1,DS18X20_ToDecicel[t2],
			SensorDataInfo.InstallPosition);
		//2ʵʱ�б�
		Record_Add(2,cID_Record,rData);
	}
}
uint32_t JumpButtonProc(uint16_t control_id)
{
	switch(control_id)
	{
	case 1://����
		WirelessOff();
		break;
	case 4://ʵʱ����
		SetScreen(2);//�л���ʾ��ʵʱ���ݲɼ�ҳ��
		Wireless_RxInit();//��������
		Record_Clear(2,cID_Record);
		break;
	case 5://��ʷ����
		WirelessOff();
		break;
	case 6://�������
		WirelessOff();
		break;
	case 7://�������
		WirelessOff();
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
