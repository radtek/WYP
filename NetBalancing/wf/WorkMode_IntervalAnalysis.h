#ifndef __WorkMode_IntervalAnalysis_H__
#define __WorkMode_IntervalAnalysis_H__

#include "main.h"
#include "TypeDefine.h"

//�¶ȼ��������ҳ��
extern uint32_t IntervalAnalysis_TPageCount;
//�¶ȼ�������ĵ�ǰҳ��
extern uint32_t IntervalAnalysis_TPageIndex;
//�¶ȼ�����������ݱ��ؼ�����Ŀ�����
//extern uint32_t IntervalAnalysis_TRecordIndex;
void IntervalAnalysisModeInit(void);
void IntervalAnalysis_TBarChartsSwitchPage(uint16_t cID);
#endif
