/*************************************************************************************************************
 * �ļ���:	RTC.c
 * ����:		S3C6410 RTC�ײ���������
 * ����:		����
 * ����ʱ��:	2012��3��28��21:52
 * ����޸�ʱ��:2012��3��28��
 * ��ϸ:		RTC��صײ���������
*************************************************************************************************************/
#include "system.h"
#include "rtc.h"

Time_TypeDef Timer;//���ȫ��ʱ��

//�ڲ���̬��������
static bool uAutoTimeUpdate = FALSE;	//����Ƿ��Զ����жϸ���ʱ��
static bool uSecEnd = FALSE;			//�������־,������Ϳ��Ը���ʱ����


/*************************************************************************************************************************
*����        :	void RTC_Install(u8 ENABLE)
*����        :	RTC����ʹ����ʧ��
*����        :	Enable:ʹ��ʱ������,Disable:ʹ��ʱ������
*����        :	��
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120328
*����޸�ʱ��:	20120329
*˵��        :	ƽʱ������ʱӦ������ΪDisable
*************************************************************************************************************************/
void RTC_Install(u8 ENABLE)
{
	if(ENABLE)
		RTC->CON |=  BIT0;//RTCʱ������ʹ��
	else
		RTC->CON &=  ~BIT0;//RTCʱ������ʧ��
}

/*************************************************************************************************************************
*����        :	u8 BCD_to_DEC(u8 BCD_Code)
*����        :	BCD��תDEC��
*����        :	BCD��
*����        :	DEC��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	���ڽ�BCD��ʱ��ת��Ϊʮ����ʱ��,����Ϊ8bit
*************************************************************************************************************************/
u8 BCD_to_DEC(u8 BCD_Code)
{
	return ((BCD_Code >> 4) * 10 + (BCD_Code & 0x0f));
}


/*************************************************************************************************************************
*����        :	__inline u8 GetSec(void)
*����        :	��ȡ��
*����        :	��
*����        :	��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	��BCD���ȡ��ת��ΪDEC��
*************************************************************************************************************************/
__inline u8 GetSec(void)
{
	return (BCD_to_DEC(RTC->BCDSEC));//��ȡ��
}

/*************************************************************************************************************************
*����        :	__inline u8 GetMin(void)
*����        :	��ȡ��
*����        :	��
*����        :	��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	��BCD�ֻ�ȡ��ת��ΪDEC��
*************************************************************************************************************************/
__inline u8 GetMin(void)
{
	return (BCD_to_DEC(RTC->BCDMIN));//��ȡ��
}

/*************************************************************************************************************************
*����        :	__inline u8 GetHour(void)
*����        :	��ȡʱ
*����        :	��
*����        :	ʱ
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	��BCDʱ��ȡ��ת��ΪDECʱ
*************************************************************************************************************************/
__inline u8 GetHour(void)
{
	return (BCD_to_DEC(RTC->BCDHOUR));//��ȡʱ
}

/*************************************************************************************************************************
*����        :	__inline u8 GetWeek(void)
*����        :	��ȡ����
*����        :	��
*����        :	����
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	��BCD���ڻ�ȡ��ת��ΪDEC����,1-7:����һ-������
*************************************************************************************************************************/
__inline u8 GetWeek(void)
{
	return (BCD_to_DEC(RTC->BCDDAY));//��ȡ����
}


/*************************************************************************************************************************
*����        :	__inline u8 GetDate(void)
*����        :	��ȡ��
*����        :	��
*����        :	��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	��BCD�ջ�ȡ��ת��ΪDEC��
*************************************************************************************************************************/
__inline u8 GetDate(void)
{
	return (BCD_to_DEC(RTC->BCDDATE));//��ȡ��
}


/*************************************************************************************************************************
*����        :	__inline u8 GetMonth(void)
*����        :	��ȡ��
*����        :	��
*����        :	��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	��BCD�»�ȡ��ת��ΪDEC��
*************************************************************************************************************************/
__inline u8 GetMonth(void)
{
	return (BCD_to_DEC(RTC->BCDMON));//��ȡ��
}



/*************************************************************************************************************************
*����        :	__inline u8 GetYear(void)
*����        :	��ȡ��
*����        :	��
*����        :	��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	��BCD���ȡ��ת��ΪDEC��
*************************************************************************************************************************/
__inline u8 GetYear(void)
{
	return (BCD_to_DEC(RTC->BCDYEAR));//��ȡ��
}



/*************************************************************************************************************************
*����        :	void UpdateTimer(void)
*����        :	����ʱ��
*����        :	��
*����        :	��
*����        : 	�ײ㺯��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	��ʱ�������ȫ�ֵĽṹTimer��
*************************************************************************************************************************/
void UpdateTimer(void)
{
	Timer.w_year = 2000 + GetYear();	//������,���Ϊ2000��
	Timer.w_month = GetMonth();	//������
	Timer.w_date = GetDate();	//������
	Timer.week = GetWeek();		//��������
	Timer.hour = GetHour();		//����ʱ
	Timer.min = GetMin();		//���·�
	Timer.sec = GetSec();		//������
}


/*************************************************************************************************************************
*����        :	u8 DEC_to_BCD(u8 DEC_Code)
*����        :	DEC��תBCD��
*����        :	DEC��
*����        :	BCD��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	���ڽ�DEC��ʱ��ת��ΪBCDʱ��,����Ϊ8bit
*************************************************************************************************************************/
u8 DEC_to_BCD(u8 DEC_Code)
{
	u8 temp;
	
	if(DEC_Code > 99)
		return 0;
	temp = DEC_Code / 10;
	temp <<= 4;
	temp += DEC_Code % 10;
	return temp;
}


/*************************************************************************************************************************
*����        :	void SetSec(u8 DEC_Sec)
*����        :	������
*����        :	��
*����        :	��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	����ʱ��,ע����Ҫ�ȿ���д����,�����޷�д��
*************************************************************************************************************************/
void SetSec(u8 DEC_Sec)
{
	RTC->BCDSEC = DEC_to_BCD(DEC_Sec);//����ȡ��
}

/*************************************************************************************************************************
*����        :	void SetMin(u8 DEC_Min)
*����        :	���÷�
*����        :	��
*����        :	��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	����ʱ��,ע����Ҫ�ȿ���д����,�����޷�д��
*************************************************************************************************************************/
void SetMin(u8 DEC_Min)
{
	RTC->BCDMIN = DEC_to_BCD(DEC_Min);//���÷�
}

/*************************************************************************************************************************
*����        :	void SetHour(u8 DEC_Hour)
*����        :	����ʱ
*����        :	ʱ
*����        :	��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	����ʱ��,ע����Ҫ�ȿ���д����,�����޷�д��
*************************************************************************************************************************/
void SetHour(u8 DEC_Hour)
{
	RTC->BCDHOUR = DEC_to_BCD(DEC_Hour);//����ʱ
}

/*************************************************************************************************************************
*����        :	void SetWeek(u8 DEC_Week)
*����        :	��������
*����        :	����
*����        :	��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	����ʱ��,ע����Ҫ�ȿ���д����,�����޷�д��
*************************************************************************************************************************/
void SetWeek(u8 DEC_Week)
{
	RTC->BCDDAY = DEC_to_BCD(DEC_Week);//��������
}


/*************************************************************************************************************************
*����        :	void SetDate(u8 DEC_Date)
*����        :	������
*����        :	��
*����        :	��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	����ʱ��,ע����Ҫ�ȿ���д����,�����޷�д��
*************************************************************************************************************************/
void SetDate(u8 DEC_Date)
{
	RTC->BCDDATE = DEC_to_BCD(DEC_Date);//������
}


/*************************************************************************************************************************
*����        :	void SetMonth(u8 DEC_Month)
*����        :	������
*����        :	��
*����        :	��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	����ʱ��,ע����Ҫ�ȿ���д����,�����޷�д��
*************************************************************************************************************************/
void SetMonth(u8 DEC_Month)
{
	RTC->BCDMON = DEC_to_BCD(DEC_Month);//������
}



/*************************************************************************************************************************
*����        :	void SetYear(u8 DEC_Year)
*����        :	������
*����        :	��
*����        :	��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	����ʱ��,ע����Ҫ�ȿ���д����,�����޷�д��
*************************************************************************************************************************/
void SetYear(u8 DEC_Year)
{
	RTC->BCDYEAR = DEC_to_BCD(DEC_Year);//������
}



/*************************************************************************************************************************
*����        :	void InstallData(u16 Year,u8 Month,u8 Date,u8 Week)
*����        :	��������
*����        :	��,��,��,����
*����        :	��
*����        : 	�ײ㺯��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	���û��Զ�ȥ��д����,�귶ΧΪ2000-2099
*************************************************************************************************************************/
void InstallData(u16 Year,u8 Month,u8 Date,u8 Week)
{
	RTC_Install(Enable);	//��������ʱ��
	SetYear(Year-2000);			//������
	SetMonth(Month);		//������
	SetDate(Date);			//������
	SetWeek(Week);			//��������
	RTC_Install(Disable);	//��ֹ����ʱ��
}


/*************************************************************************************************************************
*����        :	void InstallTime(u8 Hour,u8 Min,u8 Sec)
*����        :	����ʱ��
*����        :	ʱ,��,��
*����        :	��
*����        : 	�ײ㺯��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120329
*˵��        :	���û��Զ�ȥ��д����
*************************************************************************************************************************/
void InstallTime(u8 Hour,u8 Min,u8 Sec)
{
	RTC_Install(Enable);	//��������ʱ��
	SetHour(Hour);			//����ʱ
	SetMin(Min);			//���÷�
	SetSec(Sec);			//������
	RTC_Install(Disable);	//��ֹ����ʱ��
}


/*************************************************************************************************************************
*����        :	__inline ClearIntP(void)
*����        :	���INTP�жϱ�־
*����        :	��
*����        :	��
*����        : 	�ײ㺯��
*����        :	����
*ʱ��        :	20120331
*����޸�ʱ��:	20120331
*˵��        :	��
*************************************************************************************************************************/
__inline ClearIntP(void)
{
	RTC->INTP |= BIT0;	//д1���TIC�ж�
}

/*************************************************************************************************************************
*����        :	bool RTC_SecEnd(void)
*����        :	�ж����Ƿ񵽴�
*����        :	��
*����        :	TRUE:�뵽��;FALSE:δ����
*����        : 	�ײ㺯��
*����        :	����
*ʱ��        :	20120331
*����޸�ʱ��:	20120331
*˵��        :	��
*************************************************************************************************************************/
bool RTC_SecEnd(void)
{
	if(uAutoTimeUpdate == TRUE)	//�������Զ�����ʱ��
	{
		if(uSecEnd == TRUE)		//�����
		{
			uSecEnd = FALSE;	//�����־
			return TRUE;
		}
		else
			return FALSE;
	}
	if(RTC->INTP & BIT0)	//TIC�жϱ�־��λ
	{
		ClearIntP();		//����жϱ�־
		return TRUE;
	}
	return FALSE;
}



//TIC�жϷ������,�����Զ�����ʱ��
void __irq Isr_RTC_InspectTick(void)
{
	uSecEnd = TRUE;					//�����ʱ�����
	UpdateTimer();					//��������ʱ��
	ClearIntP();					//����жϱ�־
	VICInterruptEnd();	//�ж��˳�
}




/*************************************************************************************************************************
*����        :	void RTC_Init(u8 AutoUpdate)
*����        :	��ʼ��RTC
*����        :	EnableInt:�Ƿ����ж�
*����        :	��
*����        : 	��
*����        :	����
*ʱ��        :	20120329
*����޸�ʱ��:	20120331
*˵��        :	�ж�ʱ���Ƿ�д����,���û,��д��������,������ʱ��
*************************************************************************************************************************/
void RTC_Init(FunctionalState EnableInt)
{
	RTC->TICNT = 32768-1;
	RTC->CON = BIT8;		//ʹ�ܱ�Ƕ�ʱ��,32768��Ƶ
	RTC_Install(Disable);	//����ʱ��д����
	UpdateTimer();			//��������ʱ��
	if(EnableInt == ENABLE)			//ʹ���Զ�����ʱ��
	{
		uAutoTimeUpdate = TRUE;//��־��Ч
		Set_IsrAddr(INT_RTC_TIC,(u32)Isr_RTC_InspectTick);//�����ж�������ַ
		Set_IntEnable(INT_RTC_TIC,ENABLE);//ʹ��TIC�ж�
	}
	else
	{
		uAutoTimeUpdate = FALSE;
		Set_IntEnable(INT_RTC_TIC,DISABLE);//�ر�TIC�ж�
	}
}














