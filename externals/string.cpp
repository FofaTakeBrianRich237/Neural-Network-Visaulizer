#include "string.h"

std::ostream& operator<<(std::ostream& flux,const string& st)
{
    if(st.length != 0) flux << st.String;
    else flux << "";
    // for(int i = 0; i < st.length; i++) flux << st[i];
    return flux;
}

std::istream& operator>>(std::istream& flux, string& st)
{
    int bufferCap = 20;
    int bufferlength = 0;
    char * buffer = new char[bufferCap];
    char c;
    auto lambda = [&c]() 
    {
        if(c == '\n' || c == '\t' || c == '\r' || c == EOF || c == '\0') return false;
        else return true;
    };

    while(flux.get(c) && lambda())
    {
        if(bufferlength + 1 >= bufferCap)
        {
            bufferCap *= 2;
            char* Nbuffer = new char[bufferCap];
            for(int i = 0; i < bufferlength; i++) Nbuffer[i] = buffer[i];
            delete[]buffer;
            buffer = Nbuffer;
            delete[]Nbuffer;
        }
        buffer[bufferlength++] = c;
    }
    buffer[bufferlength] = '\0';
    // if(st.String != nullptr) free(st.String);
    st.~string();
    st.copy(buffer);
    delete[]buffer;

    return flux;
}

string string::cut(const int& start,int end = 0)
{
    string temp;
    if (end < 0) end = this->length + end;
    for(int i = start; i < end+1; i++) temp += this->String[i];
    return temp;
}

void string::copy(const char * st)
{
    int len;
    for(len = 0; st[len] != '\0'; len++);

    if(this->String != nullptr) this->String = nullptr;
    this->String = new char[len+1];
    this->length = len;

    for(int i = 0; st[i] != '\0'; i++) this->String[i] = st[i];
    this->String[len] = '\0';
}

string string::remove(const char& val)
{
    int count = 0;
    for(int i = 0; i < length; i++)
    {
        if(String[i] == val) count++;
    }
    string temp = *this;
    length -= count;
    delete [] String;
    String = new char[length+1];
    
    for(int i = 0, j = 0; i < length && j < temp.length; i++,j++)
    {
        if(temp[j] == val)
        {
            for(j++; temp[j] == val ; j++);
        }
        String[i] = temp[j];
    }
    (*this)[length] = '\0';

    return *this;
}

int string::count(const char& val)
{
    int c = 0;
    for(int i = 0; i < length; i++)
    {
        if(String[i] == val) c++;
    }
    return c;
}

void string::replace(const int& posistion,const char& val) { String[posistion] = val; }

void string::replace(const char& out,const char& in)
{
    for(int i = 0; i < length; i++)
    {
        if(String[i] == out) String[i] =  in;
    }
}

string string::insert(const char& val,const int& position)
{
    int pos = (position > 0) ? position : length + position;
    string temp = *this;

    delete [] String;
    length++;
    

    String = new char[length+1];

    for(int i = 0, j = 0; i < length && j < temp.length; i++)
    {
        if(i != pos)  
        {
            String[i] = temp[j];
            j++;
        }
        else String[i] = val;
    }
    String[length] = '\0';

    return *this;
}

string string::insert_at_end(const char& val)
{
    string temp = *this;

    delete [] String;
    length++;
    

    String = new char[length+1];

    for(int i = 0, j = 0; i < length && j < temp.length; i++)
    {
        String[i] = temp[j];
        j++;
    }
    String[length-1] = val;
    String[length] = '\0';

    return *this;
}

void string::copy(const string& st)
{
    // if(st.length != 0)
    // {
        if(this->String != nullptr) delete[] String;
        this->String = new char[(this->length = st.length)+1];
        for(int i = 0; i < this->length; i++) this->String[i] = st.String[i];
        this->String[this->length] = '\0';
    // }

    // if(this->String != nullptr) delete[] String;
    // this->String = new char[((((st.length == 0) ? 0 : this->length = st.length)+1))];
    // for(int i = 0; i < this->length; i++) this->String[i] = st.String[i];
    // this->String[this->length] = '\0';
}

void string::fill(const char* st, const int& start = 0)
{
    int len;
    for(len = 0; st[len] != '\0'; len++);
    for(int i = start, j = 0; (i < this->length && j < len); i++,j++) this->String[i] = st[j];
}

void string::fill(const char& c,const int& pos)
{
    if(pos >= 0 && pos < length) String[pos] = c;
}

void string::fill(const string& st, const int& start = 0)
{
    if(st.String != nullptr)
    {
        for(int i = start, j = 0; (i < this->length && j < st.length); i++,j++) 
        this->String[i] = st.String[j];
    }
}

string string::del (const int& st,const int& en)
{
    int end,start;

    if(st >= 0) start = st;
    else start = length + st;

    if(en == 0) end = start;
    else if(en > 0) end = en;
    else end = length + en;

    int len = (end - start)+1;

    string temp = *this;

    delete[] String;

    length -= len;
    String = new char[length+1];

    for(int i = -1,j = 0; (i < temp.length) && (j < length); j++)
    {
        if(i == start)
        {
            i = end+1;
        }
        else i++;
        String[j] = temp[i];
    }


    (*this)[length] = '\0';
    return *this;
}


bool operator==(const string& init, const string& other)
{
    bool same  = true;
    if(init.length != other.length) same = false;
    else 
    {
        for(int i = 0; i < init.length; i++) 
        {
            if(init.String[i] != other.String[i]) 
            { 
                same = false; 
                break;
            }
        }
    }

    return same;
}

bool operator!=(const string& init, const string& other)
{
    bool same  = false;
    if(init.length != other.length) {same = true;}
    else 
    {
        for(int i = 0; i < init.length; i++) 
        {
            if(init.String[i] != other.String[i]) 
            { 
                same = true; 
                break;
            }
        }
    }

    return same;
}

bool operator==(const string& init, const char* st)
{
    bool same  = true;
    int len;
    for(len = 0; st[len] != '\0'; len++);
    if(init.length != len) same = false;
    else 
    {
        for(int i = 0; i < init.length; i++) 
        {
            if(init.String[i] != st[i]) 
            { 
                same = false; 
                break;
            }
        }
    }

    return same;
}

bool operator!=(const string& init, const char* st)
{
    bool same  = false;
    int len;
    for(len = 0; st[len] != '\0'; len++);
    if(init.length != len) same = true;
    else 
    {
        for(int i = 0; i < init.length; i++) 
        {
            if(init.String[i] != st[i]) 
            { 
                same = true; 
                break;
            }
        }
    }

    return same;
}

void string::operator+=(const string& st)
{
    string temp;

    if(this->String != nullptr) 
    {
        temp = *this;
        this->String = nullptr;
    }
    this->length += st.length;
    this->String = new char[this->length+1];

    fill(temp);
    fill(st,temp.length);
    this->operator[](length) = '\0';

}

void string::operator+=(const char* st)
{
    int len;
    for(len = 0; st[len] != '\0'; len++);
    string temp;
    temp = *this;
    this->length += len;
    delete[]String;
    this->String = new char[this->length+1];
    fill(temp);
    fill(st,temp.length);
    this->operator[](length) = '\0';
}

void string::operator+=(const char& st)
{
   string temp;

   if(this->String != nullptr) 
    {
        temp  = *this;
        this->String = nullptr; 
    }
    this->length ++;
    this->String = new char[this->length+1];
    fill(temp);
    fill(st,temp.length);
    this->operator[](length) = '\0';
}

string operator+(const string& one, const string& two)
{
    string temp;
    temp += one;
    temp += two;
    return temp;
}



