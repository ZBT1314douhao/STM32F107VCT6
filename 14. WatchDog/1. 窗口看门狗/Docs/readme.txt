/**
  * @file  readme.txt
  * @brief 项目工程说明
  * @par   date        version    author    remarks
  *        2016-08-12  v1.0       zbt       初次创建
  *
  */

  1. 窗口看门狗程序
    硬件平台：   STM32F107VCT6学习板
    软件平台：   MDK V5.20 & V5.17 + STM32Cube MX 4.14
    操作系统：   无（裸机开发模板）
  
	已经配置usart1串口作为调试用
  
  2. 模板说明
	WWDG 时钟计数器 (PCLK1 (36MHz)/4096)/8) = 1099 Hz (~910 us) 
		 上式的8源自预分频值： Prescaler = WWDG_PRESCALER_8
		 窗口值 Window = 81表示窗口看门狗计数器应该在80以下0x40（64）
		 以上的时候被刷新，否则将产生复位
		 Counter = 127
		 超时时间 timeout = 910 * 64 / 1000 = 58.24 ms
			
		更改check_system_resume_status函数中的延时可观察现象
  
    