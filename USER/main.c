#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "usart_idle_dma.h"
 
extern char g_sendBuff[USART_DMA_TX_BUFF_LEN];
 int main(void)
 {		
	u16 i;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
//	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	USARTx_Config(); 
 	while(1)
	{
		if(g_USART_RxBuff_t.rx_data_len > 0)
		{			
			printf(">>>data len is��%d \r\n",g_USART_RxBuff_t.rx_data_len);
			for(i = 0;i < g_USART_RxBuff_t.rx_data_len;i++)
			{
				g_sendBuff[i] = g_USART_RxBuff_t.rx_buff[i];
			}				
			USART1_DMA_SendData(g_USART_RxBuff_t.rx_data_len);
			g_USART_RxBuff_t.rx_data_len = 0;
		}
//		printf("aaa\r\n");
	}	 
}

