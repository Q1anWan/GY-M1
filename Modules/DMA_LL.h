/*
	DMA LowLazer库DMA支持
	  千万 2022.6.21
*/
/*
	一键清除中断
	消除Channel与Stream的差异
*/

#ifndef DMA_LL_H
#define DMA_LL_H
#include "main.h"
/*是否启用DMA*/
#if defined (DMA1) || defined (DMA2) || defined (DMA3)
#include "dma.h"
#ifdef __cplusplus
class DMA_C
{
	public:
	DMA_TypeDef *DMAx;
	uint32_t DMA_Channelx;
		
	public:
	void DMA_EnableCH(void);
	void DMA_DisableCH(void);
	#if defined(DMA_IFCR_CTCIF1)
	void DMA_Clear_TC(void);
	#endif
	
	#if defined(DMA_IFCR_CHTIF1)
	void DMA_Clear_HT(void);
	#endif
	
	#if defined(DMA_IFCR_CGIF1)
	void DMA_Clear_GI(void);
	#endif
	
	#if defined(DMA_IFCR_CTEIF1)
	void DMA_Clear_TE(void);
	#endif
	
	#ifdef LL_DMA_CHANNEL_1
	void DMA_InitConfig(DMA_TypeDef *DMAx, uint32_t Channel);
	#endif
	#ifdef LL_DMA_STREAM_1
	void DMA_InitConfig(DMA_TypeDef *DMAx, uint32_t Stream);
	#endif
};
	
extern "C" {
}
#endif
#endif
#endif