#include "Flash_M3_HAL.h"

//半字形式读写
uint16_t Flash_ReadHalfWord(uint32_t faddr)
{
	return *(volatile uint16_t*)faddr; 
}

/*
	从指定地址开始写入指定长度的数据
	WriteAddr:起始地址(此地址必须为2的倍数!!)
	pBuffer:数据指针
	NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
	注意：此读写程序不会检查扇区是否已被擦除	*/
void Flash_Data_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)
{
	//地址检查
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//非法地址
	
	HAL_FLASH_Unlock();					//解锁
	FLASH_WaitForLastOperation(FLASH_WAITETIME);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR);
	FLASH_WaitForLastOperation(FLASH_WAITETIME);
	for(uint16_t i=0;i<NumToWrite;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
	HAL_FLASH_Lock();
}

void Flash_Data_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	
	for(uint16_t i=0;i<NumToRead;i++)
	{
		pBuffer[i]=Flash_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}
extern void    FLASH_PageErase(uint32_t PageAddress); 
void Flash_Data_PageErase(uint32_t PageNum)
{
	//页检查
	if(PageNum > (STM32_FLASH_SIZE * 1024 / STM_SECTOR_SIZE)) return;//检查页编号

	HAL_FLASH_Unlock();					//解锁
	FLASH_WaitForLastOperation(FLASH_WAITETIME);
	FLASH_PageErase(PageNum*STM_SECTOR_SIZE+STM32_FLASH_BASE);
	FLASH_WaitForLastOperation(FLASH_WAITETIME);
	CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
	HAL_FLASH_Lock();

}
