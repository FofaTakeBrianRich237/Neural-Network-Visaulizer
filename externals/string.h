#pragma once 
#include <iostream>

class string
{
    public:
        char * String = nullptr;    
        int length = 0;

    public:
        string(){};
        string(const char* word) { copy(word); }
        string(const string& word) { copy(word); }
        ~string()
        {
            delete[]String;
        }
    
    public:
        friend std::ostream& operator<<(std::ostream& flux,const string&);
        friend std::istream& operator>>(std::istream& flux, string&);
        void operator=(const string& st) { copy(st); }
        void operator=(const char* st) { copy(st); }
        char& operator[](const int& pos) const { return String[(pos >= 0) ? pos : length + pos]; }
        operator const char*() { return this->String; }

        void operator+=(const string&);
        void operator+=(const char * st);
        void operator+=(const char& st);

    public:
        void replace(const char&,const char&);
        void replace(const int&,const char&);
        int count(const char&);
        string insert(const char&,const int& position = -1);
        string insert_at_end(const char&);
        string remove(const char&);
        string cut(const int&,int);
        string del (const int&,const int& end = -1);
        bool IsEmpty() { return (length == 0) ? true : false; }

    private:
        void copy(const char*);
        void copy(const string&);
        void fill(const char&,const int&);
        void fill(const char*,const int&);
        void fill(const string&,const int&);
};

bool operator==(const string&, const string&);
bool operator==(const string&, const char*);
bool operator!=(const string&, const string&);
bool operator!=(const string&, const char*);

string operator+(const string&, const string&);

static const char* const_char(const string& st) { return st.String; }



