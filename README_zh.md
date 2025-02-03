# LTC4150 模块
[English](README.md) | 简体中文

---
## 环境与相关引脚：
该程序基于*STM32F103*，其中 **INT** 引脚为 PB7，**POL** 引脚为 PB6，**SHDN** 引脚为 PB5。

## 功能：
通过输入 电池总毫安时(mAh) 和 目前电量百分比(%)，来计算 电池剩余电量 以及 电流大小（可选），本人还提供了 IIC 通信的 OLED 屏幕来显示数据。

## 说明：
该程序由官方提供的 Arduino 代码移植到 STM32F103。

## 注意事项：
该程序使用中断，因此你的 LTC4150 上需要将 **SJ1闭合**。

如果你需要输出电压设置为 3.3V，请将 **SJ2 和 SJ3 闭合**，5V 则打开即可。

## 参考：
##### https://github.com/sparkfun/LTC4150_Coulomb_Counter_BOB
##### https://learn.sparkfun.com/tutorials/ltc4150-coulomb-counter-hookup-guide#solderjumpers
