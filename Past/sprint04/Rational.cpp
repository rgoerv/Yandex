#include <iostream>
#include <numeric>

using namespace std;

class Rational {
public:
    Rational() = default;

    Rational(int numerator)
        : numerator_(numerator)
        , denominator_(1)
    {
    }

    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator)
    {
        Normalize();
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

    friend Rational operator+(Rational left, Rational right) {
        const int numerator = left.Numerator() * right.Denominator() 
                      + right.Numerator() * left.Denominator();
        const int denominator = left.Denominator() * right.Denominator();

        return {numerator, denominator};
    }

    friend Rational operator-(Rational left, Rational right) {
        const int numerator = left.Numerator() * right.Denominator() 
                      - right.Numerator() * left.Denominator();
        const int denominator = left.Denominator() * right.Denominator();

        return {numerator, denominator};
    }

    friend Rational operator*(const Rational left, const Rational right) {
        return Rational{left.Numerator() * right.Numerator(), left.Denominator() * right.Denominator()};
    }

    friend Rational operator/(const Rational left, const Rational right) {
        Rational rd(right.Denominator(), right.Numerator());
        return left * rd;
    }

    Rational & operator+=(Rational rational)
    {
        *this = *this + rational;
        Normalize();
        return *this;
    }

    Rational & operator-=(Rational rational)
    {
        *this = *this - rational;
        Normalize();
        return *this;
    }

    Rational & operator*=(Rational rational)
    {
        *this = *this * rational;
        Normalize();
        return *this;
    }

    Rational & operator/=(Rational rational)
    {
        *this = *this / rational;
        Normalize();
        return *this;
    }

private:
    void Normalize() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        int n = gcd(numerator_, denominator_);
        numerator_ /= n;
        denominator_ /= n;
    }

    int numerator_ = 0;
    int denominator_ = 1;
};

Rational operator+(const Rational arg) {
    return arg;
}

Rational operator-(const Rational arg) {
    return {-arg.Numerator(), arg.Denominator()};
}

bool operator==(Rational left, Rational right) {
    return left.Numerator() == right.Numerator() && 
           left.Denominator() == right.Denominator();
} 

bool operator!=(Rational left, Rational right) {
    return !(left == right);
}

bool operator<(Rational left, Rational right) {
    return ((-(left - right)) - (left - right)) == 0;
}
bool operator>(Rational left, Rational  right) { return !(right < left); }
bool operator<=(Rational  left, Rational  right) { return (right < left) && !(right == left); }    
bool operator>=(Rational  left, Rational  right) { return (left > right) && !(right == left); }

ostream& operator<<(ostream& output, Rational rational) {
    output << rational.Numerator() << '/' << rational.Denominator();
    return output;
}

istream& operator>>(istream& input, Rational& rational) {
    int numerator, denominator;
    char slash;
    input >> numerator >> slash >> denominator;
    rational = Rational{numerator, denominator};
    return input;
}



