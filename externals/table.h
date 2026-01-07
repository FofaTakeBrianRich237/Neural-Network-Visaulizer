#ifndef __MDTA_H__
#define __MDTA_H__

#include <iostream>
template<typename type1, typename type2, int m_size>
class table
{
    public:
        type1 * array1;
        type2 * array2;
        int size = m_size;

    public:
        table() 
        {
            array1 = new type1[size];
            array2 = new type2[size];
        }

    public:
        int Size() const { return size; }
        type1& first(const int& postion  = 0) { return array1[postion]; }
        type2& second(const int& postion = 0) { return array2[postion]; }

    public:
        friend std::ostream& operator<<(std::ostream& flux,const table& tab)
        {
            for(int i = 0; i < tab.Size(); i++) 
            flux << tab.array1[i] << "  " << tab.array2[i] << std::endl;
            return flux;
        }
};

#endif