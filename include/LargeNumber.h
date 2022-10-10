#ifndef LARGENUMBER_H
#define LARGENUMBER_H
#define MAXN 10000
#include <string>

static short PREC = 10;

class LargeNumber {
public:

    short reversedRawNumber[MAXN + 2]{};
    short dotPos;
    short length;
    short neg;
    bool overflow;

    LargeNumber();
    explicit LargeNumber(std::string str);
    explicit LargeNumber(int num);
    explicit LargeNumber(double num);
    short at(int digit);
    std::string toString() const;
    short toShort() const;
    int toInt() const;

    void deleteZeros();

//    LargeNumber addZero() const;

    bool isZero() const;

    bool isInteger() const;

    LargeNumber negate() const;


    LargeNumber& operator=(const LargeNumber &a);
    bool operator==(const LargeNumber &a) const;
    bool operator<(const LargeNumber &a) const;
    bool operator<=(const LargeNumber &a) const;

    static void setPrecision(short prec);
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
    static LargeNumber log(const LargeNumber &a);
    static LargeNumber log(const LargeNumber &a, short precision);
    static LargeNumber ln(const LargeNumber &a);
    static LargeNumber ln(const LargeNumber &a, short precision);
    static LargeNumber sin(const LargeNumber &a);
    static LargeNumber cos(const LargeNumber &a);
    static LargeNumber tan(const LargeNumber &a);
    static LargeNumber factorial(const LargeNumber &a);
//    static LargeNumber abs(const LargeNumber &a);
    static LargeNumber mod(const LargeNumber &a, const LargeNumber &b);
    static LargeNumber gcd(const LargeNumber &a, const LargeNumber &b);
    static LargeNumber lcm(const LargeNumber &a, const LargeNumber &b);
//    static LargeNumber round(const LargeNumber &a);
    static LargeNumber floor(const LargeNumber &a);
    static LargeNumber ceil(const LargeNumber &a);
    static LargeNumber trunc(const LargeNumber &a);
    static LargeNumber frac(const LargeNumber &a);


};

#endif
