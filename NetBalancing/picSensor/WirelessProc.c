#include "HardwareProfile.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
//#include "RADIO_CONFIG_SI446x.h"
#include "CONFIG_470tx.H"
const unsigned char config_table[] = RADIO_CONFIGURATION_DATA_ARRAY;
unsigned char MissWirelessTimes;


void Wireless_Init(void)
{
		
}
void Wireless_SetInit(void)
{
	
}
void Wireless_WorkInit(void)
{	
	unsigned char i,*cfg;
	unsigned int j = 0;
	cfg=(unsigned char*)config_table;
	Si4463_RESET();
	while(1)
	{
		i=cfg[j];
		if(i==0)
			break;
		j += 1;
		Si4463_CMD( cfg + j, i );
		j += i;
	}

#if PACKET_LENGTH > 0           //fixed packet length
	Si4463_SET_PROPERTY_1( PKT_FIELD_1_LENGTH_7_0, PACKET_LENGTH );
#else                           //variable packet length
	Si4463_SET_PROPERTY_1( PKT_CONFIG1, 0x00 );
	Si4463_SET_PROPERTY_1( PKT_CRC_CONFIG, 0x00 );
	Si4463_SET_PROPERTY_1( PKT_LEN_FIELD_SOURCE, 0x01 );
	Si4463_SET_PROPERTY_1( PKT_LEN, 0x2A );
	Si4463_SET_PROPERTY_1( PKT_LEN_ADJUST, 0x00 );
	Si4463_SET_PROPERTY_1( PKT_FIELD_1_LENGTH_12_8, 0x00 );
	Si4463_SET_PROPERTY_1( PKT_FIELD_1_LENGTH_7_0, 0x01 );
	Si4463_SET_PROPERTY_1( PKT_FIELD_1_CONFIG, 0x00 );
	Si4463_SET_PROPERTY_1( PKT_FIELD_1_CRC_CONFIG, 0x00 );
	Si4463_SET_PROPERTY_1( PKT_FIELD_2_LENGTH_12_8, 0x00 );
	Si4463_SET_PROPERTY_1( PKT_FIELD_2_LENGTH_7_0, 0x10 );
	Si4463_SET_PROPERTY_1( PKT_FIELD_2_CONFIG, 0x00 );
	Si4463_SET_PROPERTY_1( PKT_FIELD_2_CRC_CONFIG, 0x00 );
#endif //PACKET_LENGTH
	//��Ҫ�� 4463��GDO2��GDO3������Ƶ���أ�  33 ,32 
	//����ʱ���룺 GDO2=0��GDO3=1
	//����ʱ���룺 GDO2=1��GDO3=0
	//���ԣ�����������4463�Ǳ���ģ�4460��4461����Ҫ��Ƶ���أ������ȥ�������
	//Si4463_GPIO_CONFIG( 0, 0, 33|0x40, 32|0x40, 0, 0, 0 );    //��Ҫ
	Si4463_GPIO_CONFIG( 0, 0, 0x20, 0x21, 0, 0, 0 );//��Ҫ
}

