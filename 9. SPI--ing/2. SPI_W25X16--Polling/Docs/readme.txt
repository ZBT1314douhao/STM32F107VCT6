/**
  * @file  readme.txt
  * @brief ��Ŀ����˵��
  * @par   date        version    author    remarks
  *        2016-08-12  v1.0       zbt       ���δ���
  *
  */

  1. RTCʱ�ӳ���
    Ӳ��ƽ̨��   STM32F107VCT6ѧϰ��
    ���ƽ̨��   MDK V5.20 & V5.17 + STM32Cube MX 4.14 + STemwin V5.22
    ����ϵͳ��   �ޣ��������ģ�壩
	
	/** ȥ���������Ժ��������ַ��ľ��� ��Options for target��C++ѡ���е�misc control
    �ı�������� --diag_suppress 870 ���ɣ�����Դ����������������
    #pragma diag_suppress 870    
	*/
	
	�Ѿ�����usart1���ڿ���Ϊ������
  
  2. ����˵��
  
	 2.1 оƬ����˵����	
	     STM32         W25X16
	     PB9        -- CS
		 PA5��CLK�� -- SCK
		 PA6��MISO��-- DO
		 PA7��MOSI��-- DI
		 SPI1����Ϊ��ѯģʽ
		 
	 2.2 ������˵����
         ѧϰ����ش洢��W25X16����������ͨ��SPI1����ͨѶ�Ĳ��Գ���
	     Ŀǰ���������ж�ȡоƬ��ID�Ĳ��ԣ�
		 