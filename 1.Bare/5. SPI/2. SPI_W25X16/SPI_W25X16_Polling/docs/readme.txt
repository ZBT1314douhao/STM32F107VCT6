/**
  * @file  readme.txt
  * @brief ��Ŀ����˵��
  * @par   date        version    author    remarks
  *        2016-08-12  v1.0       zbt       ���δ���
  *		   2016-10-06  v2.0		  zbt		���bsp.c bsp.h
  */

  1. ��������˵��
    Ӳ��ƽ̨��   STM32F107VCT6ѧϰ��
    ���ƽ̨��   MDK V5.20 & V5.17 + STM32Cube MX 4.14 + STemwin5.22
    ����ϵͳ��   �ޣ��������ģ�壩
	
	/** ȥ���������Ժ��������ַ��ľ��� ��Options for target��C++ѡ���е�misc control
    �ı�������� --diag_suppress 870 ���ɣ�����Դ����������������
    #pragma diag_suppress 870    
	*/
	
	�Ѿ�����usart1���ڿ���Ϊ������
	drivers�ļ��£�����user drivers & board drivers��������ͷ�ļ�������bsp.h�а���
	
	1.1 STM32F407ZGT6���ŷ���˵��
		���ط�ʽ��SWD -> SWDIO SWCLK
		���ڴ�ӡ��USART1 -> USART_TX��PA9�� + USART_RX��PA10��
		Һ��������
				SPI1 -> SCK��PA5�� + MISO��PA6�� + MOSI��PA7��
				CS��PA4�� + RESET��PC5��+ INT��PC4��
				
	1.2 ��Ŀ�����ļ��ܹ�˵��
		Application/MDK_ARM				--------	STM32�����ļ�		
		Application/User 				--------  	���Ӧ�ò����
		Drivers/CMSIS					--------	M4�ں�����ӿڲ�ϵͳԴ�ļ�
		Drivers/STM32F1xx_HAL_Drivers	--------	STM32F1��HAL��Դ�ļ�
		Drviers/User     				-------- 	��ŷ�STM32Ƭ��������������
		Drivers/Board   				--------  	���STM32Ƭ�����������������
		Middlewares/FreeRTOS			--------	FreeRTOS����ļ�
		STemwin/Config					--------	�����ֲSTemwin����������ļ�
		STemwin/Font					--------	����õ�������
		Docs							--------  	�����Ŀ����˵���ļ�
		
  2. ����˵��