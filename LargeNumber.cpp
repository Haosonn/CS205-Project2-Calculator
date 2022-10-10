#include <regex>
#include <cmath>
#include <LargeNumber.h>
#define MAXL 10000

#include <iostream>
char* double2str(double num) {
    char* str = new char[100];
    sprintf(str, "%lf", num);
    return str;
}

LargeNumber::LargeNumber() {
    for(int i = 0; i < MAXL; i++)
        reversedRawNumber[i] = 0;
    dotposition = 0;
    length = 0;
    neg = 1;
    overflow = false;
}

LargeNumber::LargeNumber(int num) {
    for(int i = 0; i < MAXL; i++)
        reversedRawNumber[i] = 0;
    dotposition = 0;
    length = 0;
    neg = 1;
    overflow = false;
    if(!num) {
        length = 1;
        return;
    }
    if(num < 0) {
        neg = -1;
        num = -num;
    }
    length = 0;
    while(num) {
        reversedRawNumber[length++] = num % 10;
        num /= 10;
    }
    dotposition = 0;
    overflow = false;
}

LargeNumber::LargeNumber(double num) : LargeNumber(double2str(num)) {}

LargeNumber::LargeNumber(std::string str) {
    dotposition = 0;
    length = 0;
    neg = 1;
    overflow = false;
    bool dot = false;
    bool sci = false;
    bool firstNoneZero = false;
    int scineg = 1;
    int scitmp = 0;
    int i = 0;
    while (i < str.length()) {
        if(str[i] == '-') neg = -1;
        if(str[i] == '.') dot = true;
        if(str[i] == 'e') {
            sci = true;
            i++;
            break;
        }
        if(str[i] >= '0' && str[i] <= '9') {
            if(length >= MAXL) {
                overflow = true;
                return;
            }
            dotposition += dot ? 1 : 0;
            if(str[i] >= '1') firstNoneZero = true;
            else if(!firstNoneZero) {
                i++;
                continue;
            }
            reversedRawNumber[length++] = str[i] - '0';
        }
        i++;
    }
    if(sci) {
        while (i < str.length()) {
            if(str[i] == '-') scineg = -1;
            if(str[i] >= '0' && str[i] <= '9') {
                scitmp *= 10;
                scitmp += str[i] - '0';
                if (scitmp > MAXL) {
                    overflow = true;
                    return;
                }
            }
            i++;
        }
        dotposition -= scitmp * scineg;
    }
    for (i = 0; i < length / 2; i++) {
        std::swap(reversedRawNumber[i],reversedRawNumber[length - i - 1]);
    }
//    formatting();
}

std::string LargeNumber::toString() {
    std::string output;
    LargeNumber tmp = *this;
    if(tmp.isZero()) return "0";
    tmp.formatting();
    if (tmp.overflow) {
        output.append("Overflow!");
        return output;
    }
    if (tmp.neg == -1) output.append(std::string(1, '-'));
    if (tmp.dotposition >= tmp.length) {
        output.append("0.");
        for (int i = tmp.dotposition - tmp.length; i >= 1; i--)
            output.append(std::string(1, '0'));
    }
    for (int i = tmp.length - 1; i >= 0; i--) {
        char ch = (tmp.reversedRawNumber)[i] + '0';
        output.append(std::string(1, ch));
        if (tmp.dotposition > 0 && i == tmp.dotposition) output.append(".");
    }
    if (tmp.dotposition < 0)
        for (int i = tmp.dotposition; i < 0; i++)
            output.append(std::string(1, '0'));
    return output;
}

void LargeNumber::formatting() {
    if(isZero()) {
        neg = 1;
        dotposition = 0;
        length = 1;
        return;
    }
    if (dotposition < length)
        while(!reversedRawNumber[length - 1] && length) length--;
    int laggingZero = 0;
    while(!reversedRawNumber[laggingZero] && laggingZero < length) laggingZero++;
    length -= laggingZero;
    dotposition -= laggingZero;
    for(int i = 0; i < length; i++) {
        reversedRawNumber[i] = reversedRawNumber[i + laggingZero];
    }
    for(int i = length; i < length + laggingZero; i++) reversedRawNumber[i] = 0;
}
//delete leading zeros and lagging zeros

LargeNumber LargeNumber::addZero() const {
    LargeNumber res = *this;
    if(dotposition >= 0) return res;
    else {
        int zeroCnt = abs(dotposition);
        for (int i = length - 1; i >= 0; i--)
            res.reversedRawNumber[i + zeroCnt] = res.reversedRawNumber[i];
        for (int i = 0; i <= zeroCnt - 1; i++)
            res.reversedRawNumber[i] = 0;
        res.dotposition = 0;
        res.length += zeroCnt;
        return res;
    }
}
//add zeros if dotposition < 0 for add and sub

bool LargeNumber::isZero() const {
    LargeNumber tmp = *this;
    return (tmp.length < 1 && tmp.reversedRawNumber[0] == 0) || tmp.length == 0;
}



bool LargeNumber::isInteger() const {
    return dotposition <= 0;
}

LargeNumber& LargeNumber::operator= (const LargeNumber &a) {
    length = a.length;
    dotposition = a.dotposition;
    neg = a.neg;
    overflow = a.overflow;
    for (int i = 0; i < a.length; i++) {
        reversedRawNumber[i] = a.reversedRawNumber[i];
    }
    for (int i = a.length; i < 2 * a.length; i++) {
        reversedRawNumber[i] = 0;
    }
    return *this;
}

bool LargeNumber::operator== (const LargeNumber &a) const {
    LargeNumber tmpa = a;
    LargeNumber tmpb = *this;
    if (tmpa.neg != tmpb.neg) return false;
    if (tmpa.length - tmpa.dotposition != tmpb.length - tmpb.dotposition) return false;
    int i = tmpa.length - 1, j = tmpb.length - 1;
    while (true) {
        if (tmpa.reversedRawNumber[i] != tmpb.reversedRawNumber[j])
            return tmpa.reversedRawNumber[i] > tmpb.reversedRawNumber[j];
        i--; j--;
        if(i < 0 && j >= 0) return true;
        if(j < 0 && i >= 0) return false;
        if(i < 0 && j < 0) break;
    }
    return true;
}

bool LargeNumber::operator< (const LargeNumber &a) const {
    LargeNumber tmpa = a;
    LargeNumber tmpb = *this;
    tmpa.formatting();
    tmpb.formatting();
    if (tmpa.neg != tmpb.neg) return tmpb.neg == -1;
    if (tmpa.neg == -1 && tmpb.neg == -1) return !(tmpa.negate() < tmpb.negate());

    if (tmpa.length - tmpa.dotposition != tmpb.length - tmpb.dotposition) return tmpa.length - tmpa.dotposition > tmpb.length - tmpb.dotposition;

    tmpa = tmpa.addZero();
    tmpb = tmpb.addZero();

    int i = tmpa.length - 1, j = tmpb.length - 1;
    while (true) {
        if (tmpa.reversedRawNumber[i] != tmpb.reversedRawNumber[j])
            return tmpa.reversedRawNumber[i] > tmpb.reversedRawNumber[j];
        i--; j--;
        if(i < 0 && j >= 0) return true;
        if(j < 0 && i >= 0) return false;
        if(i < 0 && j < 0) break;
    }
    return false;
}

bool LargeNumber::operator<= (const LargeNumber &a) const {
    return *this < a || *this == a;
}

LargeNumber LargeNumber::negate() const {
    LargeNumber tmp = *this;
    tmp.neg = -tmp.neg;
    return tmp;
}

LargeNumber LargeNumber::approx(LargeNumber l1, short precision) {
    LargeNumber tmp = l1;
    if(l1.dotposition > precision) {
        short carry;
        short carryIdx;
        if(l1.length + 1 < l1.dotposition - precision) return LargeNumber(0);
        else {
            for (int i = l1.length; i < MAXL; i++) tmp.reversedRawNumber[i] = 0;
            carryIdx = std::max(-1, l1.dotposition - precision - 1);
            carry = (carryIdx == -1)? 0 : tmp.reversedRawNumber[carryIdx] >= 5;
            tmp.length -= l1.dotposition - precision;
            tmp.dotposition = precision;
            for(int i = 0; i < tmp.length; i++)
                tmp.reversedRawNumber[i] = l1.reversedRawNumber[i + l1.dotposition - precision];
            for(int i = tmp.length; i < l1.length; i++) tmp.reversedRawNumber[i] = 0;
            if(tmp.length == 0 && carry) {
                tmp.length = 1;
                tmp.reversedRawNumber[0] = 1;
                tmp.dotposition = 1;
            }
            else {
                carryIdx = 0;
                while (carry) {
                    tmp.reversedRawNumber[carryIdx] += carry;
                    if(tmp.reversedRawNumber[carryIdx] >= 10) {
                        tmp.reversedRawNumber[carryIdx] -= 10;
                        carry = 1;
                    }
                    else carry = 0;
                    carryIdx++;
                }
                tmp.length = std::max(tmp.length, (short) (carryIdx + 1));
            }
            return tmp;
        }
    }
    else return l1;

}

LargeNumber LargeNumber::calc(const LargeNumber &a, const LargeNumber &b, char op) {
    switch (op) {
        case '+':
            return add(a, b);
        case '-':
            return sub(b, a);
        case '*':
            return mul(a, b);
        case '/':
            try {
                return div(b, a);
            } catch (const char* msg) {
                throw msg;
            }
        case '^':
            return pow(b, a);
        default:
            return LargeNumber();
    }
}

LargeNumber LargeNumber::calc(const LargeNumber &a , char op) {
    switch (op) {
//        case 's':
//            return sin(a);
//        case 'c':
//            return cos(a);
//        case 't':
//            return tan(a);
        case 'n':
            return ln(a);
        case '!':
            return a.negate();
        case 'e':
            return exp(a);
        case 'r':
            return sqrt(a);
        default:
            return LargeNumber();
    }
}

LargeNumber LargeNumber::add(const LargeNumber &a, const LargeNumber &b) {
    LargeNumber largeNumber1 = a.addZero();
    LargeNumber largeNumber2 = b.addZero();
    LargeNumber res = LargeNumber();
    res.dotposition = std::max(largeNumber1.dotposition, largeNumber2.dotposition);
    if (largeNumber1.neg == -1 && largeNumber2.neg == -1) {
        return add(largeNumber1.negate(), largeNumber2.negate()).negate();
    }
    else if (largeNumber1.neg == -1) {
        return sub(b, largeNumber1.negate());
    }
    else if (largeNumber2.neg == -1) {
        return sub(a, largeNumber2.negate());
    }
    else {
        int i = 0;
        int carry = 0;
        int dotPositionDifference = largeNumber1.dotposition - largeNumber2.dotposition;
        if (dotPositionDifference > 0) {
            while (i < abs(dotPositionDifference)) {
                res.reversedRawNumber[res.length++] = largeNumber1.reversedRawNumber[i];
                i++;
            }
            while (i < largeNumber1.length || i - dotPositionDifference < largeNumber2.length || carry) {
                int tmp = carry;
                if (i < largeNumber1.length) tmp += largeNumber1.reversedRawNumber[i];
                if (i - dotPositionDifference < largeNumber2.length) tmp += largeNumber2.reversedRawNumber[i - dotPositionDifference];
                if (tmp >= 10) {
                    tmp -= 10;
                    carry = 1;
                } else carry = 0;
                res.reversedRawNumber[res.length++] = tmp;
                i++;
            }
        } else {
            carry = 0;
            while (i < abs(dotPositionDifference)) {
                res.reversedRawNumber[res.length++] = largeNumber2.reversedRawNumber[i] + carry;
                if (res.reversedRawNumber[res.length - 1] == 10) {
                    res.reversedRawNumber[res.length - 1] = 0;
                    carry = 1;
                } else
                    carry = 0;
                i++;
            }
            while (i + dotPositionDifference < largeNumber1.length || i < largeNumber2.length || carry) {
                int tmp = carry;
                if (i + dotPositionDifference < largeNumber1.length)
                    tmp += largeNumber1.reversedRawNumber[i + dotPositionDifference];
                if (i < largeNumber2.length) tmp += largeNumber2.reversedRawNumber[i];
                if (tmp >= 10) {
                    tmp -= 10;
                    carry = 1;
                } else carry = 0;
                res.reversedRawNumber[res.length++] = tmp;
                i++;
            }
        }
    }
    res.formatting();
    return res;
}

LargeNumber LargeNumber::sub(const LargeNumber &a, const LargeNumber &b) {
    LargeNumber largeNumber1 = a.addZero();
    LargeNumber largeNumber2 = b.addZero();
    LargeNumber res = LargeNumber();
    if (largeNumber1.neg != largeNumber2.neg) {
        if (largeNumber1.neg == -1) {
            return add(largeNumber1.negate(), b).negate();
        }
        else {
            return add(a, largeNumber2.negate());
        }
    }
    else if(largeNumber1.neg == -1 && largeNumber2.neg == -1) {
        return sub(largeNumber2.negate(), largeNumber1.negate());
    }
    else {
        if (largeNumber1 < largeNumber2) {
            res.neg = -1;
            std::swap(largeNumber1, largeNumber2);
        }
        int i = 0;
        int carry = 0;
        int dotPositionDifference = largeNumber1.dotposition - largeNumber2.dotposition;
        if (dotPositionDifference > 0) {
            while (i < abs(dotPositionDifference)) {
                res.reversedRawNumber[res.length++] = largeNumber1.reversedRawNumber[i];
                i++;
            }
            while (i < largeNumber1.length || i - dotPositionDifference < largeNumber2.length || carry) {
                int tmp = carry;
                if (i < largeNumber1.length) tmp += largeNumber1.reversedRawNumber[i];
                if (i - dotPositionDifference < largeNumber2.length) tmp -= largeNumber2.reversedRawNumber[i - dotPositionDifference];
                if (tmp < 0) {
                    tmp += 10;
                    carry = -1;
                }
                else carry = 0;
                res.reversedRawNumber[res.length++] = tmp;
                i++;
            }
        }
        else {
            carry = 0;
            while (i < abs(dotPositionDifference)) {
                res.reversedRawNumber[res.length++] = 10 - largeNumber2.reversedRawNumber[i] + carry;
                if(res.reversedRawNumber[res.length - 1] == 10) {
                    res.reversedRawNumber[res.length - 1] = 0;
                    carry = 0;
                }
                else
                    carry = -1;
                i++;
            }
            while (i + dotPositionDifference < largeNumber1.length || i < largeNumber2.length || carry) {
                int tmp = carry;
                if (i + dotPositionDifference < largeNumber1.length) tmp += largeNumber1.reversedRawNumber[i + dotPositionDifference];
                if (i < largeNumber2.length) tmp -= largeNumber2.reversedRawNumber[i];
                if (tmp < 0) {
                    tmp += 10;
                    carry = -1;
                }
                else carry = 0;
                res.reversedRawNumber[res.length++] = tmp;
                i++;
            }

        }
        res.dotposition = std::max(largeNumber1.dotposition, largeNumber2.dotposition);
        res.formatting();
        return res;
    }
}

LargeNumber LargeNumber::mul(const LargeNumber &a, const LargeNumber &b) {
    LargeNumber res = LargeNumber();

    if(a.isZero() || b.isZero()) return LargeNumber(0);

    if(a.overflow || b.overflow) {
        res.overflow = true;
        return res;
    }
    res.dotposition = a.dotposition + b.dotposition;
    res.length = a.length + b.length - 1;
    if (res.length > MAXL) {
        res.overflow = true;
        return res;
    }
    for (int i = 0; i < a.length ; i++) {
        for (int j = 0; j < b.length ; j++) {
            res.reversedRawNumber[i + j] += a.reversedRawNumber[i] * b.reversedRawNumber[j];
        }
    }
    for (int i = 0; i < a.length + b.length - 1; i++) {
        res.reversedRawNumber[i + 1] += res.reversedRawNumber[i] / 10;
        res.reversedRawNumber[i] %= 10;
    }
    if(res.reversedRawNumber[a.length + b.length - 1]) res.length++;
    if (res.length > MAXL) {
        res.overflow = true;
        return res;
    }
    res.neg = a.neg * b.neg;
    res.formatting();
    return res;
}

LargeNumber LargeNumber::div(const LargeNumber &a, const LargeNumber &b) {

    if(b.isZero()) throw "Divide by zero";
    if(a.isZero()) return LargeNumber(0);

    LargeNumber tmpb = b;
    tmpb.dotposition = 0;
    //ignore dotposition of b;
    LargeNumber res = LargeNumber();
    if(a.overflow || b.overflow) {
        res.overflow = true;
        return res;
    }
    res.neg = a.neg * b.neg;
    res.dotposition = a.dotposition - b.dotposition;
    res.length = a.length;
    int i = a.length - 1;
    LargeNumber tmp = LargeNumber(0);
    while (i >= 0) {
        tmp = mul(tmp,LargeNumber(10));
        tmp = add(tmp, LargeNumber((int)a.reversedRawNumber[i]));
        if (tmp < tmpb) {
            res.reversedRawNumber[i] = 0;
            i--;
            continue;
        }
        while (tmpb <= tmp) {
            tmp = sub(tmp,tmpb);
            res.reversedRawNumber[i]++;
        }
        i--;
    }
    if (res.length > MAXL) {
        res.overflow = true;
        return res;
    }
    res.formatting();
    return res;
}

LargeNumber LargeNumber::div(const LargeNumber &a, const LargeNumber &b, short precision) {
    if(a.isZero()) return LargeNumber(0);
    if(a.length > precision || b.length > precision) return div(a,b);
    else{
        LargeNumber tmp = a;
        for(int i = tmp.length - 1; i >= 0; i--)
            tmp.reversedRawNumber[i + precision] = tmp.reversedRawNumber[i];
        for(int i = 0; i < precision; i++)
            tmp.reversedRawNumber[i] = 0;
        tmp.length += precision;
        tmp.dotposition += precision;
        try {
            return div(tmp, b);
        }
        catch (const char* msg) {
            throw msg;
        }
    }
}

LargeNumber LargeNumber::sqrt(const LargeNumber &a) {
    if(a.neg == -1) {
        //todo throw exception
        throw "Sqrt of negative number";
    }
    else return pow(a, LargeNumber(0.5));
}

LargeNumber LargeNumber::exp(const LargeNumber &a) {
    return exp(a,20);
    //precision: 20 by default
}

LargeNumber LargeNumber::exp(const LargeNumber &a, short precision) {
    LargeNumber res = LargeNumber(1);
    if(a.isZero()) return res;
    if(a.overflow) {
        res.overflow = true;
        return res;
    }
    LargeNumber fac = LargeNumber(1); //factorial
    LargeNumber base = LargeNumber(1); //power of a
    for (int i = 1; i <= precision * 2; i++) {
        fac = mul(fac, LargeNumber(i));
        base = mul(base, a);
        base = approx(base, precision * 2);
        LargeNumber term = div(base, fac, precision * 2);
        res = add(res, term);
//        std::cout << i << " " << term.toString() << " " << res.toString() << std::endl;
    }
    return res;
}

LargeNumber LargeNumber::ln(const LargeNumber &a) {
    return approx(ln(a, 20),10);
    //precision:20 by default
}

LargeNumber LargeNumber::ln(const LargeNumber &a, short precision) {
    LargeNumber res = LargeNumber(0);
    if(a.isZero() || a.neg == -1) {
        throw "ln of negative number is undefined";
    }
    if(a.overflow) {
        res.overflow = true;
        return res;
    }

    //slow version (taylor expansion)
//    LargeNumber aMinusOne = sub(a, LargeNumber(1));
//    LargeNumber term = aMinusOne;
//    for (int i = 1; i <= 1000; i++) {
//        term = mul(term, aMinusOne);
//        if(i >= 2) term = mul(term, LargeNumber(i - 1));
//        term = div(term, LargeNumber(i), 20);
//        if(i % 2) res = add(res, term);
//        else res = sub(res, term);
//    }

    //fast version (\sigma \frac{2}{2k+1} (\frac{a - 1}{a + 1})^(2k+1))
    LargeNumber base = div(sub(a, LargeNumber(1)), add(a, LargeNumber(1)), precision * 2);
    LargeNumber baseSquare = mul(base, base);
    res = mul(base,LargeNumber(2));
    LargeNumber term = res;
    for(int i = 1; i <= precision * 2; i++) {
        term = mul(term, LargeNumber(2 * i - 1));
        term = div(term, LargeNumber(2 * i + 1), precision * 2);
        term = mul(term, baseSquare);
        term = approx(term, precision * 2);
        res = add(res, term);
        std::cout << i << " " << term.toString() << " " << res.toString() << std::endl;
    }

    return res;
}

LargeNumber LargeNumber::pow(const LargeNumber &a, const LargeNumber &b) {
    return pow(a, b, 20);
    //precision:20 by default
}

LargeNumber LargeNumber::pow(const LargeNumber &a, const LargeNumber &b, short precision) {
    LargeNumber res = exp(mul(b, ln(a,precision)));
    return res;
}


/*
LargeNumber LargeNumber::log(LargeNumber a) {
    LargeNumber res = LargeNumber();
    if(a.overflow) {
        res.overflow = true;
        return res;
    }
    if (a.neg == -1) {
        res.overflow = true;
        return res;
    }
    if (res.length > MAXL) {
        res.overflow = true;
        return res;
    }
    return res;
}

LargeNumber LargeNumber::sin(LargeNumber a) {
    LargeNumber res = LargeNumber();
    if(a.overflow) {
        res.overflow = true;
        return res;
    }
    if (res.length > MAXL) {
        res.overflow = true;
        return res;
    }
    return res;
}

LargeNumber LargeNumber::cos(LargeNumber a) {
    LargeNumber res = LargeNumber();
    if(a.overflow) {
        res.overflow = true;
        return res;
    }
    if (res.length > MAXL) {
        res.overflow = true;
        return res;
    }
    return res;
}


LargeNumber LargeNumber::tan(LargeNumber a) {
    LargeNumber res = LargeNumber();
    if(a.overflow) {
        res.overflow = true;
        return res;
    }
    if (res.length > MAXL) {
        res.overflow = true;
        return res;
    }
    return res;
}

 */


//bool ifValid(char *str) {
//    return regex_match(string(str), regex("-?\\d+\\.?\\d*(e-?\\d+)?"));
//}

/*
double str2double(char *str) {
    int i = 0;
    int length = strlen(str);
    double tmp = 0;
    int scitmp = 0;
    double dot = 0;
    int neg = 1;
    int scineg = 1;
    bool sci = false;
    while (i < strlen(str)) {
        if(str[i] == '-') neg = -1;
        if(str[i] == '.') dot = 10;
        if(str[i] == 'e') {
            sci = true;
            i++;
            break;
        }
        if(str[i] >= '0' && str[i] <= '9') {
            if (dot == 0) {
                tmp *= 10;
                tmp += str[i] - '0';
            }
            else {
                tmp += (str[i] - '0') / (double) (dot);
                dot *= (double) 10;
            }
        }
        i++;
    }
    if(sci) {
        while (i < strlen(str)) {
            if(str[i] == '-') scineg = -1;
            if(str[i] >= '0' && str[i] <= '9') {
                scitmp *= 10;
                scitmp += str[i] - '0';
            }
            i++;
        }
        tmp *= pow(10, scitmp* scineg);
    }
    return tmp * neg;
}
*/


