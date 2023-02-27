#include "Flash_M3_HAL.h"

//������ʽ��д
uint16_t Flash_ReadHalfWord(uint32_t faddr)
{
	return *(volatile uint16_t*)faddr; 
}

/*
	��ָ����ַ��ʼд��ָ�����ȵ�����
	WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
	pBuffer:����ָ��
	NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
	ע�⣺�˶�д���򲻻��������Ƿ��ѱ�����	*/
void Flash_Data_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)
{
	//��ַ���
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	
	HAL_FLASH_Unlock();					//����
	FLASH_WaitForLastOperation(FLASH_WAITETIME);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR);
	FLASH_WaitForLastOperation(FLASH_WAITETIME);
	for(uint16_t i=0;i<NumToWrite;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
	HAL_FLASH_Lock();
}

void Flash_Data_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	
	for(uint16_t i=0;i<NumToRead;i++)
	{
		pBuffer[i]=Flash_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}
extern void    FLASH_PageErase(uint32_t PageAddress); 
void Flash_Data_PageErase(uint32_t PageNum)
{
	//ҳ���
	if(PageNum > (STM32_FLASH_SIZE * 1024 / STM_SECTOR_SIZE)) return;//���ҳ���

	HAL_FLASH_Unlock();					//����
	FLASH_WaitForLastOperation(FLASH_WAITETIME);
	FLASH_PageErase(PageNum*STM_SECTOR_SIZE+STM32_FLASH_BASE);
	FLASH_WaitForLastOperation(FLASH_WAITETIME);
	CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
	HAL_FLASH_Lock();

}
