/**
 * @file    readme.c
 * @brief   更新日志&工程说明       
 * @author  Z B T
 * @version V0.1
 * @date    2016.06.07
 */

/** --------------------------- Log ----------------------------------*/
/**
    1. 利用stm32cube mx配置生成freertos
    2. 通过freertos点灯（单一线程）
    3. 增加STemwin显示线程
    4. 利用OSDelayUntil函数实现固定周期性任务 需要在FreeRTOSConfig.h中
        配置INCLUDE_vTaskDelayUntil的宏定义为 1
    5. 增加LED_1挂起任务 恢复挂起任务功能 结果实现LED_0 3S亮灭一次
        LED_1 5S亮灭一次        2016.06.11
    
 
 
 
 
 
 
 
 */

