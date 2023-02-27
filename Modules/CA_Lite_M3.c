/*****************************************************************
**                qianwan's Code
**---------------------------------------------------------------
** 项目名称：   CA控制算法青春版
** 日    期：   2022-04-11
** 作    者：   qianwan
** 版    本：   1.1
**---------------------------------------------------------------
** 文 件 名：   CA_Lite_M3.c
** 代码需求：   arm_math.h
** 文件补充：   仅保留增量式PID
*****************************************************************/

/*---------------------INCLUDES----------------------*/
#include "stdio.h"
#include "CA_Lite_M3.h"
#include "arm_math.h"
#include "BMI160_LL.h"
#include "tim.h"
#include "string.h"
#include "stdarg.h"
/*---------------------VARIABLES---------------------*/
PID_Data Pid_gyrotemp;

/***********************************************************************
** 函 数 名： InitPID()
** 函数说明： PID参数初始化
**---------------------------------------------------------------------
** 输入参数： 无
** 返回参数： 无
***********************************************************************/
void InitPID(void)
{
	//陀螺仪加热
			Pid_gyrotemp.type_selection = 0;
			Pid_gyrotemp.Ref =4500.0f;
			Pid_gyrotemp.FeedBack = 0.0f;
			Pid_gyrotemp.Error = 0.0f ;
			Pid_gyrotemp.DError = 0.0f ;
			Pid_gyrotemp.DDError = 0.0f ;
			Pid_gyrotemp.PreError = 0.0f ;
			Pid_gyrotemp.PreDError = 0.0f ;
			Pid_gyrotemp.Kp = 10.0f;
			Pid_gyrotemp.Ki = 0.20f;
			Pid_gyrotemp.Kd = 0.01f;
			Pid_gyrotemp.Ki_able_error_range = 200.0f;//100.0f;
			Pid_gyrotemp.Kd_able_error_range = 100.0f;//50.0f;
			Pid_gyrotemp.MaxOutValue = 7200.0f;
			Pid_gyrotemp.MinOutValue = 800.0f;
			Pid_gyrotemp.Out = 0.0f;
}

/***********************************************************************
** 函 数 名： PIDCal()
** 函数说明： 普通PID运算函数
**---------------------------------------------------------------------
** 输入参数： PID参数结构体、反馈值
** 返回参数： PID运算结果
***********************************************************************/
int32_t PIDCal(PID_Data *pid, float fdb)
{
			float Kp =0.0f;
			float Ki =0.0f;
			float Kd =0.0f;
	switch(pid->type_selection)
		{
		case 0://增量式
			
			pid->FeedBack = fdb;
			//pid->Error = pid->Ref - pid->FeedBack;
			arm_sub_f32(&pid->Ref,&pid->FeedBack,&pid->Error,1);
			//pid->DError = pid->Error - pid->PreError;
			arm_sub_f32(&pid->Error,&pid->PreError,&pid->DError,1);
			//pid->DDError = pid->DError - pid->PreDError;
			arm_sub_f32(&pid->DError,&pid->PreDError,&pid->DDError,1);
			
			pid->PreError = pid->Error;
			
			pid->PreDError = pid->DError;
			
			//把设定参数赋到处理后的实际参数上
			//PID分开运算
		
		//P一直开		
			arm_mult_f32(&pid->Kp,&pid->DError,&Kp,1);			
		//I在误差较大时关闭	
			if((fabs(pid->Error)<pid->Ki_able_error_range)||!pid->Ki_able_error_range)
				{arm_mult_f32(&pid->Ki,&pid->Error,&Ki,1);}		
		//D在误差较小时关闭	
			if((fabs(pid->Error)>pid->Kd_able_error_range)||!pid->Kd_able_error_range)
				{arm_mult_f32(&pid->Kd,&pid->DDError,&Kd,1);}
			
			//加起来
			arm_add_f32(&Ki,&Kd,&Ki,1);
			arm_add_f32(&pid->Out,&Kp,&pid->Out,1);
			arm_add_f32(&pid->Out,&Ki,&pid->Out,1);			
			//pid->Out += (pid->Kp * pid->DError + pid->Ki * pid->Error  + pid->Kd * pid->DDError );//做积分限幅
			
			//输出限幅
			if(pid->Out > pid->MaxOutValue)
			{pid->Out = (pid->MaxOutValue);}
			else if(pid->Out < pid->MinOutValue)
			{pid->Out = pid->MinOutValue;}			
		break;
	}
		
	return pid->Out;
}


/***********************************************************************
** 函 数 名： Usart_Printf()
** 函数说明： 串口打印程序
**---------------------------------------------------------------------
** 输入参数： 串口句柄，发送信息
** 返回参数： 无
***********************************************************************/
void Usart_Printf(UART_HandleTypeDef *huart,char *fmt, ...) //串口发送
{
  uint8_t tx_buf[40] = {0};
  va_list ap;
  uint16_t len;
  va_start(ap, fmt);

  len = vsprintf((char *)tx_buf, fmt, ap);
  va_end(ap);
  HAL_UART_Transmit(huart,tx_buf,len,1000);
}
/***********************************************************************
** 函 数 名： Usart_Printf()
** 函数说明： 串口打印程序
**---------------------------------------------------------------------
** 输入参数： 串口句柄，曲线名，值
** 返回参数： 无
***********************************************************************/
void imPtintf(UART_HandleTypeDef *huart, uint8_t name, float value)
{
  char buf[20];
  sprintf(buf, "Line%d=%f,", name, value);
  Usart_Printf(huart,buf);
}