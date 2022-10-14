# Project 2 Report

## 1.概述

​	本次Project 为实现一个具有赋值功能的无限精度科学计算器，采用中序表达式，支持加减乘除，取余，阶乘，幂（支持浮点数），及无限精度各类常用函数。（无限精度指所有运算都依靠LargeNumber类，重载运算完成，预定义LargeNumber位数为10000）

```
Welcome to the calculator!
Type "\help" for help.
This is a calculator that can calculate expressions with variables and functions.
The supported functions are: sin, cos, tan, lg, ln, exp, sqrt.
The supported operators are: +, -, *, /, ^, !, %.
The supported constants are: pi, e.
The supported assignment expression is: var = expr.
You can set precision by typing prec = n, where n is the number of digits.
This will only be applied to the functions.
Otherwise, precision is set according to the maximum precision of your term.
Type "exit" to exit.
```

```
\\基础功能演示
Please enter your expression:3+4*5
 The answer is 23
 Please enter your expression:5*(3+4)
 The answer is 35
Please enter your expression:2^((5%3)!)
 The answer is 4
Please enter your expression:(3*(2.5+1.5))+3
 The answer is 15
 
```



## 2.特色功能

- 判断赋值/运算语句是否合法

  ```
  Please enter your expression:(1+2
   Unmatched bracket!
  Invalid expression!
  Please enter your expression:cube(3)
   Invalid/undefined variable/function name: cube
  Invalid expression!
  Please enter your expression:x+3
   Invalid/undefined variable/function name: x
  Invalid expression!
  Please enter your expression:3++4
   Invalid expression!
  ```
  
  
  
- 可以对变量赋值，变量可用作后续计算

  ```c++
  //结果演示
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
  
  //
  // 不允许对常量的赋值
  Please enter your expression: e = 3
   Cannot assign value to constant variable
  Please enter your expression: pi = 4
   Cannot assign value to constant variable
  ```

  

- 储存前一次计算答案，包括给变量赋值的值和表达式的计算值

  ```c++
  Please enter your expression:1.3*1.4
   The answer is 1.82
  Please enter your expression:5 * (ans + 0.18)
   The answer is 10
  Please enter your expression:a = ans^2
   Variable "a" assigned!
  Please enter your expression:a
   The answer is 100
  ```

  

- 可根据输入的小数位的最大位数调整答案的精度

  ```
  Please enter your expression:1.0/3
   The answer is 0.3
  Please enter your expression:1.0000/3.00
   The answer is 0.3333
  Please enter your expression:1/3.00000
   The answer is 0.33333
  ```

  

- 实现了以下函数的理论无限精度计算，通过对prec（默认为10）变量赋值设定函数运算的精度

  - exp
  
    ```
    Please enter your expression:exp(1)
     The answer is 2.7182818285
    Please enter your expression:prec = 30
     Precision set to 30
    Please enter your expression:exp(1)
     The answer is 2.718281828459045235360287471353
    //非整数幂运算演示
    Please enter your expression:2^ans
     The answer is 6.580885991017920970851542403888
    ```
  
    
  
  - ln, lg
  
    ```
    //1/2=0
    Please enter your expression:ln(1/2)
     Logarithm of non-positive number is undefined!
    Please enter your expression:ln(0)
     Logarithm of non-positive number is undefined!
    Please enter your expression:ln(-1)
     Logarithm of non-positive number is undefined!
    Please enter your expression:ln(2)
     The answer is 0.6931471806
    Please enter your expression:prec = 30
     Precision set to 30
    Please enter your expression:ln(2)
     The answer is 0.693147180559945309417232121458
    Please enter your expression:lg2 = ln(2)/ln(10)
     Variable "lg2" assigned!
    Please enter your expression:lg(2)
     The answer is 0.301029995663981195213738894724
    Please enter your expression:lg2
     The answer is 0.301029995663981195213738894724
    ```
  
  - pow
  
    ```c++
    Please enter your expression:8^(1.0000000000/3)
     The answer is 1.9999999999
    Please enter your expression:8^(ln(3)/ln(8))
     The answer is 3.0000000002
    Please enter your expression:prec = 100
     Precision set to 100
    Please enter your expression:e^pi
     The answer is 23.1406926327792690057290863679485473802661062425984839993951624456757932245
    38076300525100219106730342
    ```
  
    
  
  - sqrt
  
    ```
    Please enter your expression:prec = 100
     Precision set to 100
    Please enter your expression:sqrt(2)
     The answer is 1.41421356237309504880168872420969807856967187537694807317667973799073247846
    21070388503875343276415727
    ```
  
  - sin, cos, tan
  
    ```
    Please enter your expression:tan(pi/4)
     The answer is 1
    Please enter your expression:tan(pi/3)-sqrt(3)
     The answer is 0
    Please enter your expression:sin(pi/6)
     The answer is 0.5
    Please enter your expression:cos(pi/6)
     The answer is 0.8660254038
    Please enter your expression:sin(1)
     The answer is 0.8414709848
    ```
  
    
  
  



## 3.困难与解决办法

- 对赋值，运算语句作先前的有效判定和处理

  - 判断表达式中负号为一元运算符或二元运算符

    ```c++
    //首先将一元运算符的负号改写
    string convertMinusSign(string str) {
        string res;
        for(int i = 0; i < str.length(); i++) {
            if(str[i] == '-') {
                if(i == 0 || str[i - 1] == '(' ) {
                    res += '~';
                } else {
                    res += '-';
                }
            } else {
                res += str[i];
            }
        }
        return res;
    }
    ```

    

  - 判断处理函数的括号

  - 核心代码，通过栈的操作，将表达式转化为后序表达式

    ```
    LargeNumber evaluateExpression(string str) {
        // r = sqrt s = sin c = cos t = tan l = lg n = ln e = exp
        stack<char> operators;
        stack<LargeNumber> operands;
        str = removeSpace(str);
        str = convertMinusSign(str);
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == '(') operators.push('(');
            else if (str[i] == ')') {
                while (operators.top() != '(' && operators.top() != '[') {
                    char op = operators.top();
                    operatorHandling(op, operands, operators);
                }
                //判断处理函数的括号
                if(operators.top() == '[') {
                    operators.pop();
                    char op = operators.top();
                    operators.pop();
                    LargeNumber l = operands.top();
                    operands.pop();
                    operands.push(LargeNumber::calc(l, op));
                }
                else
                    operators.pop();
            } else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '^' || str[i] == '%') {
                while (!operators.empty() && preference(str[i]) <= preference(operators.top())) {
                    char op = operators.top();
                    operatorHandling(op, operands, operators);
                }
                operators.push(str[i]);
            } else if (str[i] == '~') {
                operators.push('~');
            } else if(str[i] == '!'){
                LargeNumber l = operands.top();
                operands.pop();
                operands.push(LargeNumber::factorial(l));
            } else {
                string tmp;
                while (i < str.length() && !isOperator(str[i]) ) {
                    tmp += str[i++];
                }
                i--;
                if(tmp == "ans") {
                    operands.push(ans);
                }
                else if (func.find(tmp) != func.end()) {
                    operators.push(func.at(tmp));
                    //将函数后的括号改写
                    operators.push('[');
                    i++;
                } else if (vars.find(tmp) != vars.end()) {
                    operands.push(vars.at(tmp));
                }
                else {
                    operands.push(LargeNumber(tmp));
                }
            }
        }
        while (!operators.empty()) {
            char op = operators.top();
            operatorHandling(op, operands, operators);
        }
        return operands.top();
    }
    
    ```

    

- 对LargeNumber这一高精度类维护涉及到无穷无尽的对数组操作的细节，尤其是边界条件。经过无尽的细致（折磨）的debug，和炒饭输入，将各类细节问题逐个解决。

- 大量函数的重载，整整800行代码手搓，每个操作运算模拟，非常锻炼码力，没有核心，但处处都是核心代码！

  ```c++
   //LargeNumber 头文件
  class LargeNumber {
  public:
      short reversedRawNumber[MAXN + 2]{};
      short dotPos;
      short length;
      short neg;
      bool overflow;
  
  //各种初始化函数
      LargeNumber();
      explicit LargeNumber(std::string str);
      explicit LargeNumber(int num);
      explicit LargeNumber(double num);
      
  //获取指定数位的数字
      short at(int digit);
  
  //字符串输出
      std::string toString() const;
      
      short toShort() const;
      int toInt() const;
  
  //去除前置0，后置0
      void deleteZeros();
      
      bool isZero() const;
      bool isInteger() const;
      
  //取负
      LargeNumber negate() const;
      
  //赋值重载
      LargeNumber& operator=(const LargeNumber &a);
      
  //一些运算符重载
      bool operator==(const LargeNumber &a) const;
      bool operator<(const LargeNumber &a) const;
      bool operator<=(const LargeNumber &a) const;
      
      static void setPrecision(short prec);
      
      //最折磨的函数，根据精度四舍五入
      static LargeNumber format(const LargeNumber &l, short precision) ; //round to precision digits
      
      static LargeNumber abs(const LargeNumber &l);
      static LargeNumber calc(const LargeNumber &a, const LargeNumber &b, char op);
      static LargeNumber calc(const LargeNumber &a, char op);
      static LargeNumber add(const LargeNumber &a, const LargeNumber &b);
      static LargeNumber sub(const LargeNumber &a, const LargeNumber &b);
      static LargeNumber mul(const LargeNumber &a, const LargeNumber &b);
      static LargeNumber div(const LargeNumber &a, const LargeNumber &b);
      static LargeNumber div(const LargeNumber &a, const LargeNumber &b, short precision);
      static LargeNumber pow(const LargeNumber &a, const LargeNumber &b, short precision);
      static LargeNumber pow(const LargeNumber &a, const LargeNumber &b);
      static LargeNumber exp(const LargeNumber &a);
      static LargeNumber exp(const LargeNumber &a, short precision);
      static LargeNumber sqrt(const LargeNumber &a);
      static LargeNumber sqrt(const LargeNumber &a, short precision);
      static LargeNumber lg(const LargeNumber &a);
      static LargeNumber log(const LargeNumber &a, short precision);
      static LargeNumber ln(const LargeNumber &a);
      static LargeNumber ln(const LargeNumber &a, short precision);
      static LargeNumber sin(const LargeNumber &a);
      static LargeNumber sin(const LargeNumber &a, short precision);
      static LargeNumber cos(const LargeNumber &a);
      static LargeNumber cos(const LargeNumber &a, short precision);
      static LargeNumber tan(const LargeNumber &a);
      static LargeNumber tan(const LargeNumber &a, short precision);
      static LargeNumber factorial(const LargeNumber &a);
      static LargeNumber mod(const LargeNumber &a, const LargeNumber &b);
  
  };
  
  
  ```
  
  

## 4.迫于时间没有完成的地方

- 大数乘法，除法太慢，可以优化
- 加入表达式计算加入多元函数
- 定义函数，并允许后续调用