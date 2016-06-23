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
        LED_1 5S亮灭一次                                2016.06.10
     6. 增加临界区和中断演示开关功能(用于临界资源的访问 当然目前这里
        似乎并没有临界)
        taskENTER_CRITICAL & taskEXIT_CRITICAL为调度器锁
        里面包含关闭和开中断语句
        
        taskDISABLE_INTERRUPTS & taskENABLE_INTERRUPTS 为中断锁
        中断关闭器件系统将不再响应任何中断时间，故对系统的实时性影响非常大
        故最好保持关闭中断的时间非常短，否则会导致系统实时性变差甚至导致系统
        基本无实时性
        在RT-Thread里面 调度器上锁后依然响应中断，        2016.06.11
        
        存在临界区和多线程调用emwin函数API的bug           20:00
        
 
 
 
 
 
 */

