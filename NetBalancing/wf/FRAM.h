#ifndef __FRAM_h__
#define __FRAM_h__

#include "main.h"
//ϵͳ����
extern _ROMParams ROMParams;
//FRAM_GetSensorDataInfo�����Ĵ��������ݴ洢״̬
extern _SensorDataInfo SensorDataInfo;

#define IsLatestSensorDataInvalid() (LatestSensorData.Month==0xff)
#define IsLatestSensorDataValid() (LatestSensorData.Month!=0xff)
//FRAM_GetSensorDataInfo�����Ĵ���������һ������
extern _SensorData LatestSensorData;

extern uint32_t TminAddr1,TminAddr2,TmaxAddr1,TmaxAddr2;
extern uint32_t PminAddr1,PminAddr2,PmaxAddr1,PmaxAddr2;

//�¶ȴ���������ŵ�ַ����
extern _SensorAddrInfo TSensorAddrList[SensorDataItemCount];
//ѹ������������ŵ�ַ����
extern _SensorAddrInfo PSensorAddrList[SensorDataItemCount];
//�¶ȴ���������ַ���������
extern _SensorAddrSortingInfo TSensorAddrSortingList[SensorDataItemCount];
//ѹ������������ַ���������
extern _SensorAddrSortingInfo PSensorAddrSortingList[SensorDataItemCount];
//ʵ���¶ȴ���������
extern uint8_t realTSensorCount;
//ʵ��ѹ������������
extern uint8_t realPSensorCount;
//�¶ȴ���������ַ����������ܳ���
extern uint8_t TSensorAddrSortingListCount;
//ѹ������������ַ����������ܳ���
extern uint8_t PSensorAddrSortingListCount;

//������
uint8_t TAddr1ListCount;
//������
uint8_t PAddr1ListCount;
//���б�
uint8_t TAddr1List[SensorDataItemCount];
//���б�
uint8_t PAddr1List[SensorDataItemCount];

void FRAM_SaveROMParams(void);
void FRAM_GetROMParams(void);
void FRAM_InitSensorData(uint8_t startIndex,uint8_t endIndex);
void FRAM_InitSensorAddrInfo(void);


void FRAM_GetLatestSensorData(uint8_t bT,uint8_t index);//��ȡ��������������
void FRAM_GetSensorDataInfo(uint8_t bT,uint8_t index);
void FRAM_SaveSensorAddr(void);
#endif


