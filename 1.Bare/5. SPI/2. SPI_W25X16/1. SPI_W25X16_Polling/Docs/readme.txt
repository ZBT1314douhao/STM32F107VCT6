/**
  * @file  readme.txt
  * @brief 项目工程说明
  * @par   date        version    author    remarks
  *        2016-08-12  v1.0       zbt       初次创建
  *
  */

  1. RTC时钟程序
    硬件平台：   STM32F107VCT6学习板
    软件平台：   MDK V5.20 & V5.17 + STM32Cube MX 4.14 + STemwin V5.22
    操作系统：   无（裸机开发模板）
	
	/** 去除编译器对汉字奇数字符的警告 在Options for target的C++选项中的misc control
    文本框内添加 --diag_suppress 870 即可，或在源程序中添加如下语句
    #pragma diag_suppress 870    
	*/
	
	已经配置usart1串口可作为调试用
  
  2. 工程说明
  
	 2.1 芯片连接说明：	
	     STM32         W25X16
	     PB9        -- CS
		 PA5（CLK） -- SCK
		 PA6（MISO）-- DO
		 PA7（MOSI）-- DI
		 SPI1配置为轮询模式
		 
	 2.2 程序功能说明：
         学习板板载存储器W25X16和主控制器通过SPI1进行通讯的测试程序
	     目前本程序今进行读取芯片的ID的测试，
		 