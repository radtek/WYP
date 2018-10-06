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
	uint8_t t1,t2,x1,x2,x,i;	
	if(WirelessPacket.StateBits.bTemperature)//�¶�
	{	
		t1=(WirelessPacket.SensorData&0x0fff)>>4;
		t2=WirelessPacket.SensorData&0x000f;
		//2ʵʱ�б�	
		sprintf(rData,"%u;%03u;%02u:%02u:%02u;%u.%u;%u-%u",
			realtimeTDataIndex,WirelessPacket.index,
			RTCData.hour,RTCData.minute,RTCData.second,
			t1,DS18X20_ToDecicel[t2],
			SensorDataInfo.Addr1,SensorDataInfo.Addr2);		
		Record_Add(2,cID_Record,rData);
		//4ʵʱ�¶�����ͼ
		x=0xff;
		for(i=0;i<TSensorAddrSortingListCount;i++)
		{
			if(TSensorAddrSortingList[i].SensorIndex==WirelessPacket.index)
			{
				x=i;
				break;
			}
		}
		if(x==0xff)//û�ҵ����޷�������
			return;
		x1=realtimeTPageOfBarCharts*SensorCountOfBarCharts;
		x2=x1+SensorCountOfBarCharts;
		if((x1<=x) && (x<x2))//�ڵ�ǰ��ʾ��ҳ����
		{
			x2=x-x1;//ʣ��������������ε�����
			if(t2>8)
				t1++;
			SetProgressValue(4,cID_Bar1+x2,t1);
		}
	}
	else
	{
		//�ȴ����䣺ѹ�����ݵĻ��㷽ʽ
		//3ʵʱѹ��
		sprintf(rData,"%u;%03u;%02u:%02u:%02u;%u.%u;%u-%u",
			realtimePDataIndex,WirelessPacket.index,
			RTCData.hour,RTCData.minute,RTCData.second,
			t1,DS18X20_ToDecicel[t2],
			SensorDataInfo.Addr1,SensorDataInfo.Addr2);		
		Record_Add(3,cID_Record,rData);
		//5ʵʱѹ������ͼ
		x=0xff;
		for(i=0;i<PSensorAddrSortingListCount;i++)
		{
			if(PSensorAddrSortingList[i].SensorIndex==WirelessPacket.index)
			{
				x=i;
				break;
			}
		}
		if(x==0xff)//û�ҵ����޷�������
			return;
		x1=realtimePPageOfBarCharts*SensorCountOfBarCharts;
		x2=x1+SensorCountOfBarCharts;
		if((x1<=x) && (x<x2))//�ڵ�ǰ��ʾ��ҳ����
		{
			x2=x-x1;//ʣ��������������ε�����
			if(t2>8)
				t1++;
			SetProgressValue(4,cID_Bar1+x2,t1);
		}
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
