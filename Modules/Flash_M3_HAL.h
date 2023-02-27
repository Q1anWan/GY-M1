#ifndef FLASH_M3_HAL_H
#define FLASH_M3_HAL_H
#include "main.h"
#define STM32_FLASH_BASE 0x08000000
#define STM32_FLASH_SIZE 32 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              	//使能FLASH写入(0，不是能;1，使能)
#define FLASH_WAITETIME  50000          	//FLASH等待超时时间//STM32 FLASH的起始地址

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif	

void Flash_Data_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead) ;
void Flash_Data_PageErase(uint32_t PageNum);
void Flash_Data_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);
#endif
