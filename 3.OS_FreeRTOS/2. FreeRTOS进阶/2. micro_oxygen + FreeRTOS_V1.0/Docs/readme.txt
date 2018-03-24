/**
  * @file	readme.txt 
  * @brief	项目工程说明文档
  * @par	date        version     author      remarks
  *			2016-07-22	V1.0		zbt			初次创建
  */
  
  1. 项目简介
    项目名称：微氧测量仪模拟设计(通过STM32F107VCT6片内外设模拟各功能)          
    硬件平台：STM32F107VCT6开发板
    软件平台：STM32Cube MX 4.16 & MDK V5.20
    触摸屏驱动方式：SPI通讯（SPI3）
    显示界面设计：STemwin V5.22
    
  2. 项目功能说明
    2.1 利用ADC1的3各通道分别模拟测量氧传感器的浓度、流量传感器的流速、
       CPU附近的温度
    2.2 利用芯片自带的RTC显示当前日期和时间，可以通过按下SetTime按钮进
        入adjust_time_window进行时间、日期的调节
    2.3 sys_info_window显示系统信息，在此界面下触摸屏幕弹出PopUp Menu
        可跳入其他页面，或者显示帮助对话框
    2.4 添加操作系统支持FreeRTOS，提高系统响应的实时性
                                                