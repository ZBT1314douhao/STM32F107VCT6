/**
  * @file  readme.txt
  * @brief 项目工程说明
  * @par   date        version    author    remarks
  *        2016-08-12  v1.0       zbt       初次创建
  *		   2016-10-06  v2.0		  zbt		添加bsp.c bsp.h
  */

  1. 开发环境说明
    硬件平台：   STM32F107VCT6学习板
    软件平台：   MDK V5.20 & V5.17 + STM32Cube MX 4.14 + STemwin5.22
    操作系统：   无（裸机开发模板）
	
	/** 去除编译器对汉字奇数字符的警告 在Options for target的C++选项中的misc control
    文本框内添加 --diag_suppress 870 即可，或在源程序中添加如下语句
    #pragma diag_suppress 870    
	*/
	
	已经配置usart1串口可作为调试用
	drivers文件下（包括user drivers & board drivers）的驱动头文件均需在bsp.h中包含
	
	1.1 STM32F407ZGT6引脚分配说明
		下载方式：SWD -> SWDIO SWCLK
		串口打印：USART1 -> USART_TX（PA9） + USART_RX（PA10）
		液晶触摸：
				SPI1 -> SCK（PA5） + MISO（PA6） + MOSI（PA7）
				CS（PA4） + RESET（PC5）+ INT（PC4）
				
	1.2 项目工程文件架构说明
		Application/MDK_ARM				--------	STM32启动文件		
		Application/User 				--------  	存放应用层程序
		Drivers/CMSIS					--------	M4内核外设接口层系统源文件
		Drivers/STM32F1xx_HAL_Drivers	--------	STM32F1的HAL库源文件
		Drviers/User     				-------- 	存放非STM32片上外设驱动程序
		Drivers/Board   				--------  	存放STM32片上外设相关驱动程序
		Middlewares/FreeRTOS			--------	FreeRTOS相关文件
		STemwin/Config					--------	存放移植STemwin所需的配置文件
		STemwin/Font					--------	存放用到的字体
		Docs							--------  	存放项目工程说明文件
		
  2. 工程说明