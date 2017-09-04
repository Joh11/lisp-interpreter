#include <stdexcept>
#include <regex>

#include "number.hpp"

#include "utils.hpp"

rational::rational(int numerator, int denominator) : _num(numerator), _denom(denominator)
{
    if(denominator == 0)
	throw std::runtime_error("Divison by zero when trying to instantiate a rational");
    
    reduce();
}

rational& rational::operator+=(rational const& r)
{
    _num = _num * r._denom + r._num * _denom;
    reduce();

    return *this;
}

rational& rational::operator-=(rational const& r)
{
    *this += r * (-1);

    return *this;
}

rational& rational::operator*=(rational const& r)
{
    _num *= r._num;
    _denom *= r._denom;

    reduce();

    return *this;
}

rational& rational::operator/=(rational const& r)
{
    *this *= r.inverse();

    return *this;
}

bool rational::operator<(rational const& r) const
{
    if(r == 0)
	return _num < 0;

    return (*this - r) < 0;
}

rational rational::inverse() const
{
    if(_denom == 0)
	throw std::runtime_error("You cannot compute the inverse of zero");

    return rational(_denom, _num);
}

void rational::reduce()
{
    int g{gcd(_num, _denom)};

    _num /= g;
    _denom /= g;

    // Make sure that only the numerator will be negative
    if(_denom < 0)
    {
	_denom *= -1;
	_num *= -1;
    }
}

bool is_rational(std::string const& str)
{
    return std::regex_match(str, std::regex("-?\\d+/?\\d*"));
}

rational str_to_rational(std::string str)
{
    if(!is_rational(str))
	throw std::runtime_error("Not a rational !");

    int num = 1;
    
    if(str[0] == '-')
    {
	num = -1;
	str = str.substr(1, std::string::npos);
    }

    size_t slashPos{str.find('/')};

    if(slashPos == std::string::npos)
    {
	// It is only an integer
	return rational{num * std::stoi(str)};
    }

    num *= std::stoi(str.substr(0, slashPos - 1));
    int denom{std::stoi(str.substr(slashPos + 1, std::string::npos))};

    return rational{num, denom};
}
