/**
  * @file    exti_dup.c 
  * @author  Z B T
  * @version V0.1
  * @date    2016- 03
  * @brief   
  *
  */  

#include "exti_dup.h"
#include "led.h"
#include "delay.h"

static void EXTI0_ISR(void);
static void EXTI13_ISR(void);


/**
 *  @brief  完成外部中断服务程序
            stm32f10xx_hal_gpio.c中的虚函数 在此重新实现此函数 
 *  @param  [in]  GPIO_Pin 连接在中断线上的引脚
 *  @return None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
        case GPIO_PIN_0:
            EXTI0_ISR();
        break;
        case GPIO_PIN_13:
            EXTI13_ISR();
        break;
        default:
            break;
    }
}


/**
 *  @brief  外部中断0 中断服务程序
 *  @param 	None	
 *  @return None
 */
static void EXTI0_ISR(void)
{
    led_on(LED_0);
//    led_on(LED_0 | LED_1);
//    delay_n_ms(200);
//    led_off(LED_0 | LED_1);
//    delay_n_ms(200);
}

/**
 *  @brief  外部中断13 中断服务程序
 *  @param  None
 *  @return None
 */
static void EXTI13_ISR(void)
{
    led_off(LED_0);
//    led_on(LED_2 | LED_3);
//    delay_n_ms(100);
//    led_off(LED_2 | LED_3);
//    delay_n_ms(100);
}


