#include "Control.h"
#include "CA_Lite_M3.h"
#include "BMI160_LL.h"
#include "CAN_M3_HAL.h"
#include "QCS_Lite.h"
Key_T KEY;
void KEY_Scan(Key_T *KEY)//200Hz执行
{
	if(KEY->Flag)
	{	
		static uint8_t T_250us_cnt;
		static uint8_t Key_temp  = 0x01;
		
		if ( ++T_250us_cnt >= 8 )//2ms IRQ
		{
			Key_temp = LL_GPIO_IsInputPinSet(KEY_GPIO_Port,KEY_Pin);
			switch(Key_temp)
			{
				case 0x00:
					KEY->Statue = 1;
					break;
				case 0x01:
					KEY->Statue = 0;
					break;
				default:
					break;					
			}
			T_250us_cnt = 0;				
			KEY->Flag=0;
			}
	}
}

void Pin_EXTI_Callback(uint32_t GPIO_Pin)
{
	if(GPIO_Pin == KEY_Pin) {//up
	KEY.Flag=1;}
}
void KEY_Operate(Key_T *KEY) //20Hz执行
{

}
void Tim_Callback(TIM_TypeDef *Timx)
{
	if(Timx == Tim_Control)
	{
		if(LL_TIM_IsActiveFlag_UPDATE(Tim_Control))LL_TIM_ClearFlag_UPDATE(Tim_Control);
		static uint16_t Pulse_1;
		if(++Pulse_1==1001)
		{
			Pulse_1=1;
			CAN_TX_FPS = CAN_TX_FPS_;
			CAN_TX_FPS_=0;
		}
//		if(Pulse_1%5==2)//200Hz按键扫描
//		{KEY_Scan(&KEY);}
		if(Pulse_1%5==4)//200Hz四元数发送
		{CAN_IMU_Translate(&hcan,QCS_AB_ID,QCS_AHRSq);CAN_TX_FPS_++;}
//		if(Pulse_1%50==7)//20Hz按键操作
//		{KEY_Operate(&KEY);}
		if(Pulse_1%100==37)//10Hz加热
		{Advanced_Heating();}
	}
}
