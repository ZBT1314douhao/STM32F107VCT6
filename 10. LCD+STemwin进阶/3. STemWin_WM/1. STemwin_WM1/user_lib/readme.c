/**
 * @file    readme.c
 * @brief   更新日志&工程说明       
 * @author  Z B T
 * @version V0.1
 * @date    2016.06.07
 */

/** --------------------------- Log ---------------------------------- */
/**
    Demo
    1. 移植STM32官方Demo程序(终于发现不能动画演示是因为3的问题)
    2. 如果下载到目标板(STM32F107VCT6学习板)无法运行可尝试下一条建议
    3. 修改startup_stm32f107xc.s文件中的Stack_Size大小 (用Cube重新生成代码时要人工再次更改Stack_Size大小)
    4. 为啥运行的有点卡呢 
    5. 在guidemo.h 可以将相应的宏写为1 or 0 使能或失能相应演示效果
    6. Demo程序需要一个定时器不断调用OS_TimeMS 此处选用TIM7
    
    WM（窗口管理器）
    1. WM_TEST_N == 0   利用GUIBuilder设计界面并 生成c语言程序
    2. WM_TEST_N == 1   回调函数的基本应用示例
    3. WM_TEST_N == 2   Widget之进度条Demo程序
    4. WM_TEST_N == 3   创建框架窗口和按键示例     2016.06.10
    
    WM_MESSAGE的元素
    数据类型    元素        描述
    int         MsgId       消息的类型   见emwin手册14.3.2
    WM_HWIN     hWin        目标窗口
    WM_HWIN     hwinSrc     源窗口
    void*       Data.p      数据指针
    int         Data.v      数据值        应该是存放ID的值
    
 
 
 
 
 
 
 
 */

