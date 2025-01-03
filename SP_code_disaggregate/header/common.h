#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
template <class T>
std::ostream& operator<<(std::ostream& out, std::vector<T>& v) 
{
    out << "[";

    for (int i = 0; i < v.size() - 1; i++)
        out << v[i] << ", ";

    out << *(v.end()--) << "]";

    return out;
}


#endif