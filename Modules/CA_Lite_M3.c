/*****************************************************************
**                qianwan's Code
**---------------------------------------------------------------
** ��Ŀ���ƣ�   CA�����㷨�ഺ��
** ��    �ڣ�   2022-04-11
** ��    �ߣ�   qianwan
** ��    ����   1.1
**---------------------------------------------------------------
** �� �� ����   CA_Lite_M3.c
** ��������   arm_math.h
** �ļ����䣺   ����������ʽPID
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
** �� �� ���� InitPID()
** ����˵���� PID������ʼ��
**---------------------------------------------------------------------
** ��������� ��
** ���ز����� ��
***********************************************************************/
void InitPID(void)
{
	//�����Ǽ���
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
** �� �� ���� PIDCal()
** ����˵���� ��ͨPID���㺯��
**---------------------------------------------------------------------
** ��������� PID�����ṹ�塢����ֵ
** ���ز����� PID������
***********************************************************************/
int32_t PIDCal(PID_Data *pid, float fdb)
{
			float Kp =0.0f;
			float Ki =0.0f;
			float Kd =0.0f;
	switch(pid->type_selection)
		{
		case 0://����ʽ
			
			pid->FeedBack = fdb;
			//pid->Error = pid->Ref - pid->FeedBack;
			arm_sub_f32(&pid->Ref,&pid->FeedBack,&pid->Error,1);
			//pid->DError = pid->Error - pid->PreError;
			arm_sub_f32(&pid->Error,&pid->PreError,&pid->DError,1);
			//pid->DDError = pid->DError - pid->PreDError;
			arm_sub_f32(&pid->DError,&pid->PreDError,&pid->DDError,1);
			
			pid->PreError = pid->Error;
			
			pid->PreDError = pid->DError;
			
			//���趨��������������ʵ�ʲ�����
			//PID�ֿ�����
		
		//Pһֱ��		
			arm_mult_f32(&pid->Kp,&pid->DError,&Kp,1);			
		//I�����ϴ�ʱ�ر�	
			if((fabs(pid->Error)<pid->Ki_able_error_range)||!pid->Ki_able_error_range)
				{arm_mult_f32(&pid->Ki,&pid->Error,&Ki,1);}		
		//D������Сʱ�ر�	
			if((fabs(pid->Error)>pid->Kd_able_error_range)||!pid->Kd_able_error_range)
				{arm_mult_f32(&pid->Kd,&pid->DDError,&Kd,1);}
			
			//������
			arm_add_f32(&Ki,&Kd,&Ki,1);
			arm_add_f32(&pid->Out,&Kp,&pid->Out,1);
			arm_add_f32(&pid->Out,&Ki,&pid->Out,1);			
			//pid->Out += (pid->Kp * pid->DError + pid->Ki * pid->Error  + pid->Kd * pid->DDError );//�������޷�
			
			//����޷�
			if(pid->Out > pid->MaxOutValue)
			{pid->Out = (pid->MaxOutValue);}
			else if(pid->Out < pid->MinOutValue)
			{pid->Out = pid->MinOutValue;}			
		break;
	}
		
	return pid->Out;
}


/***********************************************************************
** �� �� ���� Usart_Printf()
** ����˵���� ���ڴ�ӡ����
**---------------------------------------------------------------------
** ��������� ���ھ����������Ϣ
** ���ز����� ��
***********************************************************************/
void Usart_Printf(UART_HandleTypeDef *huart,char *fmt, ...) //���ڷ���
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
** �� �� ���� Usart_Printf()
** ����˵���� ���ڴ�ӡ����
**---------------------------------------------------------------------
** ��������� ���ھ������������ֵ
** ���ز����� ��
***********************************************************************/
void imPtintf(UART_HandleTypeDef *huart, uint8_t name, float value)
{
  char buf[20];
  sprintf(buf, "Line%d=%f,", name, value);
  Usart_Printf(huart,buf);
}