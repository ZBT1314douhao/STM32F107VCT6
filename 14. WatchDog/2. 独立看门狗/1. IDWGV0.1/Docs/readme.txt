/**
  * @file  readme.txt
  * @brief 项目工程说明
  * @par   date        version    author    remarks
  *        2016-08-12  v1.0       zbt       初次创建
  *
  */

  1. 本工程为模板文件(非模板文件添加项目说明)
    硬件平台：   STM32F107VCT6学习板
    软件平台：   MDK V5.20 & V5.17 + STM32Cube MX 4.14
    操作系统：   无（裸机开发模板）
  
	已经配置usart1串口作为调试用
	
  2. 模板说明
	独立看门狗程序，
	
	独立看门狗几个特性： 
		程序中用到了独立看门狗 则使用者有责任定时喂狗
		向下计数，到0x00后产生系统复位
		启动后即使主时钟挂了 其专用的LSI(一般为40kHZ，波动区间30-60kHZ)也不挂
		更多内容参见stm32f1xx_hal_iwdg.c or STM32参考手册
		
		定时计算公式：count_reload_value = timeout * Prescaler
		reload此处为：4095，此处为：IWDG_PRESCALER_4
		计算可得timeout 约为409.5ms
		重载值相同情况下，预分频值越大，独立看门狗溢出需要的时间越长
		最长可达26.2144S
