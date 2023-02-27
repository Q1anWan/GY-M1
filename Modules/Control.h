#ifndef CONTROL_H
#define CONTROL_H
#include "main.h"
#define Tim_Control TIM1
typedef struct
{
	uint8_t Flag;
	uint8_t Statue;
	uint8_t operte_mode;
}Key_T;
extern Key_T KEY;
void Pin_EXTI_Callback(uint32_t GPIO_Pin);
void Tim_Callback(TIM_TypeDef *Timx);
#endif
