# LTC4150模块
---
## 环境与相关引脚：
#### 该程序基于*STM32F103*，其中INT引脚为PB7，POL引脚为PB6，SHDN引脚为PB5
## 功能：
#### 通过输入电池总mAh和目前电量百分比%，来计算电池剩余电量以 及 电流大小，本人还提供了IIC通信的OLED屏幕显示数据
## 说明：
#### 该程序由官方提供的Ardunio移植到STM32F103
## 注意事项：
#### 该程序使用中断，因此你的LTC4150上需要将SJ1闭合。如果你需要输出电压设置为3.3V，请将SJ2和SJ3闭合

## Reference
#### (https://github.com/sparkfun/LTC4150_Coulomb_Counter_BOB)
#### (https://learn.sparkfun.com/tutorials/ltc4150-coulomb-counter-hookup-guide#solderjumpers)
