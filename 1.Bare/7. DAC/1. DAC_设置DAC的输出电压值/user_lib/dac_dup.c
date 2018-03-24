/**
  * @file    dac_dup.c 
  * @author  Z B T
  * @version V0.1
  * @date    2016.05
  * @brief   dac应用层
  *
  */  

#include "dac_dup.h"


extern DAC_HandleTypeDef hdac;


/**
 *  @brief  设置DAC的输出电压值
 *  @param  voltage： 要设置的电压值，单位mV
			范围： 0 - 3300，(最大值是 VDD = 3300mV)
 *  @retval None
 */
void DAC_set_voltage(float voltage)
{
    uint32_t dac_set_value;
    double dac_out_value;
    
    voltage /= 1000;
    voltage = voltage * 4095 / 3.3;
    
    /** 做个简单的误差补偿，可能越补偿误差越大o(╯□╰)o  */
    if (((uint32_t)(voltage * 10) % 10) >= 5)
    {
        voltage += 1;
    }
    else
    {
        voltage += 0;
    }
    printf("vlotage is %f\n", voltage);
    
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, voltage);
    
    HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
    
    dac_set_value = HAL_DAC_GetValue(&hdac, DAC_CHANNEL_1);
    printf("set DAC Value is %d\n", dac_set_value);
    
    dac_out_value = dac_set_value * 3.3 / 4095;
    printf("DAC output vlotage value is %f\n", dac_out_value);
}
