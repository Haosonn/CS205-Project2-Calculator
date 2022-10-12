# Project 2 Report

## 1.概述

​	本次Project 为实现一个具有赋值功能的科学计算器

```C++
\\程序初始时演示
Welcome to the calculator!
Type "\help" for help.
Please enter your expression:\help
 This is a calculator that can calculate expressions with variables and functions.
The supported functions are: sin, cos, tan, log, ln, exp, sqrt.
The supported operators are: +, -, *, /, ^, ~.
The supported constants are: pi, e.
The supported assignment expression is: var = expr.
```



## 2.特色功能

- 可以对变量赋值，变量可用作后续计算

  ```c++
  //结果演示
  Welcome to the calculator!
  Please enter your expression: a = 2 * 3
   Variable a assigned!
  Please enter your expression: b = a * a
   Variable b assigned!
  Please enter your expression: a * b
   The answer is 216
  Please enter your expression: a = a * a
   Variable a reassigned!
  Please enter your expression: a
   The answer is 36
  
  // 不允许对常量的赋值
  Please enter your expression: e = 3
   Cannot assign value to constant variable
  Please enter your expression: pi = 4
   Cannot assign value to constant variable
  ```

  

- 储存前一次计算答案，包括给变量赋值的值和表达式的计算值

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