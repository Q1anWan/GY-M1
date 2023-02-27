#ifndef __CA_H
#define __CA_H
/*****************************************************************
**                qianwan's Code
**---------------------------------------------------------------
** 项目名称：   CA控制算法青春版
** 日    期：   2022-03-07
** 作    者：   qianwan
** 版    本：   1.1
**---------------------------------------------------------------
** 文 件 名：   CA_Lite_M3.h
** 代码需求：   arm_math.h
** 文件补充：   仅保留增量式PID
*****************************************************************/
#include "main.h"

/*---------------------VARIABLES---------------------*/
typedef struct _PID_Data
{
	//类别设置
	uint8_t type_selection;
		
	float	Ref;
	float 	FeedBack;	
	float 	Error; //误差
	float 	DError;
	float 	DDError;
	float 	PreError;
	float 	PreDError;
	
	float Ki_able_error_range;
	float Kd_able_error_range;
	
	float Kp; //pid参数
	float Ki;
	float Kd;
	
	int32_t MaxOutValue; //输出限幅
	int32_t MinOutValue;

	
	float Out; //输出值
	
}PID_Data;
/*---------------------DECLARES----------------------*/
void InitPID(void);
int32_t PIDCal(PID_Data *pid, float fdb);
extern PID_Data Pid_gyrotemp;
void Usart_Printf(UART_HandleTypeDef *huart,char *fmt, ...); //串口发送
void imPtintf(UART_HandleTypeDef *huart, uint8_t name, float value);
#endif
