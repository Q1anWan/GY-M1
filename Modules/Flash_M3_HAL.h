#ifndef FLASH_M3_HAL_H
#define FLASH_M3_HAL_H
#include "main.h"
#define STM32_FLASH_BASE 0x08000000
#define STM32_FLASH_SIZE 32 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              	//ʹ��FLASHд��(0��������;1��ʹ��)
#define FLASH_WAITETIME  50000          	//FLASH�ȴ���ʱʱ��//STM32 FLASH����ʼ��ַ

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif	

void Flash_Data_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead) ;
void Flash_Data_PageErase(uint32_t PageNum);
void Flash_Data_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);
#endif
