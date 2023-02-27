/*
	SPI LayLayer支持库
	  千万 2022.6.21
*/
#include "SPI_LL.h"
#if defined (SPI1) || defined (SPI2) || defined (SPI3) || defined (SPI4)
/*
	初始化SPI接口
	SPI结构体、片选信号端口、片选引脚
*/
void SPI_C::Init(SPI_TypeDef  *SPI, GPIO_TypeDef *CS_Port, uint32_t CS_Pin)
{
	this->SPI = SPI;
	this->CS_Port = CS_Port;
	this->CS_Pin  = CS_Pin;
	LL_SPI_Enable(this->SPI);
}

/*
	片选信号有效
	一般是置低电平
*/
void SPI_C::CS_Enable(void)
{
	LL_GPIO_ResetOutputPin(this->CS_Port,this->CS_Pin);
}

/*
	片选信号无效
	一般是置高电平
*/
void SPI_C::CS_Disable(void)
{
	LL_GPIO_SetOutputPin(this->CS_Port,this->CS_Pin);
}

/*
	SPI数据交换
	发一字节收一字节
*/
uint8_t SPI_C::Exchange_Byte(uint8_t byte)
{
	uint8_t wait_cnt=0;
 	while(!LL_SPI_IsActiveFlag_TXE(this->SPI))        //等待数据发送完成
	{
		if(wait_cnt++ >this->OutTime)
			break;
	}
	LL_SPI_TransmitData8(this->SPI, byte);            //发送8位数据
	wait_cnt = 0;
	while(!LL_SPI_IsActiveFlag_RXNE(this->SPI))       //等待接收缓存非空
	{
		if(wait_cnt++ >this->OutTime)
			break;
	}
	return LL_SPI_ReceiveData8(this->SPI);            //返回SPI2接收到的数据
}

/*
	SPI寄存器读取
	发送信息帧与空帧
	接收空帧与信息帧
*/
uint8_t	SPI_C::Read_Reg(uint8_t Reg)
{
	uint8_t Data = this->blank_reg;
	Reg |= 0x80;
	this->CS_Enable();
	this->Exchange_Byte(Reg);
	Data = Exchange_Byte(Data);
	this->CS_Disable();
	return Data;
}

/*
	SPI寄存器连续读取
	发送头信息帧与num个空帧
	接收头空帧与num个信息帧
*/
void SPI_C::Read_Muti_Reg(uint8_t reg, uint16_t num, uint8_t *pdata)
{
	reg |= 0x80;
	this->CS_Enable();
	this->Exchange_Byte(reg);
	for(uint16_t i = 0; i < num; i++)
	{
		pdata[i] = this->Exchange_Byte(this->blank_reg);
	}
	this->CS_Disable();
}

/*
	SPI寄存器写入
	发送两个信息帧
	接收空帧数和信息帧
*/
uint8_t	SPI_C::Write_Reg(uint8_t Reg,uint8_t data)
{
	this->CS_Enable();
	Reg = this->Exchange_Byte(Reg);
	this->Exchange_Byte(data);
	this->CS_Disable();
	return Reg;
}

/*
	SPI信息发送
	发送size个信息帧
	返回最后一个信息帧数
	发送中接收到的数据将不进行读取
*/
uint8_t SPI_C::MesgTransmit(uint8_t *pdata,uint32_t size)
{
	/*校核数据*/
	if ((pdata == NULL) || (size == 0)){return this->blank_reg;}
	this->CS_Enable();
	/*数据发送*/
	for(uint32_t i=0 ;i<size;i++){
	uint8_t wait_cnt=0;
 	while(!LL_SPI_IsActiveFlag_TXE(this->SPI))        //等待数据发送完成
	{
		if(wait_cnt++ >this->OutTime)
		break;
	}
	LL_SPI_TransmitData8(this->SPI, pdata[i]);            //发送8位数据
	}
	/*清除发送过程中不接受数据导致的溢出*/
	this->CS_Disable();
	LL_SPI_ClearFlag_OVR(this->SPI);
	return LL_SPI_ReceiveData8(this->SPI);
}

/*启动SPI DMA时*/
#if defined (DMA1) || defined (DMA2) || defined (DMA3)
/*
	DMA中断回调函数
	清除数据传输完成中断并置标志位
*/
void SPI_C::DMA_IQR_Handel(void)
{
	LL_DMA_DisableIT_TC(this->DMAx,this->DMA_Channelx);
	this->DMA_DisableCH();
	this->CS_Disable();
	this->DMA_Clear_TC();
	LL_SPI_ClearFlag_OVR(this->SPI);
	IsDMATransmitOK = 0;
}
/*
	DMA连续数据发送
*/
void SPI_C::MesgTransmit_DMA(uint8_t *pdata,uint32_t size)
{
	/*校核数据*/
	if ((pdata == NULL) || (size == 0)){return;}
	this->CS_Enable();
	LL_DMA_SetPeriphAddress(this->DMAx,this->DMA_Channelx,(uint32_t)&this->SPI->DR);
	LL_DMA_SetMemoryAddress(this->DMAx,this->DMA_Channelx,(uint32_t)pdata);
	LL_DMA_SetDataLength(this->DMAx,this->DMA_Channelx,size);
	LL_DMA_EnableIT_TC(this->DMAx,this->DMA_Channelx);
	LL_SPI_EnableDMAReq_TX(this->SPI);
	this->DMA_EnableCH();
	this->IsDMATransmitOK = 1;
}
#endif

SPI_C SPI_1;
#endif