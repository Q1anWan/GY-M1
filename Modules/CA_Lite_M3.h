#ifndef __CA_H
#define __CA_H
/*****************************************************************
**                qianwan's Code
**---------------------------------------------------------------
** ��Ŀ���ƣ�   CA�����㷨�ഺ��
** ��    �ڣ�   2022-03-07
** ��    �ߣ�   qianwan
** ��    ����   1.1
**---------------------------------------------------------------
** �� �� ����   CA_Lite_M3.h
** ��������   arm_math.h
** �ļ����䣺   ����������ʽPID
*****************************************************************/
#include "main.h"

/*---------------------VARIABLES---------------------*/
typedef struct _PID_Data
{
	//�������
	uint8_t type_selection;
		
	float	Ref;
	float 	FeedBack;	
	float 	Error; //���
	float 	DError;
	float 	DDError;
	float 	PreError;
	float 	PreDError;
	
	float Ki_able_error_range;
	float Kd_able_error_range;
	
	float Kp; //pid����
	float Ki;
	float Kd;
	
	int32_t MaxOutValue; //����޷�
	int32_t MinOutValue;

	
	float Out; //���ֵ
	
}PID_Data;
/*---------------------DECLARES----------------------*/
void InitPID(void);
int32_t PIDCal(PID_Data *pid, float fdb);
extern PID_Data Pid_gyrotemp;
void Usart_Printf(UART_HandleTypeDef *huart,char *fmt, ...); //���ڷ���
void imPtintf(UART_HandleTypeDef *huart, uint8_t name, float value);
#endif
