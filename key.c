#include "key.h"

/**
 * 多个（两个）按键检测短按和长按事件
 * 短按：时间 10ms < T < 1 s, 长按：时间 T >1 s
 * 功能：使用状态机方式，扫描按键；扫描周期为10ms,10ms刚好跳过抖动；
 * 状态机使用switch case语句实现状态之间的跳转
 * lock变量用于判断是否是第一次进行按键确认状态
 * 长按键事件提前执行，短按键事件释放后才执行
 *
 * 特性：长按时不会触发短按的事件
 * based on https://blog.csdn.net/qq_34142812/article/details/119721386
 */

KEY_STATE KeyState = KEY_CHECK;  // 初始化按键状态为检测状态

KEY_TYPE g_KeyActionFlag;    //用于区别长按和短按

KEY_VALUE g_KeyValue;

void Key_Scan(void)
{
  static uint8_t TimeCnt = 0;
  static uint8_t lock = 0;
  switch (KeyState)
  {
    //按键未按下状态，此时判断Key的值
    case KEY_CHECK:
      if (!Key) KeyState = KEY_COMFIRM;  //如果按键Key值为0，说明按键开始按下，进入下一个状态
      TimeCnt = 0;                  //计数复位
      lock = 0;
      break;

    case KEY_COMFIRM:
      if (!Key)                     //查看当前Key是否还是0，再次确认是否按下
      {
        if (!lock) lock = 1;

        TimeCnt++;

        /*按键时长判断*/
        if (TimeCnt > 100)            // 长按 1 s
        {
          g_KeyActionFlag = LONG_KEY;
          //判断按键依然按下
          if (0 == KEY0) g_KeyValue = KEY_0;
          if (0 == KEY1) g_KeyValue = KEY_1;
          TimeCnt = 0;
          lock = 0;               //重新检查
          KeyState = KEY_RELEASE;    // 需要进入按键释放状态
        }
      } else
      {
        if (1 == lock)                // 不是第一次进入，  释放按键才执行
        {
          //所以这里要判断按键被释放才行
          if (1 == KEY0) g_KeyValue = KEY_0;
          if (1 == KEY1) g_KeyValue = KEY_1;
          g_KeyActionFlag = SHORT_KEY;          // 短按
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

void Key_Proc()
{
  switch (g_KeyValue)
  {
    case KEY_0:
      switch (g_KeyActionFlag)
      {
        case SHORT_KEY:
          /*
          执行短按对应的事件
          */
          g_KeyActionFlag = NULL_KEY;    //状态回到空
          break;

        case LONG_KEY:
          /*
         执行长按对应的事件
         */
          g_KeyActionFlag = NULL_KEY;    //状态回到空
        default:
          break;
      }
      g_KeyValue = KEY_NULL;
      break;

    case KEY_1:
      switch (g_KeyActionFlag)
      {
        case SHORT_KEY:
          /*
          执行短按对应的事件
          */
          g_KeyActionFlag = NULL_KEY;    //状态回到空
          break;

        case LONG_KEY:
          /*
         执行长按对应的事件
         */
          g_KeyActionFlag = NULL_KEY;    //状态回到空
        default:
          break;
      }
      g_KeyValue = KEY_NULL;
      break;
  }
}

