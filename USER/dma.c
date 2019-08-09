#include "dma.h"

void My_DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 peri_base_addr,
	               u32 mem_base_addr,u16 buff_size,uint32_t DMA_DIR,uint32_t DMA_Mode)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����

	DMA_DeInit(DMA_CHx);       //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
//	DMA1_MEM_LEN                             = cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = peri_base_addr; //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr     = mem_base_addr;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR                = DMA_DIR;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize         = buff_size;                  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ���������
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;       //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;    //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode               = DMA_Mode;            //����������ģʽ
	DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;        //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;             //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);   //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
}

void USARTx_DMA_Enable_Send(USART_TypeDef* USARTx,DMA_Channel_TypeDef*DMA_CHx,uint16_t DataNumber)
{ 
	DMA_Cmd(DMA_CHx, DISABLE);                //�ر�USART1 TX DMA1 ��ָʾ��ͨ��      
 	DMA_SetCurrDataCounter(DMA_CHx,DataNumber);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);                    //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE); //ʹ��DMA����
}	  

