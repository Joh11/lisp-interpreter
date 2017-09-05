#ifndef NUMBER_HPP
#define NUMBER_HPP

#include <string>
#include <ostream>

// A file that will contain all the stuff to use numbers in LISP :
// String to number, vice-versa, and a class for rational numbers


// A class that represent a rational number
class rational
{
public:
    // Constructors
    rational(int number = 0) : _num{number}, _denom{1} {}
    rational(int numerator, int denominator);
    rational(rational const& r) : rational(r._num, r._denom) {}
    rational operator=(rational const& r) {_num = r._num; _denom = r._denom; return *this; }

    // Operators
    rational& operator+=(rational const& r);
    rational& operator-=(rational const& r);
    rational& operator*=(rational const& r);
    rational& operator/=(rational const& r);

    bool operator==(rational const& r) const {return _num == r._num && _denom == r._denom; }
    bool operator!=(rational const& r) const {return !(*this == r); }
    bool operator<(rational const& r) const;
    bool operator<=(rational const& r) const {return *this < r || *this == r; }
    bool operator>(rational const& r) const {return !(*this <= r); }
    bool operator>=(rational const& r) const {return !(*this < r); }

    double to_double() const {return (double)_num / _denom;}

    rational inverse() const;

    std::ostream & display(std::ostream & stream) const;

private:
    // Divide each term by their gcd
    void reduce();
    
    int _num, _denom;
};

// Operators
inline rational operator+(rational a, rational const& b){return a += b; }
inline rational operator-(rational a, rational const& b){return a -= b; }
inline rational operator*(rational a, rational const& b){return a *= b; }
inline rational operator/(rational a, rational const& b){return a /= b; }

inline std::ostream & operator<<(std::ostream & stream, rational r) {return r.display(stream);}

// Check using regex
bool is_rational(std::string const& str);
rational str_to_rational(std::string const& str);

namespace std
{
    std::string to_string(rational const& r);
}
    
#endif
