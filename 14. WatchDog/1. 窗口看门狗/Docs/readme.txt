/**
  * @file  readme.txt
  * @brief ��Ŀ����˵��
  * @par   date        version    author    remarks
  *        2016-08-12  v1.0       zbt       ���δ���
  *
  */

  1. ���ڿ��Ź�����
    Ӳ��ƽ̨��   STM32F107VCT6ѧϰ��
    ���ƽ̨��   MDK V5.20 & V5.17 + STM32Cube MX 4.14
    ����ϵͳ��   �ޣ��������ģ�壩
  
	�Ѿ�����usart1������Ϊ������
  
  2. ģ��˵��
	WWDG ʱ�Ӽ����� (PCLK1 (36MHz)/4096)/8) = 1099 Hz (~910 us) 
		 ��ʽ��8Դ��Ԥ��Ƶֵ�� Prescaler = WWDG_PRESCALER_8
		 ����ֵ Window = 81��ʾ���ڿ��Ź�������Ӧ����80����0x40��64��
		 ���ϵ�ʱ��ˢ�£����򽫲�����λ
		 Counter = 127
		 ��ʱʱ�� timeout = 910 * 64 / 1000 = 58.24 ms
			
		����check_system_resume_status�����е���ʱ�ɹ۲�����
  
    