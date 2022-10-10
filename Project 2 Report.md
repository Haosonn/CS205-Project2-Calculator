# Project 2 Report

## 1.概述

​	本次Project 为实现一个具有赋值功能的科学计算器

## 2.特色功能

- 可以对变量赋值，变量可用作后续计算

- 对赋值，运算语句作先前的有效判定

- 可根据输入调整答案的精度

- 实现了以下函数的理论无限精度计算

  - exp
  - ln
  - log
  - pow
  - sqrt
  - sin
  - cos
  - tan

  



## 3.困难与解决办法

- 对LargeNumber这一高精度类维护涉及到无穷无尽的对数组操作的细节，经过细致的debug，和炒饭输入，将各类细节问题逐个解决。
- 判断表达式中负号为一元运算符或二元运算符
- 大量函数的重载