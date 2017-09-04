#ifndef UTILS_HPP
#define UTILS_HPP

#include <list>

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

#endif
