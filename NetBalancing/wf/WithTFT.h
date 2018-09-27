#ifndef __WithTFT_H__
#define __WithTFT_H__

#include "main.h"
#include "TypeDefine.h"

/************************************************************************/
/* ����id                                                               */
/************************************************************************/
#define sID_Screen0 0
#define sID_Screen1 1
#define sID_Menu 2
#define sID_Setting 3
#define sID_WIFIconfiguration 4
#define sID_wifimima 5
#define sID_wxWifi 6
#define sID_APKWifi 7
#define sID_ParamSet 8
#define sID_ExperimentConfig 10
#define sID_FactorySettings 11
#define sID_hWorkCurrent 12
#define sID_FuAnWorkCurrent 13
#define sID_ExperimentalProject 15
#define sID_AutoVA 16
#define sID_AutoGQ 17
#define sID_AutoPuLangke 18
#define sID_ManVA 19
#define sID_ManGQ 20
#define sID_PLdatePrecessing 22
#define sID_Print 23
#define sID_EnterDlg 25
#define sID_AutoVA_Work 26
#define sID_AutoGQ_Work 27
#define sID_ManVA_Work 28
#define sID_ManGQ_Work 29
#define sID_QueryDlg 30


#define NOTIFY_TOUCH_PRESS      0X01   //����������֪ͨ
#define NOTIFY_TOUCH_RELEASE  0X03  //�������ɿ�֪ͨ
#define NOTIFY_WRITE_FLASH_OK  0X0C  //дFLASH�ɹ�
#define NOTIFY_WRITE_FLASH_FAILD  0X0D  //дFLASHʧ��
#define NOTIFY_READ_FLASH_OK  0X0B  //��FLASH�ɹ�
#define NOTIFY_READ_FLASH_FAILD  0X0F  //��FLASHʧ��
#define NOTIFY_MENU                        0X14  //�˵��¼�֪ͨ
#define NOTIFY_TIMER                       0X43  //��ʱ����ʱ֪ͨ
#define NOTIFY_CONTROL                0XB1  //�ؼ�����֪ͨ
#define NOTIFY_SCREEN                   0XB2  //�����л�֪ͨ

enum CtrlType
{
	kCtrlUnknown=0x0,
	kCtrlButton=0x10,  //��ť
	kCtrlText,  //�ı�
	kCtrlProgress,  //������
	kCtrlSlider,    //������
	kCtrlMeter,  //�Ǳ�
	kCtrlDropList, //�����б�
	kCtrlAnimation, //����
	kCtrlRTC, //ʱ����ʾ
	kCtrlGraph, //����ͼ�ؼ�
	kCtrlTable, //���ؼ�
	kCtrlMenu,//�˵��ؼ�
	kCtrlSelector,//ѡ��ؼ�
	kCtrlQRCode,//��ά��
};
#define MAXWithTFTRX 100
typedef union
{
	struct
	{		
		uint8_t  cmd_head;  //֡ͷ
		uint8_t  cmd_type;  //��������(UPDATE_CONTROL)	
		uint8_t  ctrl_msg;   //CtrlMsgType-ָʾ��Ϣ������
		uint8_t  screen_id_H;  //������Ϣ�Ļ���ID
		uint8_t  screen_id_L;  //������Ϣ�Ļ���ID
		uint8_t  control_id_H;  //������Ϣ�Ŀؼ�ID
		uint8_t  control_id_L;  //������Ϣ�Ŀؼ�ID
		uint8_t  control_type; //�ؼ�����
		uint8_t  data[MAXWithTFTRX-8];
	};
	uint8_t All[MAXWithTFTRX];
}_TFTMsg;
_TFTMsg TFTMsg;
typedef union   
{
	struct
	{		
		unsigned bWithTFTRx:1;
		unsigned bLen:1;
	};
	uint8_t AllFlag;
} _TFTFlags;        // general flags
_TFTFlags TFTFlags;

#define GetTxtRetryMax 5
#define SetScreenRetryMax 5


uint16_t WithTFTRxLen;
uint16_t WithTFTRxCount;
uint16_t WithTFTNeedRxCount;

void InitWithTFT(void);
void EndProcCommandFromTFT(void);
void ProcRxFromTFT(uint8_t rx);
void ProcCommandFromTFT_Main(void);


void SetTxt(uint8_t sID,uint8_t cID,const char* data);
void GetTxt(uint8_t sID,uint8_t cID);
void SetChar(uint8_t sID,uint8_t cID,char data);
void SetTextFlash(uint8_t sID,uint8_t cID,uint16_t time);
void SetButton(uint8_t sID,uint8_t cID,uint8_t status);
void SetScreen(uint8_t sID);
void SetProgress(uint8_t sID,uint8_t cID,uint8_t status);
void DeleteCurve(uint8_t sID,uint8_t cID,uint8_t n);
void AddCurveData(uint8_t sID,uint8_t cID,uint8_t channel,uint16_t data);
void SetCurveVerticalScaling(uint8_t sID,uint8_t cID,uint16_t XScaling,uint16_t YScaling);
void ShowEnterDlg(const char *content);
uint8_t WithTFT_GetTxt(uint8_t sID,uint8_t cID,char* pBuf);
void ShowPic(uint8_t picID,uint16_t x,uint16_t y,uint16_t weight,uint16_t high);
void ShowQueryDlg(const char *content);
#endif
