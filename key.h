//
// Created by Fir on 2023/2/8.
//

#ifndef KEY_H
#define KEY_H

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
  KEY_COMFIRM = 1,
  KEY_RELEASE = 2
} KEY_STATE;

typedef enum
{
  NULL_KEY = 0,
  SHORT_KEY = 1,
  LONG_KEY
} KEY_TYPE;

//对应的按键值，
typedef enum
{
  KEY_NULL = 0,
  KEY_0,
  KEY_1,
} KEY_VALUE;

extern void Key_Scan(void);

extern void Key_Proc(void);

extern uint8_t g_KeyFlag;                // 按键有效标志，0： 按键值无效； 1：按键值有效

#ifdef __cplusplus
}
#endif

#endif //KEY_H
