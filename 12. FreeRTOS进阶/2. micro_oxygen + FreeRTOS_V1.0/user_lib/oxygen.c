/**
  * @file	oxygen.c 
  * @brief	获取氧气浓度
  * @par	date        version     author      remarks
  *			2016-07-23	V1.0		zbt			初次创建			
  */
 
/** -------------------- 头文件包含区 ------------------------------ */
#include "oxygen.h"
#include "adc.h"
#include "gui.h"

/** -------------------- 文件私有变量 ------------------------------ */
#define OXYGEN_BUFF_SIZE       1       /**< 设置采集次数 */

static uint8_t s_adc2_it_flag[] = {0, };

/** -------------------- 外部引用变量 ------------------------------ */

/** -------------------- 私有函数原型 ------------------------------ */
static struct S_AD_OXYGEN AD_get_oxygen_concertration(void);

/** -------------------- 公有函数 ---------------------------------- */
/**
 * @brief   启动ADC2
 * @param   None
 * @retval  None
 * @note    STM32F107VCT6的ADC2不能进行DMA方式传送
 */
void ADC2_start_IT(void)
{
    HAL_ADC_Start_IT(&hadc2);
}

 /**
 * @brief   显示采集到的氧浓度
 * @param   coor_x：显示氧浓度的X坐标
            coor_y：显示氧浓度的Y坐标
 * @retval  None
 */
void display_oxygen_concertration(const uint16_t coor_x, const uint16_t coor_y)
{
    struct S_AD_OXYGEN s_oxygen;
    
    if (read_adc2_it_flag(ADC2_IN8))
    {
        s_oxygen = AD_get_oxygen_concertration();
    }
    
    GUI_GotoXY(coor_x, coor_y);
    GUI_DispFloat(s_oxygen.value, 5);
    GUI_DispStringAt("V", (coor_x + 70), coor_y);
    GUI_GotoXY((coor_x + 105), coor_y);
    GUI_DispFloat(s_oxygen.oxygen_value, 5);
}

/**
 * @brief   读取ADC2中断标志
 * @param   adc2_x:ADC2的通道
 * @retval  tmp_flag：读取到的中断标志的值
 */
uint8_t read_adc2_it_flag(enum ADC2_INx adc2_x)
{
    uint8_t tmp_flag;
    
    tmp_flag = s_adc2_it_flag[adc2_x];
    s_adc2_it_flag[adc2_x] = 0;
    
    return tmp_flag;
}

/**
 * @brief   设置ADC2中断标志
 * @param   adc2_x：ADC2的通道
 * @retval  None
 */
void set_adc2_it_flag(enum ADC2_INx adc2_x)
{
//    if (__HAL_ADC_GET_FLAG(&hadc2, ADC_FLAG_EOC))
//    {
        s_adc2_it_flag[adc2_x] = 1;
//    }
    
}

 /** -------------------- 私有函数 --------------------------------- */
 /**
 * @brief   通过AD转换得到的电压值得到相应的氧浓度
 * @param   None
 * @retval  s_oxygen：包含电压值和氧浓度值的结构体
 */
static struct S_AD_OXYGEN AD_get_oxygen_concertration(void)
{
    uint8_t  i;
    uint32_t ad_value;
    struct S_AD_OXYGEN s_oxygen;
    
    for (i = 0; i < OXYGEN_BUFF_SIZE; i++)
    {
        ad_value += HAL_ADC_GetValue(&hadc2);
    }
    ad_value /= OXYGEN_BUFF_SIZE; 
    
    /** 获取电压值 用于计算氧浓度 */
    s_oxygen.value = (double)(ad_value * 3.3/ (4096 - 1));
    /** 此处为模拟计算公式 具体公式视传感器而定 */
    s_oxygen.oxygen_value = (1.43 - s_oxygen.value) / 4.3 + 25.0;
    
    return s_oxygen;
}
