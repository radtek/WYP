#ifndef __WorkMode_DataAnalysis_H__
#define __WorkMode_DataAnalysis_H__

#include "main.h"
#include "TypeDefine.h"

//�¶ȼ��������ҳ��
extern uint32_t DataAnalysis_DA_TPageCountOfRecord;
//�¶ȼ�������ĵ�ǰҳ��
extern uint32_t DataAnalysis_DA_TPageIndexOfRecord;
//�¶ȼ�����������ݱ��ؼ�����Ŀ�����
//extern uint32_t IntervalAnalysis_TRecordIndex;
void DataAnalysisModeInit( void );
void DataAnalysis_DA_TRecordSwitchPage(uint16_t cID);
void DataAnalysis_DA_TBarChartsSwitchPage(uint16_t cID);
void DataAnalysis_DY_TRecordSwitchPage(uint16_t cID);
void DataAnalysis_DY_TBarChartsSwitchPage(uint16_t cID);
#endif
