#include "cmd_process.h"
#include <stdio.h>
#include "TFTWorkProc.h"
#include "..\WirelessProc.h"
#include "WorkMode_RealTime.h"
#include "WorkMode_History.h"
#include "WorkMode_IntervalAnalysis.h"
#include "WorkMode_Setting.h"
#include "WorkMode_DataAnalysis.h"

uint8 cmd_buffer[CMD_MAX_SIZE];
uint16_t waitSID;
uint16_t waitCID;
uint8_t controlValue[50];


extern uint32_t SleepSecond;
/*! 
 *  \brief  ��Ϣ�������̣��˴�һ�㲻��Ҫ����
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void ProcessMessage( PCTRL_MSG msg, uint16 size )
{
	uint8 cmd_type = msg->cmd_type;//ָ������
	uint8 ctrl_msg = msg->ctrl_msg;   //��Ϣ������
	uint8 control_type = msg->control_type;//�ؼ�����
	uint16 screen_id = PTR2U16(&msg->screen_id_high);//����ID
	uint16 control_id = PTR2U16(&msg->control_id_high);//�ؼ�ID
	uint32 value = PTR2U32(msg->param);//��ֵ
	SleepSecond=0;
	switch(cmd_type)
	{
	case NOTIFY_REST://��λ
		if(TFTWaitCommandFlags.bWaitReset)//������λ
		{
			TFTWaitCommandFlags.bWaitReset=0;
			TFTNotifyCommandFlags.bRxReset=1;
		}
		else
			NVIC_SystemReset();
		break;
	case NOTIFY_CONTROL://�ؼ��¼�
		{
			if(ctrl_msg==MSG_GET_CURRENT_SCREEN)//����ID�仯֪ͨ
			{
				NotifyScreen(screen_id);
			}
			else
			{
				switch(control_type)
				{
				case kCtrlButton: //��ť�ؼ�
					NotifyButton(screen_id,control_id,msg->param[1]);
					break;
				case kCtrlText://�ı��ؼ�
					NotifyText(screen_id,control_id,msg->param);
					break;
				case kCtrlProgress: //�������ؼ�
					NotifyProgress(screen_id,control_id,value);
					break;
				case kCtrlSlider: //�������ؼ�
					NotifySlider(screen_id,control_id,value);
					break;
				case kCtrlMeter: //�Ǳ�ؼ�
					NotifyMeter(screen_id,control_id,value);
					break;
				case kCtrlMenu://�˵��ؼ�
					NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
					break;
				case kCtrlSelector://ѡ��ؼ�
					NotifySelector(screen_id,control_id,msg->param[0]);
					break;
				case kCtrlRTC://����ʱ�ؼ�
					NotifyTimer(screen_id,control_id);
					break;
				default:
					break;
				}
			}			
		}
		break;
	case NOTIFY_TOUCH_PRESS://����������
	case NOTIFY_TOUCH_RELEASE://�������ɿ�
		NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4));
		break;	
	case NOTIFY_WRITE_FLASH_OK://дFLASH�ɹ�
		NotifyWriteFlash(1);
		break;
	case NOTIFY_WRITE_FLASH_FAILD://дFLASHʧ��
		NotifyWriteFlash(0);
		break;
	case NOTIFY_READ_FLASH_OK://��ȡFLASH�ɹ�
		NotifyReadFlash(1,cmd_buffer+2,size-6);//ȥ��֡ͷ֡β
		break;
	case NOTIFY_READ_FLASH_FAILD://��ȡFLASHʧ��
		NotifyReadFlash(0,0,0);
		break;
	case NOTIFY_READ_RTC://��ȡRTCʱ��
		NotifyReadRTC(cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7],cmd_buffer[8]);
		break;	
	default:
		break;
	}
}

/*! 
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
 *  \param screen_id ��ǰ����ID
 */
void NotifyScreen(uint16 screen_id)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ���������¼���Ӧ
 *  \param press 1���´�������3�ɿ�������
 *  \param x x����
 *  \param y y����
 */
void NotifyTouchXY(uint8 press,uint16 x,uint16 y)
{
	//TODO: ����û�����
}

//�ı��ؼ���ʾ����ֵ
void SetTextValueInt32(uint16 screen_id, uint16 control_id,int32 value)
{
	char buffer[12] = {0};
	sprintf(buffer,"%ld",value);
	SetTextValue(screen_id,control_id,(uchar *)buffer);
}

//�ַ���ת����
int32 StringToInt32(uint8 *str)
{
	int32 v = 0;
	sscanf((char *)str,"%ld",&v);
	return v;
}

/*! 
 *  \brief  ��ť�ؼ�֪ͨ
 *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0����1����
 */
void NotifyButton(uint16 screen_id, uint16 control_id, uint8  state)
{
	//TODO: ����û�����	
	//��������ť�ؼ�
	if(JumpButtonProc(control_id)!=0)
		return;
	switch(screen_id)
	{
	case 4://ʵʱ�¶�����ͼ
		if(control_id==cID_btBackward || control_id==cID_btForward)//����ͼ��ҳ
		{
			RealTime_TBarChartsSwitchPage(control_id);
		}
		break;
	case 5://ʵʱѹ������ͼ
		break;
	case 6://��ʷ
		if(control_id==cID_btConfirm)//ȷ������
		{
			HistoryModeInit();
		}
		break;
	case 7://��ʷ�¶��б�
		if(control_id==cID_btBackward || control_id==cID_btForward)//����ͼ��ҳ
		{
			History_TRecordSwitchPage(control_id);
		}
		break;
	case 9://��ʷ�¶�����ͼ
		if(control_id==cID_btBackward || control_id==cID_btForward)//����ͼ��ҳ
		{
			History_TBarChartsSwitchPage(control_id);
		}
		break;
	case 11://��������ƽ���¶ȡ��б�
		if(control_id==cID_btBackward || control_id==cID_btForward)//����ͼ��ҳ
		{
			DataAnalysis_DA_TRecordSwitchPage(control_id);
		}
		break;
	case 13://��������Ԫ�¶ȡ��б�
		if(control_id==cID_btBackward || control_id==cID_btForward)//����ͼ��ҳ
		{
			DataAnalysis_DY_TRecordSwitchPage(control_id);
		}
		break;
	case 15://��������ƽ���¶ȡ�����ͼ
		if(control_id==cID_btBackward || control_id==cID_btForward)//����ͼ��ҳ
		{
			DataAnalysis_DA_TBarChartsSwitchPage(control_id);
		}
		break;
	case 17://��������Ԫ�¶ȡ�����ͼ
		if(control_id==cID_btBackward || control_id==cID_btForward)//����ͼ��ҳ
		{
			DataAnalysis_DY_TBarChartsSwitchPage(control_id);
		}
		break;
	case 19://�¶ȼ������
		if(control_id==cID_btBackward || control_id==cID_btForward)//����ͼ��ҳ
		{
			IntervalAnalysis_TRecordSwitchPage(control_id);
		}
		break;
	case 25://���ã�ʱ����
		if(control_id==cID_btConfirm)//ȷ������
		{
			Setting_IntervalAnalysis();
		}
		break;
	case 26://���ã�����ʱ��
		if(control_id==cID_btConfirm)
		{
			Setting_SleepTimesSpan();
		}
		break;
	case 27://���ã��ն˱���
		if(control_id==cID_btConfirm)
		{
			Setting_SensorProgram();
		}
		else if(control_id==41)//���ؼ�
		{
			SetScreen(21);
		}
		break;
	case 29:
	case 28://�Ի���
		if(control_id==100)
		{
			StopTimer(screen_id,cID_Timer);
			SetScreen(autoReturnSID);
		}
		break;
	}
}

/*! 
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param str �ı��ؼ�����
 */
void NotifyText(uint16 screen_id, uint16 control_id, uint8 *str)
{
	//TODO: ����û�����
	uint32_t i; 
	if(!TFTWaitCommandFlags.bWaitControlValue)
		return;
	if(screen_id!=waitSID)
		return;
	if(control_id!=waitCID)
		return;
	i=0;
	while(str[i]!='\0')
	{
		controlValue[i]=str[i];
		i++;
		if(i==(ControlValue_MAX_SIZE-1))
		{
			break;
		}
	}
	controlValue[i]='\0';
	TFTNotifyCommandFlags.bRxControlValue=1;
	TFTWaitCommandFlags.bWaitControlValue=0;
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyProgress(uint16 screen_id, uint16 control_id, uint32 value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifySlider(uint16 screen_id, uint16 control_id, uint32 value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �Ǳ�ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyMeter(uint16 screen_id, uint16 control_id, uint32 value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �˵��ؼ�֪ͨ
 *  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item �˵�������
 *  \param state ��ť״̬��0�ɿ���1����
 */
void NotifyMenu(uint16 screen_id, uint16 control_id, uint8  item, uint8  state)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ѡ��ؼ�֪ͨ
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item ��ǰѡ��
 */
void NotifySelector(uint16 screen_id, uint16 control_id, uint8  item)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ʱ����ʱ֪ͨ����
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void NotifyTimer(uint16 screen_id, uint16 control_id)
{
	//TODO: ����û�����
	switch(screen_id)
	{
	case 28:
	case 29://�Զ�����
		SetScreen(autoReturnSID);
		break;
	}
}

/*! 
 *  \brief  ��ȡ�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 *  \param _data ��������
 *  \param length ���ݳ���
 */
void NotifyReadFlash(uint8 status,uint8 *_data,uint16 length)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  д�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 */
void NotifyWriteFlash(uint8 status)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ȡRTCʱ�䣬ע�ⷵ�ص���BCD��
 *  \param year �꣨BCD��
 *  \param month �£�BCD��
 *  \param week ���ڣ�BCD��
 *  \param day �գ�BCD��
 *  \param hour ʱ��BCD��
 *  \param minute �֣�BCD��
 *  \param second �루BCD��
 */
void NotifyReadRTC(uint8 year,uint8 month,uint8 week,uint8 day,uint8 hour,uint8 minute,uint8 second)
{
	if(!TFTWaitCommandFlags.bWaitRTC)
		return;
	RTCData.year=year;
	RTCData.month=month;
	RTCData.week=week;
	RTCData.day=day;
	RTCData.hour=hour;
	RTCData.minute=minute;
	RTCData.second=second;
	TFTNotifyCommandFlags.bRxRTC=1;
	TFTWaitCommandFlags.bWaitRTC=0;
}
