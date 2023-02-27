#ifndef CAN_M3_HAL_H
#define CAN_M3_HAL_H
#include "main.h"
#include "can.h"

#define BMI160_SETRecieve_ID 0x320
#define BMI160_SETTranslate_ID 0x321
#define QCS_AB_ID 0x322
#define QCS_CD_ID 0x323
void CAN_filter_init(void);
void CAN_IMU_Translate(CAN_HandleTypeDef *hcan, uint32_t identifierAB, float *Quaternion);


#endif
