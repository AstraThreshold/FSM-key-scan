//
// Created by Fir on 2023/2/8.
//

#ifndef OLED_KEY_FIR_H
#define OLED_KEY_FIR_H

#include "../../3rdParty/u8g2/u8g2.h"
#include "stm32f1xx.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  * 使用定时器(SysTick)来轮询Key_Scan()函数
  * SysTick轮询时间为1ms 建立变量 每轮询一次+1 大于10时执行一次Key_Scan() 然后清零变量
  * 状态转换时间为10ms,即每次进入switch case语句的时间差为10ms
  * 利用该10ms的间隙跳过按键抖动
  */

//Key: 1:高电平，按键未按下， 0：低电平，按键按下
#define KEY0 HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)
#define KEY1 HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)
#define Key (KEY0 && KEY1)

typedef enum
{
  KEY_CHECK = 0,
  KEY_0_COMFIRM,
  KEY_1_COMFIRM,
  KEY_RELEASE,
} KEY_STATE;

typedef enum
{
  KEY_NOT_PRESSED = 0,
  KEY_PRESSED,
} KEY_TYPE;

//对应的按键值，
typedef enum
{
  KEY_NULL = 0,
  KEY_0_CLICK,  //轻击
  KEY_0_PRESS,  //长按
  KEY_1_CLICK,
  KEY_1_PPESS,
} KEY_VALUE;

extern KEY_TYPE g_KeyActionFlag;

extern KEY_VALUE g_KeyValue;

extern void Key_Scan(void);

extern void Key_Proc(void);

extern uint8_t g_KeyFlag;                // 按键有效标志，0： 按键值无效； 1：按键值有效

#ifdef __cplusplus
}
#endif

#endif //OLED_KEY_FIR_H
