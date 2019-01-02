#ifndef   __FlagDefine_h__
#define   __FlagDefine_h__
/************************************************************************/
/* ��һ��Ϊ���λ                                                       */
/************************************************************************/
typedef union   
{
	struct
	{
		unsigned bNotify:1;
		unsigned b18B20:1;
		unsigned bFirst:1;
		unsigned bErrFull:1;
		unsigned bRetry:1;
	};
	unsigned char AllFlag;
} _GFlags;        // general flags

#define RomCount 2
typedef union   
{
	struct
	{
 		unsigned char sum;//У���
//  		union   
//  		{
//  			struct
//  			{
//  				unsigned bCall:1;//�Ƿ����
// 				unsigned bWinter:1;
//  			};
//  			unsigned char AllFlag;
//  		} SystemState; 		
 		unsigned char bWinter;//�Ƿ�Ϊ����
// 		unsigned char ConnectRetry;//�������Դ���
// 		unsigned char MeasurementCycle;//�������
	};
	unsigned char All[RomCount];
} _RomParams;        // general flags
 
#define MaxTList 12
typedef union   
{
	uint T[MaxTList];
	unsigned char All[MaxTList*2];
} _TList;        // general flags
//_ROMParams ROMParams;
#endif
