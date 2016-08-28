/**
 * @file   delay.c
 * @brief  延时程序
 * @author 王晓荣
 * @version 
 * @date   2011-03-19
 */
 

/**
 * @brief 延时函数
 * @param t 循环次数
 */
void delay(unsigned long t)
{ 
    unsigned int i;
    unsigned int j;
    
//	while(t--);	
    for (i = t; i > 0; i--)
    {
        for (j = 200; j > 0; j--);
    }
}
