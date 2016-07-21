/** 
 *  @file		lcd_ssd1289.c
 *	@brief	lcd��������(SSD1289)
 *	@par		Copyright (c):  
 *          �Ͼ���ҵ��ѧ�Զ������������ѧԺ
 *  @par		Ӳ������:
 *  -        stc15      ssd1289 
 *  -        P2      -- DB0~7
 *  -        P0      -- DB8~15  
 *  -        P4_0    -- nCS 
 *  -        P4_1    -- nRS 
 *  -        P4_2    -- nWR
 *  -        P4_3    -- nRD 
 *  -        P4_7    -- nReset 
 *  -        VCC     -- BK_LED
 *	@par		�޸���־:
 *					�汾			  ����           ����      ˵��
 *					v0.1			2013-07-06     ������     
 */ 



#include "lcd_ssd1289.h"
#include "delay.h"
#include "lcd_font.h"


/** �����������Ŷ��� */
#define LCD_WR_PIN			GPIO_PIN_14
#define LCD_WR_PORT			GPIOD
//#define LCD_WR_APB2Periph	RCC_APB2Periph_GPIOD

#define LCD_CS_PIN      	GPIO_PIN_7
#define LCD_CS_PORT     	GPIOC
//#define LCD_CS_APB2Periph	RCC_APB2Periph_GPIOC

#define LCD_RS_PIN      	GPIO_PIN_15
#define LCD_RS_PORT     	GPIOD
#define LCD_RS_APB2Periph	RCC_APB2Periph_GPIOD

#define LCD_RD_PIN      	GPIO_PIN_13
#define LCD_RD_PORT     	GPIOD
//#define LCD_RD_APB2Periph	RCC_APB2Periph_GPIOD

#define LCD_RST_PIN         GPIO_PIN_9
#define LCD_RST_PORT     	GPIOD
//#define LCD_RST_APB2Periph	RCC_APB2Periph_GPIOD

#define LCD_DATA_PORT   	GPIOE
//#define LCD_DATA_APB2Periph	RCC_APB2Periph_GPIOE 


#define lcd_set_cs()			HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN,   GPIO_PIN_SET)
#define lcd_clr_cs()			HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN,   GPIO_PIN_RESET)
#define lcd_set_rs()			HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN,   GPIO_PIN_SET)
#define lcd_clr_rs()			HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN,   GPIO_PIN_RESET)
#define lcd_set_wr()			HAL_GPIO_WritePin(LCD_WR_PORT, LCD_WR_PIN,   GPIO_PIN_SET)
#define lcd_clr_wr()			HAL_GPIO_WritePin(LCD_WR_PORT, LCD_WR_PIN,   GPIO_PIN_RESET)
#define lcd_set_rd()			HAL_GPIO_WritePin(LCD_RD_PORT, LCD_RD_PIN,   GPIO_PIN_SET)
#define lcd_clr_rd()			HAL_GPIO_WritePin(LCD_RD_PORT, LCD_RD_PIN,   GPIO_PIN_RESET)
#define lcd_set_rst()			HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET)
#define lcd_clr_rst()			HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET)


unsigned long LCD_POW10[10] = 
{
  1L , 10L, 100L, 1000L, 10000L,
  100000L, 1000000L, 10000000L, 100000000L, 1000000000L
};

/**< ˽�к���ԭ�� */
static void lcd_write_reg(uint16_t reg,uint16_t value);
static void lcd_ram_prepare(void);
static void lcd_write_ram(uint16_t value);
static uint16_t  lcd_bgr2rgb(uint16_t c);
static void lcd_data2string(long dat, uint8_t len, uint8_t* str);
static void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);


/**
 *  LCD���ú���
 *  @param None
 *  @return None
 */
void lcd_config(void)
{
	
	lcd_set_rst();
    delay(1000);	
	lcd_clr_rst();
	delay(2000);
	lcd_set_rst(); 
	delay(10000);
	
	/** �������ų�ʼ�� */
	lcd_set_cs();	
	lcd_set_wr();
	lcd_set_rd();
	lcd_set_rs();
	
	/** �����ڲ����� */
 	lcd_write_reg(0x0000,0x0001);		
 	delay(10000);										/**< �ȴ�osc�ȶ� */

	/** ִ���ϵ����� */
	lcd_write_reg(0x0003,0xA8A4);		
	lcd_write_reg(0x000C,0x0000);        
	lcd_write_reg(0x000D,0x080C);        
	lcd_write_reg(0x000E,0x2B00);        
	lcd_write_reg(0x001E,0x00B0);  
	delay(10000);	  
	
	/** .13  0��RGB��ֵԽСԽ����1��RGB��ֵԽ��Խ��
	  * .11  0��RGB��1: BGR
		* .9   0��319-->0��1: 0-->319
		* .14  0��719-->0��1: 0-->719
		* .0 ~.8  ��������к� (0x13f = 319)
	  */
	lcd_write_reg(0x0001,0x293F);		
	
	lcd_write_reg(0x0002,0x0600);     	/**< LCD Driving Waveform control */
	lcd_write_reg(0x0010,0x0000);     	/**< .0  0: �ر�˯��ģʽ; 1: ��˯��ģʽ */
	lcd_write_reg(0x0011,0x6070);				/**< .13-.14  11��16λRGBģʽ��10��18λRGBģʽ */	
	lcd_write_reg(0x0016,0xEF1C);       /**< .15-.8   ����ÿ�е���������0xef: ��Ϊ240 */
	lcd_write_reg(0x0017,0x0003);       /**< Vertical Porch */
	lcd_write_reg(0x0007,0x0233);		    /**< Display Control */  
	lcd_write_reg(0x000B,0x0000);       /**< Frame Cycle Control */  
	lcd_write_reg(0x000F,0x0000);				/**< Gate Scan Position */  
	lcd_write_reg(0x0041,0x0000);     	/**< Vertical Scroll Control */  
	lcd_write_reg(0x0042,0x0000);     	/**< Vertical Scroll Control */  
	lcd_write_reg(0x0048,0x0000);     	/**< Screen driving position */  
	lcd_write_reg(0x0049,0x013F);     	/**< Screen driving position */  
	lcd_write_reg(0x004A,0x0000);     	/**< Screen driving position */  
	lcd_write_reg(0x004B,0x0000);     	/**< Screen driving position */  
	lcd_write_reg(0x0044,0xEF00);     	/**< Horizontal RAM address position */  
	lcd_write_reg(0x0045,0x0000);    		/**< Horizontal RAM address position */   
	lcd_write_reg(0x0046,0x013F);     	/**< Horizontal RAM address position */  
	lcd_write_reg(0x0030,0x0707);     	/**< Gamma Control */  
	lcd_write_reg(0x0031,0x0204);     	/**< Gamma Control */  
	lcd_write_reg(0x0032,0x0204);     	/**< Gamma Control */  
	lcd_write_reg(0x0033,0x0502);     	/**< Gamma Control */  
	lcd_write_reg(0x0034,0x0507);     	/**< Gamma Control */  
	lcd_write_reg(0x0035,0x0204);     	/**< Gamma Control */  
	lcd_write_reg(0x0036,0x0204);    		/**< Gamma Control */   
	lcd_write_reg(0x0037,0x0502);   		/**< Gamma Control */    
	lcd_write_reg(0x003A,0x0302);   		/**< Gamma Control */    
	lcd_write_reg(0x003B,0x0302);   		/**< Gamma Control */    
	lcd_write_reg(0x0023,0x0000);    		/**< RAM write data mask */   
	lcd_write_reg(0x0024,0x0000);     	/**< RAM write data mask */  
	lcd_write_reg(0x0025,0x8000);     	/**< Frame Frequency Control; 0X8000: ��Ӧ��Ļ��ӦƵ��Ϊ65Hz */  
	lcd_write_reg(0x004e,0);        		/**< ��(X)�׵�ַ���� */  
	lcd_write_reg(0x004f,0);        		/**< ��(Y)�׵�ַ���� */		       		   
}

/**
 *  LCD����
 *  @param [in]color ˢ����ɫ
 *  @return None
 */
void lcd_clr_screen(uint16_t color)
{
	uint32_t index=0;
	lcd_set_cursor(0,0); 
	lcd_ram_prepare(); 	

	lcd_clr_cs();
	lcd_set_rs();

	for(index=0;index<76800;index++)			/** 320*240 = 76800 */
	{
		lcd_clr_wr();
		GPIO_Write(LCD_DATA_PORT, color);
		lcd_set_wr();		
	}
	lcd_set_cs();   
}	

/**
 *  �������
 *  @param [in]x ��
 *  @param [in]y ��
 *  @return None
 */
void lcd_set_cursor(uint16_t y, uint16_t x)
{
 	lcd_write_reg(0x004e,y);		
	lcd_write_reg(0x004f,x);		
}

/**
 *  ָ�����ص㸳ֵ
 *  @param [in]x ��
 *  @param [in]y ��
 *  @param [in]value ���ص㴦��ֵ
 *  @return None
 */
void lcd_set_point(uint16_t y, uint16_t x, uint16_t value)
{
	lcd_set_cursor(y, x);
	lcd_ram_prepare();
	lcd_write_ram(value);	
    lcd_write_reg(0x0022,value);	
}


/**
 *  ���16x24�ַ�
 *  @param [in]x �ַ���������
 *  @param [in]y �ַ���������
 *  @param [in]ch �ַ�
 *  @param [in]ch_color �ַ���ɫ
 *  @param [in]bk_color ������ɫ
 *  @return None
 */
void lcd_put_char_16x24(uint16_t y, uint16_t x, uint8_t ch, uint16_t ch_color, uint16_t bk_color)
{
	uint16_t i,j;	
	uint16_t temp = 0;		
	
	for(i = 0; i < 24; i++)
	{
		temp = ASCII_16x24[((ch - 0x20) * 24) + i]; 
		for(j = 0; j < 16; j++)
		{
			if((temp >> j) & 0x01)			
			{
				lcd_set_point(y + i, x + j, ch_color); 
			}
			else  if(bk_color != HYALINE)			
			{				
				lcd_set_point(y + i, x + j, bk_color);  								
			}
		}
	}		
}



/**
 *  ���16x24�ַ���
 *  @param [in]x �ַ���������
 *  @param [in]y �ַ���������
 *  @param [in]*str �ַ���
 *  @param [in]ch_color �ַ���ɫ
 *  @param [in]bk_color ������ɫ
 *  @return None
 */
void  lcd_put_string_16x24(uint16_t y, uint16_t x, uint8_t *str, uint16_t ch_color, uint16_t bk_color)
{
	uint16_t i;
    
	for (i = 0; *str; i++)
	{
		lcd_put_char_16x24(y, x + (16*i), *str++, ch_color, bk_color);
	} 	
}



/**
 *  ���16x24����
 *  @param [in]x   ������
 *  @param [in]y   ������
 *  @param [in]dat ����
 *  @param [in]len ���ݳ���
 *  @param [in]ch_color �ַ���ɫ
 *  @param [in]bk_color ������ɫ
 *  @return None
 */
void lcd_put_data_16x24(uint16_t y, uint16_t x, uint32_t dat, uint8_t len, uint16_t ch_color, uint16_t bk_color) 
{
	uint8_t dat2str[11];
	lcd_data2string(dat, len, dat2str);
	lcd_put_string_16x24(y, x, dat2str, ch_color, bk_color);
}


/**< ˽�к��� */
/**
  * @brief  Writes data to the specified GPIO data port.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @param  PortVal: specifies the value to be written to the port output data register.
  * @retval None
  */
static void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  GPIOx->ODR = PortVal;
}


/**
 *  д�Ĵ���
 *  @param None
 *  @return None
 */
static void lcd_write_reg(uint16_t reg,uint16_t value)
{
	lcd_clr_cs();
	lcd_clr_rs();
	lcd_clr_wr();
	GPIO_Write(LCD_DATA_PORT, reg);
	lcd_set_wr();

	lcd_set_rs();
	lcd_clr_wr();
	GPIO_Write(LCD_DATA_PORT, value);
	lcd_set_wr();
	lcd_set_cs();
}

/**
 *  ׼����дRAM
 *  @param None
 *  @return None
 */
static void lcd_ram_prepare(void)
{
	lcd_clr_cs();
	lcd_clr_rs();
	lcd_clr_wr();
	GPIO_Write(LCD_DATA_PORT, 0x22);
	lcd_set_wr();
	lcd_set_cs();
}

/**
 *  дRAM
 *  @param [in]value д��ֵ
 *  @return None
 */
static void lcd_write_ram(uint16_t value)
{
	lcd_clr_cs();
	lcd_set_rs();
	lcd_clr_wr();
	GPIO_Write(LCD_DATA_PORT, value);
	lcd_set_wr();
	lcd_set_cs();
}

/**
 *  �����ص��BGR��ʽת��ΪRGB��ʽ(BBBBBGGGGGGRRRRR -> RRRRRGGGGGGBBBBB)
 *  @param  BGR��ʽ
 *  @return RGB��ʽ
 */
static uint16_t lcd_bgr2rgb(uint16_t c)
{
	uint16_t  b, g, r, rgb;
	
	r = (c>>0)  & 0x1f;
	g = (c>>5)  & 0x3f;
	b = (c>>11) & 0x1f;
	
	rgb =  (r << 11) + (g << 5) + (b << 0);
	
	return( rgb );
}

/**
 *  ������ת��Ϊ�ַ���
 *  @param [in]dat ת��ǰ������
 *  @param [in]len ���ݵĳ��� ����֧�� <= 10��
 *  @param [out]*str ת������ַ���
 *  @return None
 */
static void lcd_data2string(long dat, uint8_t len, uint8_t* str) 
{
    uint8_t i;
    uint8_t c;
    long d;
	
    if(dat < 0) 
    {
        *str++ = '-';
        dat = -dat;
    } 
    for(i = len; i > 0; i--)
    {
        d = LCD_POW10[i - 1];
        c = (uint8_t) (dat / d);
            dat -= c * d;
        *str++ = c + '0';
    }  
    *str = 0;
}
