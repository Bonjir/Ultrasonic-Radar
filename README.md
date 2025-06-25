# 超声波雷达 - ATmega8A

### 介绍

这是一个大一学生的电子设计实践基础课程的综合设计大作业，所具功能为通过超声波和舵机等模块实现2维、3维的雷达，全部项目完成耗时26天。

本项目主体部分是基于ATmega8A 芯片的C++程序，上位机部分是基于OpenGL图形库实现的图形化界面程序。

### 效果展示

<img src="https://github.com/Bonjir/Ultrasonic-Radar/blob/main/.images/Readme/menu.jpg" width="75%" height="75%" />
*菜单界面*

<img src="https://github.com/Bonjir/Ultrasonic-Radar/blob/main/.images/Readme/2d-radar.jpg" width="75%" height="75%" />
*2D雷达*

<img src="https://github.com/Bonjir/Ultrasonic-Radar/blob/main/.images/Readme/3d-radar.jpg" width="75%" height="75%" />
*3D雷达上位机*

### 模块列表

- 超声波 HC-SR04 模块

- 舵机 MG90S 模块

- OLED 屏 12864 模块
- 按键阵列
- CH340G 串口模块

### 项目特点

本项目着力优化代码性能、代码效果以及代码大小，尽最大可能将尽量多的功能放在一个程序文件中，同时最大化利用 MCU 的各个存储单元，如 Flash、 EEPROM、SRAM。由于本项目所用芯片的存储大小有限，在部分代码中采取了以时间换取空间的做法，以减小空间占用。

本项目还设计并打印了舵机和超声波模块的外壳，相较于已有的二自由度舵机平台可以转动更大的角度，同时可以方便地拆卸和携带，具有更高的灵活度和便携性。

### License

MIT License

### 参与贡献

软件设计：瓶装蛋糕  
程序架构：瓶装蛋糕  
项目构思：瓶装蛋糕  
硬件设计：瓶装蛋糕  
文档编写：瓶装蛋糕  
指导教师：lyh  
