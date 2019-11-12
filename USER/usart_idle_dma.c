/*******************************************************************************
* @File     usart_idle_dma.c
* @Brief    ʹ��DMA��USART�����ļ�
* @Date     2019-11-12
* @Version  V1.0
* @Note     ʹ��DMA����USART���պͷ���
* @Author   EmbeddedXGJ
*******************************************************************************/

#include "usart_idle_dma.h"
#include "dma.h"

char g_sendBuff[USART_DMA_TX_BUFF_LEN];
stUSART_RX_BUFF  g_USART_RxBuff_t;   /* ���ջ����� */


#if USING_USART_DMA_RX
/* ����1����DMAͨ����ʼ�� */
void USARTx_RX_DMA_Config(void)
{
	My_DMA_Config(USART1_RX_DMA_CHANNEL,         //USART1 DMA RX ͨ��
	              (u32)&USART1_DR_ADDR,           //�����ڴ��ַ
	              (u32)g_USART_RxBuff_t.rx_buff,  //�ڴ��ַ
	              USART_RX_BUFF_LEN,           //DMAͨ����DMA����Ĵ�С
	              DMA_DIR_PeripheralSRC,          //��������赽�ڴ�
	              DMA_Mode_Circular);             //ѭ��ģʽ
	
	DMA_ClearFlag(DMA1_FLAG_TC5);                            //���ͨ����־λ                
	DMA_ITConfig(USART1_RX_DMA_CHANNEL,DMA_IT_TC, ENABLE);   //��������1��DMA��������ж�
	DMA_Cmd(USART1_RX_DMA_CHANNEL,ENABLE);                 //ʹ��DMA
}
#endif

#if USING_USART_DMA_TX
/* ����1����DMAͨ����ʼ�� */
void USARTx_TX_DMA_Config(void)
{
	My_DMA_Config(USART1_TX_DMA_CHANNEL,         //USART1 DMA RX ͨ��
	              (u32)&USART1_DR_ADDR,           //�����ڴ��ַ
	              (u32)g_sendBuff,                //�ڴ��ַ
	              USART_DMA_TX_BUFF_LEN,          //DMAͨ����DMA����Ĵ�С
	              DMA_DIR_PeripheralDST,          //������ڴ浽����
	              DMA_Mode_Normal);               //����ģʽ
}

/* ����һ��DMA���� */
void USART1_DMA_SendData(uint16_t DataNumber)
{
	USARTx_DMA_Enable_Send(USART1,USART1_TX_DMA_CHANNEL,DataNumber);
}
#endif


void USART1_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
		 //Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

void USARTx_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	g_USART_RxBuff_t.rx_data_len = 0;
	
	RCC_APB2PeriphClockCmd(USART1_CLK, ENABLE);	     /* ʹ��USART1ʱ�� */
	RCC_APB2PeriphClockCmd(USART1_PORT_CLK, ENABLE);  /* ʹ��GPIOAʱ�� */
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin   = USART1_TX_PIN;      //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(USART1_PORT, &GPIO_InitStructure);       //��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin  = USART1_RX_PIN;         //PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(USART1_PORT, &GPIO_InitStructure);         //��ʼ��GPIOA.10  

	/* ����1 NVIC����*/
	USART1_NVIC_Config();
	
	/* ����1��ʼ�� */
	USART_InitStructure.USART_BaudRate            = USART_BAUDRATE;         //���ڲ�����
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;    //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;       //һ��ֹͣλ
	USART_InitStructure.USART_Parity              = USART_Parity_No;       //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ	
	USART_Init(USART1, &USART_InitStructure);                                       //��ʼ������1
				
#if USING_USART_DMA_RX  /* ʹ��DMA + �����жϽ��� */
	USART_ITConfig(USING_USART, USART_IT_IDLE, ENABLE);//���ô��ڿ����ж�
	USART_DMACmd(USING_USART,USART_DMAReq_Rx, ENABLE);	//����DMA����
	USARTx_RX_DMA_Config();                            //USART1��DMA��ʼ�� 
#else  /* �������ڽ����ж�  ��ͨ�жϽ��� */
	USART_ITConfig(USING_USART, USART_IT_RXNE, ENABLE); //�������ڽ����ж�  ��ͨ�жϽ���
#endif

#if USING_USART_DMA_TX
//	USART_DMACmd(USING_USART,USART_DMAReq_Tx, ENABLE);	//����DMA����
	USARTx_TX_DMA_Config();
#endif

	USART_Cmd(USING_USART, ENABLE);                    //ʹ�ܴ���1 
}	

#if USING_USART_DMA_RX
void USART1_DMA_Recvice_Data(void)
{
	uint32_t buff_len;
	
	/* �ر�DMA ����ֹ���� */
	DMA_Cmd(USART1_RX_DMA_CHANNEL, DISABLE);  /* ��ʱ�ر�dma������δ���� */ 
	
	buff_len = USART_RX_BUFF_LEN - DMA_GetCurrDataCounter(USART1_RX_DMA_CHANNEL);
	g_USART_RxBuff_t.rx_data_len = buff_len;   //������յ����ݳ���
	
	DMA_ClearFlag(DMA1_FLAG_TC5);               //�����־λ
	
	//��������DMAͨ����DMA����Ĵ�С
	DMA_SetCurrDataCounter(USART1_RX_DMA_CHANNEL, USART_RX_BUFF_LEN); 
	
	/* ʹ��DMA */ 	
	DMA_Cmd(USART1_RX_DMA_CHANNEL, ENABLE);  
}
#else
void USART_RecvData(void)
{
	u8 res;
	static uint32_t cnt = 0;
	
	res = USART_ReceiveData(USART1);	                   //��ȡ���յ�������	
	if(cnt < USART_RX_BUFF_LEN){   
		g_USART_RxBuff_t.rx_buff[cnt] = res;
		g_USART_RxBuff_t.rx_data_len = cnt;
		cnt++;	
	}else {
		g_USART_RxBuff_t.rx_buff[0] = res;   //�����ݰᵽ��һ��
		cnt = 1;
	}		
}
#endif

void USART1_IRQHandler(void)                	//����1�жϷ������
{	
#if USING_USART_DMA_RX
	if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)   /* �����ж� */
	{	
		/* �������� */
		USART1_DMA_Recvice_Data();	
		/* ��������жϱ�־λ */ 
		USART_ReceiveData(USART1);
	}
#else	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		USART_RecvData();
	} 
#endif											
} 


