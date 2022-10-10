#include <regex>
#include <LargeNumber.h>
#define MAXL 10000

#include <iostream>


char* double2str(double num) {
    char* str = new char[100];
    sprintf(str, "%lf", num);
    return str;
}



//0 by default
LargeNumber::LargeNumber() {
    reversedRawNumber[0] = 0;
    dotPos = 0;
    length = 1;
    neg = 1;
    overflow = false;
}

LargeNumber::LargeNumber(int num) {
    dotPos = 0;
    length = 0;
    neg = 1;
    overflow = false;
    if(!num) {
        reversedRawNumber[0] = 0;
        length = 1;
        return;
    }
    if(num < 0) {
        neg = -1;
        num = -num;
    }
    length = 0;
    while(num) {
        reversedRawNumber[length++] =  (short) (num % 10);
        num /= 10;
    }
    dotPos = 0;
    overflow = false;
}

LargeNumber::LargeNumber(double num) : LargeNumber(double2str(num)) {}

LargeNumber::LargeNumber(std::string str) {
    dotPos = 0;
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
            dotPos += dot ? 1 : 0;
            if(str[i] >= '1') firstNoneZero = true;
            else if(!firstNoneZero) {
                i++;
                continue;
            }
            reversedRawNumber[length++] = (short) (str[i] - '0');
        }
        i++;
    }
    if(!firstNoneZero) {
        reversedRawNumber[0] = 0;
        length = 1;
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
        dotPos -= (scitmp * scineg);
    }
    for (i = 0; i < length / 2; i++) {
        std::swap(reversedRawNumber[i],reversedRawNumber[length - i - 1]);
    }
}

//return the digit at the position
//digit == x means the digit at the place denoting 10^x
short LargeNumber::at(int digit) {
    int high = length - 1 - dotPos;
    int low = -dotPos;
    if(digit > high || digit < low) return 0;
    return reversedRawNumber[digit + dotPos];
}

std::string LargeNumber::toString() const {
    std::string output;
    LargeNumber tmp = *this;
    tmp.deleteZeros();
    if(tmp.isZero()) return "0";
//    tmp.deleteZeros();
    if (tmp.overflow) {
        output.append("Overflow!");
        return output;
    }
    if (tmp.neg == -1) output.append(std::string(1, '-'));
    if (tmp.dotPos >= tmp.length) {
        output.append("0.");
        for (int i = tmp.dotPos - tmp.length; i >= 1; i--)
            output.append(std::string(1, '0'));
    }
    for (int i = tmp.length - 1; i >= 0; i--) {
        char ch = (char) (tmp.reversedRawNumber[i] + '0');
        output.append(std::string(1, ch));
        if (tmp.dotPos > 0 && i == tmp.dotPos) output.append(".");
    }
    if (tmp.dotPos < 0)
        for (int i = tmp.dotPos; i < 0; i++)
            output.append(std::string(1, '0'));
    return output;
}

short LargeNumber::toShort() const {
    if (overflow) return 0;
    short res = 0;
    for (int i = length - 1; i >= 0; i--) {
        res *= 10;
        res += reversedRawNumber[i];
    }
    return res * neg;
}

int LargeNumber::toInt() const {
    if (overflow) return 0;
    int res = 0;
    for (int i = length - 1; i >= 0; i--) {
        res *= 10;
        res += reversedRawNumber[i];
    }
    return res * neg;
}

void LargeNumber::deleteZeros() {
    if(isZero()) {
        neg = 1;
        dotPos = 0;
        length = 1;
        return;
    }
    //remove the leading zeros
    while(!reversedRawNumber[length - 1] && length) length--;
    if(!length) {
        dotPos = 0;
        length = 1;
        return;
    }
    //remove the trailing zeros
    int trailingZero = 0;
    while(!reversedRawNumber[trailingZero] && trailingZero < length) trailingZero++;
    if(!trailingZero) return;
    length -= trailingZero;
    dotPos -= trailingZero;
    for(int i = 0; i < length; i++) {
        reversedRawNumber[i] = reversedRawNumber[i + trailingZero];
    }
    for(int i = length; i < length + trailingZero; i++) reversedRawNumber[i] = 0;
}
//delete leading zeros and lagging zeros

//LargeNumber LargeNumber::addZero() const {
//    LargeNumber res = *this;
//    if(dotPos >= 0) return res;
//    else {
//        int zeroCnt = abs(dotPos);
//        for (int i = length - 1; i >= 0; i--)
//            res.reversedRawNumber[i + zeroCnt] = res.reversedRawNumber[i];
//        for (int i = 0; i <= zeroCnt - 1; i++)
//            res.reversedRawNumber[i] = 0;
//        res.dotPos = 0;
//        res.length += zeroCnt;
//        return res;
//    }
//}
//add zeros if dotPos < 0 for add and sub

bool LargeNumber::isZero() const {
    LargeNumber tmp = *this;
    if (tmp.length == 1 && tmp.reversedRawNumber[0] == 0) return true;
    else {
        for (int i = 0; i < tmp.length; i++)
            if (tmp.reversedRawNumber[i] != 0) return false;
        return true;
    }
}



bool LargeNumber::isInteger() const {
    return dotPos <= 0;
}

LargeNumber& LargeNumber::operator= (const LargeNumber &a) {
    length = a.length;
    dotPos = a.dotPos;
    neg = a.neg;
    overflow = a.overflow;
    for (int i = 0; i < a.length; i++) {
        reversedRawNumber[i] = a.reversedRawNumber[i];
    }
    return *this;
}

bool LargeNumber::operator== (const LargeNumber &a) const {
    LargeNumber tmpa = a;
    LargeNumber tmpb = *this;
    tmpa.deleteZeros();
    tmpb.deleteZeros();
    if (tmpa.neg != tmpb.neg) return false;
    if (tmpa.length  != tmpb.length) return false;
    int i = tmpa.length - 1;
    while (i >= 0) {
        if (tmpa.reversedRawNumber[i] != tmpb.reversedRawNumber[i])
            return tmpa.reversedRawNumber[i] > tmpb.reversedRawNumber[i];
        i--;
    }
    return true;
}

bool LargeNumber::operator< (const LargeNumber &a) const {
    LargeNumber tmpa = a;
    LargeNumber tmpb = *this;
    tmpa.deleteZeros();
    tmpb.deleteZeros();
    if (tmpa.neg != tmpb.neg) return tmpb.neg == -1;
    if (tmpa.neg == -1 && tmpb.neg == -1) return !(tmpa.negate() < tmpb.negate());

    //now both are positive
    //calculate the highest and lowest digit of the two numbers
    int aHigh = tmpa.length - 1 - tmpa.dotPos;
    int bHigh = tmpb.length - 1 - tmpb.dotPos;
    int aLow = -tmpa.dotPos;
    int bLow = -tmpb.dotPos;

    if (aHigh != bHigh) return aHigh > bHigh;

    int i = aHigh;
    while (true) {
        if (tmpa.at(i) != tmpb.at(i)) return tmpa.at(i) > tmpb.at(i);
        if (i < aLow && i < bLow) break;
        i--;
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

void LargeNumber::setPrecision(short prec) {
    PREC = prec;
}

//formatting LargeNumber according to precision
LargeNumber LargeNumber::format(const LargeNumber &l, short precision) {
    LargeNumber tmp = l;
    int high = tmp.length - 1 - tmp.dotPos;
    int low = -tmp.dotPos;
    if (precision > -low) {
        // add zeros
        int zeroCnt = precision + low;
        for (int i = tmp.length - 1; i >= 0; i--)
            tmp.reversedRawNumber[i + zeroCnt] = tmp.reversedRawNumber[i];
        for (int i = 0; i <= zeroCnt - 1; i++)
            tmp.reversedRawNumber[i] = 0;
        tmp.length += zeroCnt;
        tmp.dotPos = precision;
        return tmp;
    }
    else {
        short carry;
        carry = tmp.at(-(precision + 1)) >= 5;
        tmp.length -= tmp.dotPos - precision;
        if(!tmp.length) tmp.reversedRawNumber[0] = 0;
        for(int i = 0; i < tmp.length; i++)
            tmp.reversedRawNumber[i] = tmp.reversedRawNumber[i + tmp.dotPos - precision];
        tmp.dotPos = precision;
//        for(int i = tmp.length; i < l.length; i++) tmp.reversedRawNumber[i] = 0;
        int carryIdx = 0;
        while (carry) {
            tmp.reversedRawNumber[carryIdx] += carry;
            if(tmp.reversedRawNumber[carryIdx] >= 10) {
                tmp.reversedRawNumber[carryIdx] -= 10;
                carry = 1;
            }
            else carry = 0;
            carryIdx++;
        }
        carryIdx--;
        tmp.length = std::max(tmp.length, (short) (carryIdx + 1));
        return tmp;
    }
}

LargeNumber LargeNumber::abs(const LargeNumber &l) {
    LargeNumber tmp = l;
    tmp.neg = 1;
    return tmp;
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
    LargeNumber l1 = a;
    LargeNumber l2 = b;
    l1.deleteZeros();
    l2.deleteZeros();
    LargeNumber res = LargeNumber();
    if (l1.neg == -1 && l2.neg == -1) {
        return add(l1.negate(), l2.negate()).negate();
    }
    else if (l1.neg == -1) {
        return sub(b, l1.negate());
    }
    else if (l2.neg == -1) {
        return sub(a, l2.negate());
    }
    //now both are positive
    else {
        res.dotPos = std::max(l1.dotPos, l2.dotPos);
        int aHigh = l1.length - 1 - l1.dotPos;
        int bHigh = l2.length - 1 - l2.dotPos;
        int aLow = -l1.dotPos;
        int bLow = -l2.dotPos;
        int resHigh = std::max(aHigh, bHigh);
        int resLow = std::min(aLow, bLow);
        int i = 0;
        int resCurrentDigit = resLow;
        int carry = 0;
        while(true) {
            int tmp = l1.at(resCurrentDigit) + l2.at(resCurrentDigit) + carry;
            if (tmp >= 10) {
                carry = 1;
                tmp -= 10;
            }
            else carry = 0;
            res.reversedRawNumber[i++] = tmp;
            resCurrentDigit++;
            if (resCurrentDigit > resHigh) break;
        }
        i--;
        if(carry) res.reversedRawNumber[++i] = 1;
        res.length = i + 1;
    }
    res.deleteZeros();
    return res;
}

LargeNumber LargeNumber::sub(const LargeNumber &a, const LargeNumber &b) {
    LargeNumber l1 = a;
    LargeNumber l2 = b;
    l1.deleteZeros();
    l2.deleteZeros();
    LargeNumber res = LargeNumber();
    if (l1.neg != l2.neg) {
        if (l1.neg == -1) {
            return add(l1.negate(), b).negate();
        }
        else {
            return add(a, l2.negate());
        }
    }
    else if(l1.neg == -1 && l2.neg == -1) {
        return sub(l2.negate(), l1.negate());
    }
    //now both are positive
    else {
        if (l1 < l2) {
            res.neg = -1;
            std::swap(l1, l2);
        }
        //now l1 >= l2
        res.dotPos = std::max(l1.dotPos, l2.dotPos);
        int aHigh = l1.length - 1 - l1.dotPos;
        int bHigh = l2.length - 1 - l2.dotPos;
        int aLow = -l1.dotPos;
        int bLow = -l2.dotPos;
        int resHigh = std::max(aHigh, bHigh);
        int resLow = std::min(aLow, bLow);
        int i = 0;
        int resCurrentDigit = resLow;
        int carry = 0;
        while(true) {
            int tmp = l1.at(resCurrentDigit) - l2.at(resCurrentDigit) + carry;
            if (tmp < 0) {
                carry = -1;
                tmp += 10;
            }
            else carry = 0;
            res.reversedRawNumber[i++] = tmp;
            resCurrentDigit++;
            if (resCurrentDigit > resHigh) break;
        }
        i--;
        res.length = i + 1;
    }
    res.deleteZeros();
    return res;
}

LargeNumber LargeNumber::mul(const LargeNumber &a, const LargeNumber &b) {
    LargeNumber res = LargeNumber();

    if(a.isZero() || b.isZero()) return LargeNumber(0);

    if(a.overflow || b.overflow) {
        res.overflow = true;
        return res;
    }
    res.dotPos = a.dotPos + b.dotPos;
    res.length = a.length + b.length - 1;
    if (res.length > MAXL) {
        res.overflow = true;
        return res;
    }
    for (int i = 0; i <= res.length; i++)
        res.reversedRawNumber[i] = 0;
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
    res.deleteZeros();
    return res;
}

LargeNumber LargeNumber::div(const LargeNumber &a, const LargeNumber &b) {
    if(b.isZero()) throw "Divide by zero";
    if(a.isZero()) return LargeNumber(0);
    LargeNumber tmpa = a;
    LargeNumber tmpb = b;
    LargeNumber res = LargeNumber();
    if(!tmpa.isInteger() || !tmpb.isInteger()) {
        int precision = std::max(std::max(tmpa.dotPos, tmpb.dotPos), (short)0);
        tmpa = format(tmpa, (short)precision + std::max(tmpb.length - tmpa.length, 0) + 1);
        int tmpDotPos = tmpa.dotPos - tmpb.dotPos;
        tmpa.dotPos = 0;
        tmpb.dotPos = 0;
        res = div(tmpa, tmpb);
        res.dotPos += tmpDotPos;
        res = format(res, (short)precision);
        return res;
    }
    //ignore dotPos of b;
    if(a.overflow || b.overflow) {
        res.overflow = true;
        return res;
    }
    res.neg = a.neg * b.neg;
    res.length = a.length;
    int i = a.length - 1;
    LargeNumber tmp = LargeNumber(0);
    while (i >= 0) {
        res.reversedRawNumber[i] = 0;
        tmp = mul(tmp,LargeNumber(10));
        tmp = add(tmp, LargeNumber((int)a.reversedRawNumber[i]));
        if (tmp < tmpb) {
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
    res.deleteZeros();
    return res;
}

//division according to precision
LargeNumber LargeNumber::div(const LargeNumber &a, const LargeNumber &b, short precision) {
    if(a.isZero()) return LargeNumber((int)0);
    if(a.dotPos > precision || b.dotPos > precision) return div(a, b);
    else{
        LargeNumber res = LargeNumber();
        LargeNumber tmp = format(a, precision);
        try {
            res = div(tmp, b);
            return res;
        }
        catch (const char* msg) {
            throw msg;
        }
    }
}

LargeNumber LargeNumber::sqrt(const LargeNumber &a, short precision) {
    if(a.neg == -1) {
        throw "Sqrt of negative number";
    }
    else return pow(a, LargeNumber(0.5), precision);
}

LargeNumber LargeNumber::sqrt(const LargeNumber &a) {
    return sqrt(a, PREC);
}

LargeNumber LargeNumber::exp(const LargeNumber &a) {
    return exp(a,PREC);
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
    LargeNumber diff = LargeNumber(1);
    diff.dotPos = precision + 1;
    for (int i = 1; ; i++) {
        fac = mul(fac, LargeNumber(i));
        base = mul(base, a);
        LargeNumber term = div(base, fac, precision + 1);
        term = format(term, precision + 1);
        if (abs(term) <= diff) break;
        res = add(res, term);
        std::cout << i << " " << term.toString() << " " << res.toString() << std::endl;
    }

    res = format(res, precision);
    return res;
}

LargeNumber LargeNumber::ln(const LargeNumber &a) {
    return format(ln(a, PREC + 5), PREC);
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
    LargeNumber diff = LargeNumber(1);
    diff.dotPos = precision + 1;
    for(int i = 1; ; i++) {
        term = mul(term, LargeNumber(2 * i - 1));
        term = div(term, LargeNumber(2 * i + 1), precision + 1);
        term = mul(term, baseSquare);
        term = format(term, precision + 1);
        if(abs(term) <= diff) break;
        res = add(res, term);
        std::cout << i << " " << term.toString() << " " << res.toString() << std::endl;
    }

    res = format(res, precision);
    return res;
}

LargeNumber LargeNumber::pow(const LargeNumber &a, const LargeNumber &b) {
    if(b.isInteger() && b.neg == 1) {
        int p = b.toInt();
        LargeNumber res = LargeNumber(1);
        LargeNumber base = a;
        while(p) {
            if(p % 2) res = mul(res, base);
            base = mul(base, base);
            base = format(base, PREC);
            res = format(res, PREC);
            p >>= 1;
        }
        return format(res, PREC);
    }
    return format(pow(a, b,PREC + 10), PREC);
}

LargeNumber LargeNumber::pow(const LargeNumber &a, const LargeNumber &b, short precision) {
    LargeNumber res = exp(format(mul(b, ln(a,precision + 5)),precision + 5), precision);
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


