#pragma one
#include <iostream>

int merde;
template<typename type, int m_size>
class arr
{
    public:
    type * array = nullptr;
    int size = m_size;

    public :
    arr() { array = new type[size];}
    ~arr() { delete[]array; }
    arr(const arr<type,m_size>& other)
    {
        if(this->size == other.size)
        {
            for(int i = 0; i < size; i++) this->operator[](i) = other[i];
        }
        else std::cout << "Different sizes" << std::endl;
    }


    public :
    type& At(const int& pos)
    {
        if(pos >= 0 && pos < this->size) return this->array[pos];
        else 
        {
            std::cout << "invalid position" << std::endl;
            return array[this->size];
        }
    }

    type& Front() { return array[0]; }

    type& Back() { return array[size - 1]; }

    void copy(const arr<type,m_size>& other)
    {
        if(this->size == other.size)
        {
            for(int i = 0; i < size; i++) this->operator[](i) = other[i];
        }
        else std::cout << "Different sizes" << std::endl;
    }

    void Fill(const type& Value) { for(int i = 0; i < this->size; i++) this->array[i] = Value; }

    void Swap(arr<type,m_size>& other)
    { 
        type temp;

        auto lambda = [&temp,&other](arr<type,m_size>* val,int limit)
        {
            for(int i = 0; i < limit;i++)
            {
                temp = val->array[i];
                val->array[i] = other.array[i];
                other.array[i] = temp;
            }
        };

        if(other.size == this->size || other.size < this->size) lambda(this,this->size);
        else lambda(this,other.size);
    }

    bool Belong(const type& val)
    {
        if(size != 0)
        {
            bool found = false;

            for(int i = 0; i < size; i++)
            {
                if(array[i] == val) found = true;
            }
            return found;
        }
        else return false;
    }

    constexpr int Size() const { return this->size; }
    
    class Iterator
    {
        public:
        type * it;

        public:
        Iterator(){};
        Iterator(type* val) {it = val;}
        // Iterator(const Iterator& other) { this->it = other.it; }

        public:
        type& operator*() { return *it; }
        Iterator& operator++(int) { it++; return *this; }
        Iterator& operator++() { ++it; return *this; }
        Iterator& operator--(int) { it--; return *this; }
        Iterator operator+(const int& val) 
        { 
            Iterator temp(*this);
            temp.it = temp.it + 1;
            return temp;
        }

        Iterator operator-(const int& val) 
        { 
            Iterator temp(*this);
            temp.it = temp.it - 1;
            return temp;
        }
        const bool operator!=(const Iterator& other) { if(this->it != other.it) return true; else return false;}
        const bool operator==(const Iterator& other) { if(this->it == other.it) return true; else return false; }
    };

    // Iterator begin() { Iterator a(array); return a; }
    // Iterator end() { Iterator a(array + (size)); return a; }

    Iterator begin() { return Iterator(this->array); }
    Iterator end() { return Iterator(array - size); }

    public :
    type& operator[](const int& pos) const
    {
        if(pos >= 0 && pos < this->size) return this->array[pos];
        else 
        {
            std::cout << "invalid position" << std::endl;
            return array[this->size];
        }
    }
    
    void operator=(const arr<type,m_size>& other)
    {
        if(this->size == other.size)
        {
            for(int i = 0; i < size; i++) this->operator[](i) = other[i];
        }
        else std::cout << "Different sizes" << std::endl;
    }

};

template<typename type, int size>
std::ostream& operator<<(std::ostream& flux,const arr<type,size>& a)
{
    for(int i = 0; i < a.size; i++) { flux << a.array[i]; if(i < a.size-1) flux << ",";}
    return flux;
}
