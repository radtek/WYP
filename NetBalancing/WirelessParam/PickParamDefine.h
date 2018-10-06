#ifndef __PickParamDefine_h__
#define __PickParamDefine_h__
typedef union   
{
	struct
	{
		unsigned bMove:1;//�ƶ�
		unsigned bLowPower:1;//�͵�
		unsigned bBreak:1;//����
		unsigned bTemperature:1;//�Ƿ�Ϊ�¶ȴ�����
	};
	unsigned char AllFlag;
} _ModelState; 

#ifdef TXTEST
#define  PACKET_LENGTH    10
#else
#define  PACKET_LENGTH    5
#endif

#define SetFreq 3

typedef union   
{
	struct
	{
		unsigned char index;		
		_ModelState StateBits;
		uint16_t SensorData;
		unsigned char sum;
	};
	unsigned char All[PACKET_LENGTH];
} _WirelessPacket;        // general flags 
#endif
