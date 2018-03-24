/**
  * @file  readme.txt
  * @brief 项目工程说明
  * @par   date        version    author    remarks
  *        2016-08-12  v1.0       zbt       初次创建
  *
  */

  1. RTC时钟程序
    硬件平台：   STM32F107VCT6学习板
    软件平台：   MDK V5.20 & V5.17 + STM32Cube MX 4.14
    操作系统：   无（裸机开发模板）
	
	/** 去除编译器对汉字奇数字符的警告 在Options for target的C++选项中的misc control
    文本框内添加 --diag_suppress 870 即可，或在源程序中添加如下语句
    #pragma diag_suppress 870    
	*/
	
	已经配置usart1串口可作为调试用
  
  2. 工程说明
	利用STM32F107VCT6自带的RTC当前日期和时间
	
	增加入侵检测功能，在cube下勾选Tamper 即可
	在下载完程序之后，LCD白屏显示， 系统卡死在while (get_tamper_flag() != SET);
	按下开发板上的TAMPER按键后，产生入侵检测中断，在TAMPER_IRQHandler中断函数里面讲入侵检测标志置位
	系统跳出死循环，开始执行GUI_init();等函数，最后执行到显示时间部分
	
    