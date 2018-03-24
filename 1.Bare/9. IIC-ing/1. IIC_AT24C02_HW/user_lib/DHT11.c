/**
*
* DHT11温度处理
*
 */
 
 #include "DHT11.h"
 #include "delay.h"
 #include "led.h"
 #include "gui.h"

 

 unsigned char flag = 0;
 unsigned char comdata = 0;
 unsigned char temp_H = 0,temp_L = 0,humi_H = 0,humi_L = 0,checkdata = 0;
 unsigned char temp_H_R = 1,temp_L_R,humi_H_R,humi_L_R,checkdata_R;

/*****
***
***         输入输出模式区
*/
static void DHT11_INPUT_MODE(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);  /**< 错误 */
}
static void DHT11_OUTPUT_MODE(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
    
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   /**< 开漏改为推挽 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}






 unsigned char DHT11_START(void)
 {
     DHT11_OUTPUT_MODE();//输出模式，此时为高电平
     DHT11_OFF();       //发送低电平开始信号
//     printf("data low\r\n");
     HAL_Delay(20);      //主机信号拉低至少18ms
     DHT11_ON();     //主机信号结束           
     delay_us(30);     //等待响应信号
     DHT11_INPUT_MODE();//输入模式
     printf("响应信号产生完毕 \r\n");
     if(!DHT11_READ)       //检测到DHT响应信号低电平
     { 
        printf("等待低电平\r\n");
         while(!DHT11_READ); //等待低电平结束
         printf("等待高电平\r\n");
         while(DHT11_READ); //拉高80us准备发送数据
          printf("死在高电平\r\n");
         return 1;
     }
     
         return 0;

 }
 
 void DHT11_READ_BYTE(void)
 {
     for(unsigned char i = 0;i < 8;i++)
     {     
         delay_us(10);
         comdata <<= 1;   
         while(!DHT11_READ); //每个bit由50us的低电平开始
         printf("dead one \r\n");
         delay_us(30);       //高电平开始30us后检测电平 
         if(DHT11_READ)       //高电平为1
         {
              printf("dead two \r\n");
//             while(DHT11_READ); //等待此高电平结束
              printf("dead three \r\n");
             comdata |= 0x01;
         }
         else
         {
             comdata &= 0xfe;  //为低电平
         }

     }   
 }
 
 void DHT11_READ_DATA(void)
 {
    if(DHT11_START())
    {
        printf("DHT11 start\r\n");
        
         DHT11_READ_BYTE();
         humi_H_R = comdata;
         DHT11_READ_BYTE();
         humi_L_R = comdata;
         DHT11_READ_BYTE();
         temp_H_R = comdata;
         DHT11_READ_BYTE();
         temp_L_R = comdata;
         DHT11_READ_BYTE();
         checkdata_R = comdata;
    
//         delay_us(50);
//         DHT11_ON();
//         delay_us(20);
//         DHT11_OUTPUT_MODE();
    }
    
    
//------------------   
//    GUI_DispDecAt(humi_H_R, 50, 100, 4);
//    GUI_DispDecAt(humi_L_R, 150, 100, 4);
//    GUI_DispDecAt(temp_H_R, 50, 150, 4);
//    GUI_DispDecAt(temp_L_R, 150, 150, 4);
//    GUI_DispDecAt(checkdata_R, 50, 200, 4);
    
     checkdata = temp_H_R + temp_L_R + humi_H_R + humi_L_R;
     if(checkdata == checkdata_R)
     {
         led_on(LED_0);
         temp_H = temp_H_R;
         temp_L = temp_L_R;
         humi_H = humi_H_R;
         humi_L = humi_L_R;
         led_off(LED_0);
     }
      
}
 
