#include "CAN_M3_HAL.h"
#include "Data_Exchange.h"
void CAN_filter_init(void)
{
	CAN_FilterTypeDef can_filter_st;
	can_filter_st.FilterActivation = ENABLE;
	can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
	can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
	can_filter_st.FilterIdHigh = 0x0000;
	can_filter_st.FilterIdLow = 0x0000;
	can_filter_st.FilterMaskIdHigh = 0x0000;
	can_filter_st.FilterMaskIdLow = 0x0000;
	can_filter_st.FilterBank = 0;
	can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
	HAL_CAN_ConfigFilter(&hcan, &can_filter_st);
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	can_filter_st.SlaveStartFilterBank = 14;
	can_filter_st.FilterBank = 14;
	HAL_CAN_ConfigFilter(&hcan, &can_filter_st);
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rx_header;
	uint8_t rx_data[8];
	uint8_t i;

	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

	
		switch (rx_header.StdId)
		{
			case BMI160_SETRecieve_ID:
				
			break;
			
			default:
				break;
		}
}

void CAN_IMU_Translate(CAN_HandleTypeDef *hcan, uint32_t identifierAB, float *Quaternion)
{
	uint8_t i =0;
	for(uint8_t i=0;i<2;i++){
	CAN_TxHeaderTypeDef tx_header;
	uint8_t tx_data[8];
	uint32_t tx_mail_box=0;

	tx_header.StdId = identifierAB+i;
	tx_header.ExtId = identifierAB+i;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_DATA;
	tx_header.DLC = 0x08;

	Transform.Float_To_U8(Quaternion+2*i,tx_data,2);
	while(HAL_CAN_IsTxMessagePending(hcan,tx_mail_box));		
	HAL_CAN_AddTxMessage(hcan, &tx_header, tx_data, &tx_mail_box);
	}
}


typedef struct _PID_Data
{
	float Quaternion[4];
	float Recieve_Buff[4];
	uint8_t Data_Syn;
	
}IMU_quat_Recieved_t;
//S
/*

*/
void IMU_Recieve_Translate(uint32_t identifier, uint8_t *rx_data, IMU_quat_Recieved_t *IMU_quat_Recieved)
{
	uint32_t OffSet;
	static uint32_t OffSet_Last;
	OffSet = identifier - QCS_AB_ID;
	Transform.U8_To_Float(rx_data,IMU_quat_Recieved->Recieve_Buff+2*OffSet,2);
	if(OffSet_Last == 0 && OffSet==1)
	{
		IMU_quat_Recieved->Data_Syn=1;
		IMU_quat_Recieved->Quaternion[0] = IMU_quat_Recieved->Recieve_Buff[0];
		IMU_quat_Recieved->Quaternion[1] = IMU_quat_Recieved->Recieve_Buff[1];
		IMU_quat_Recieved->Quaternion[2] = IMU_quat_Recieved->Recieve_Buff[2];
		IMU_quat_Recieved->Quaternion[3] = IMU_quat_Recieved->Recieve_Buff[3];
	}
	else{IMU_quat_Recieved->Data_Syn=0;}
	OffSet_Last = OffSet;
}