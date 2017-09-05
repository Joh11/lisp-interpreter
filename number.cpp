#include <stdexcept>
#include <regex>
#include <sstream>
#include <iostream>

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
    _denom *= r._denom;
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

std::ostream & rational::display(std::ostream & stream) const
{
    stream << _num;
    if(_denom != 1)
	stream << '/' << _denom;

    return stream;
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

rational str_to_rational(std::string const& str)
{
    std::string strCopy{str};
    
    if(!is_rational(strCopy))
	throw std::runtime_error("Not a rational !");

    int num = 1;
    
    if(strCopy[0] == '-')
    {
	num = -1;
	strCopy = str.substr(1, std::string::npos);
    }

    size_t slashPos{strCopy.find('/')};

    if(slashPos == std::string::npos)
    {
	// It is only an integer
	return rational{num * std::stoi(strCopy)};
    }

    num *= std::stoi(strCopy.substr(0, slashPos));
    int denom{std::stoi(strCopy.substr(slashPos + 1, std::string::npos))};

    return rational{num, denom};
}

namespace std
{
    std::string to_string(rational const& r)
    {
	std::stringstream ss;

	r.display(ss);

	return ss.str();
    }
}
