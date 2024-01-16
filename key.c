#include "key.h"

/**
 * by forpaindream
 * 多个（两个）按键检测短按和长按事件
 * 短按：时间 10ms < T < 1 s, 长按：时间 T >1 s
 * 功能：使用状态机方式，扫描按键；扫描周期为10ms,10ms刚好跳过抖动；
 * 状态机使用switch case语句实现状态之间的跳转
 * lock变量用于判断是否是第一次进行按键确认状态
 * 长按键事件提前执行，短按键事件释放后才执行
 *
 * 特性：长按时不会触发短按的事件
 *
 * V1.0：创建代码和仓库
 * V1.1：修复了上个版本由于 按键释放时才检查两按键状态（此时两按键都是释放状态） 导致的按键Click返回值不正确的问题（20240116）
 */


KEY_STATE KeyState = KEY_CHECK;  // 初始化按键状态为检测状态

KEY_TYPE g_KeyActionFlag;    //用于区别长按和短按

KEY_VALUE g_KeyValue;

void Key_Scan(void)
{
  static uint8_t TimeCnt = 0;
  static uint8_t lock = 0;
  switch (KeyState) {
    //按键未按下状态，此时判断Key的值
    case KEY_CHECK:
      if (!Key)
      {
        if (1 == KEY0) KeyState = KEY_0_COMFIRM;  //如果按键Key值为0，说明按键开始按下，进入下一个状态
        if (1 == KEY1) KeyState = KEY_1_COMFIRM;  //如果按键Key值为0，说明按键开始按下，进入下一个状态
      }
      TimeCnt = 0;                  //计数复位
      lock = 0;
      break;

    case KEY_0_COMFIRM:
    case KEY_1_COMFIRM:
      //无论是KEY0还是KEY1 都代表有按键按下了 需要进一步处理
      if (!Key)                     //查看当前Key是否还是0，再次确认是否按下
      {
        if (!lock) lock = 1;

        TimeCnt++;

        /*按键时长判断*/
        if (TimeCnt > 100)            // 长按 1 s
        {
          g_KeyActionFlag = KEY_PRESSED;
          //判断按键依然按下
          if (0 == KEY0) g_KeyValue = KEY_0_PRESS;
          if (0 == KEY1) g_KeyValue = KEY_1_PPESS;
          TimeCnt = 0;
          lock = 0;               //重新检查
          KeyState = KEY_RELEASE;    // 需要进入按键释放状态
        }
      } else {
        if (1 == lock)                // 不是第一次进入，  释放按键才执行
        {
          //所以这里要判断按键被释放才行
          if (KeyState == KEY_0_COMFIRM) g_KeyValue = KEY_0_CLICK;
          if (KeyState == KEY_1_COMFIRM) g_KeyValue = KEY_1_CLICK;
          g_KeyActionFlag = KEY_PRESSED;          // 短按
          KeyState = KEY_RELEASE;    // 需要进入按键释放状态
        } else                          // 当前Key值为1，确认为抖动，则返回上一个状态
        {
          KeyState = KEY_CHECK;    // 返回上一个状态
          g_KeyValue = KEY_NULL;
        }
      }
      break;

    case KEY_RELEASE:
      if (Key) KeyState = KEY_CHECK;    //当前Key值为1，说明按键已经释放，返回开始状态
      break;

    default:
      break;
  }
}

//todo: 查出来了 是按键处理的问题 无论如何按键都不会进到KEY_0_CLICK
//修好后记得更新到github

void Key_Proc()
{
  switch (g_KeyValue) {
    case KEY_0_CLICK:
      /*
        执行短按对应的事件
      */
      //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
      g_KeyActionFlag = KEY_NOT_PRESSED;    //状态回到空
      g_KeyValue = KEY_NULL;
      break;

    case KEY_0_PRESS:
      /*
        执行长按对应的事件
      */
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
      g_KeyActionFlag = KEY_NOT_PRESSED;    //状态回到空
      g_KeyValue = KEY_NULL;
      break;

    case KEY_1_CLICK:
      /*
        执行短按对应的事件
      */
      //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
      g_KeyActionFlag = KEY_NOT_PRESSED;    //状态回到空
      g_KeyValue = KEY_NULL;
      break;


    case KEY_1_PPESS:
      /*
     执行长按对应的事件
     */
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
      g_KeyActionFlag = KEY_NOT_PRESSED;    //状态回到空
      g_KeyValue = KEY_NULL;
      break;
  }
}

