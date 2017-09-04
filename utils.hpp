#ifndef UTILS_HPP
#define UTILS_HPP

#include <list>
#include <cstdlib>

template <typename InputIterator, typename Class>
Class sub_list(InputIterator first, InputIterator last);

template <typename T>
T gcd(T a, T b);


// Build a sub list
template <typename InputIterator, typename Class>
Class sub_list(InputIterator first, InputIterator last)
{
    // Build an empty container
    Class c{};

    // Add all the elements in the range
    for(auto it = first ; it != last ; ++it)
	c.push_back(*it);

    return c;
}

// The gcd
template <typename T>
T gcd(T a, T b)
{
    return b == 0 ? abs(a) : gcd(b, a % b);
}

#endif
