/**
  * @file  readme.txt
  * @brief ��Ŀ����˵��
  * @par   date        version    author    remarks
  *        2016-08-12  v1.0       zbt       ���δ���
  *
  */

  1. RTCʱ�ӳ���
    Ӳ��ƽ̨��   STM32F107VCT6ѧϰ��
    ���ƽ̨��   MDK V5.20 & V5.17 + STM32Cube MX 4.14
    ����ϵͳ��   �ޣ��������ģ�壩
	
	/** ȥ���������Ժ��������ַ��ľ��� ��Options for target��C++ѡ���е�misc control
    �ı�������� --diag_suppress 870 ���ɣ�����Դ����������������
    #pragma diag_suppress 870    
	*/
	
	�Ѿ�����usart1���ڿ���Ϊ������
  
  2. ����˵��
	����STM32F107VCT6�Դ���RTC��ǰ���ں�ʱ��
	
	�������ּ�⹦�ܣ���cube�¹�ѡTamper ����
	�����������֮��LCD������ʾ�� ϵͳ������while (get_tamper_flag() != SET);
	���¿������ϵ�TAMPER�����󣬲������ּ���жϣ���TAMPER_IRQHandler�жϺ������潲���ּ���־��λ
	ϵͳ������ѭ������ʼִ��GUI_init();�Ⱥ��������ִ�е���ʾʱ�䲿��
	
    