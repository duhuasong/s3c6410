/*************************************************************************************************************
 * 文件名:	delay.c
 * 功能:		延时函数,调用定时器1进行延时的
 * 作者:		cp1300@139.com
 * 创建时间:	2012年9月17日22:32
 * 最后修改时间:2012年9月17日
 * 详细:		延时误差1-2us,使用定时器1
 * 			不能在中断服务程序中使用
*************************************************************************************************************/
#include "system.h"
#include "timer.h"
#include "delay.h"


/*************************************************************************************************************************
*函数        :	void Delay_US(u32 Delay)
*功能        :	US延时
*参数        :	Delay:延时时间,单位US
*返回        :	无
*依赖	: 	底层宏定义
*作者        :	cp1300@139.com
*时间     :	20120917
*最后修改时间:	20120917
*说明        :调用定时器0进行延时的,那么定时器0就不能使用了,延时误差1US
*************************************************************************************************************************/
void Delay_US(u32 Delay)
{
	rTCFG0 |= 65;			//定时器0预分频65+1,由PCLK=66提供时钟,66分频产生1MHz的定时器时钟
	rTCON &= (~0xff);		//清除设置
	rTCON |= BIT3;			//定时器0自动更新使能
	rTCNTB0 = Delay;		//重装值
	rTINT_CSTAT |= BIT5;	//清除中断标志
	rTINT_CSTAT &= ~BIT0;	//关闭定时器0中断
	//以下操作启动定时器0
	rTCON |= BIT1;			//手动更新
	rTCON &= ~BIT1;			//结束手动更新
	rTCON |= BIT0;			//启动定时器0	
	while(!(rTINT_CSTAT & BIT5));	//等待延时到达
	rTINT_CSTAT |= BIT5;	//清除中断标志
}



/*************************************************************************************************************************
*函数        :	void Delay_MS(u32 Delay)
*功能        :	MS延时
*参数        :	Delay:延时时间,单位US
*返回        :	无
*依赖	: 	底层宏定义
*作者        :	cp1300@139.com
*时间     :	20120918
*最后修改时间:	20120918
*说明        :调用定时器0进行延时的,那么定时器0就不能使用了,延时误差<1ms
*************************************************************************************************************************/
void Delay_MS(u32 Delay)
{
	while(Delay --)
	{
		Delay_US(1000);
	}
}
