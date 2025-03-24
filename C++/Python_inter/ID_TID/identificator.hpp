#pragma once
#include <vector>
#include <algorithm>
#include "../CONSTS/consts.hpp"
#include <string>
#include <ostream>

enum type {NULL_TYPE, INT_TYPE, STRING_TYPE};
class Data{
    int     int_val;
    std::string  str_val;
    type data_type = NULL_TYPE;
public:
    Data(): data_type(NULL_TYPE){};
    Data(int i): data_type(INT_TYPE), int_val(i){};
    Data(std::string i): data_type(STRING_TYPE), str_val(i){};
    void set_data(Data i){
        data_type = i.data_type;
        if (data_type == INT_TYPE)
            int_val = i.int_val;
        else if (data_type == STRING_TYPE)
            str_val = i.str_val;
    };
    Data operator=(Data i){
        data_type = i.data_type;
        if (data_type == INT_TYPE)
            int_val = i.int_val;
        else if (data_type == STRING_TYPE)
            str_val = i.str_val;
        
        return *this;
    };
    int get_int(){return int_val;};
    std::string get_str(){return str_val;};
    type get_type(){return data_type;};
    friend std::ostream& operator<<(std::ostream& os, Data& x){
        if (x.get_type() == INT_TYPE)
            os << x.get_int();
        else if(x.get_type() == STRING_TYPE)
            os << x.get_str();
        return os;
    };
    ~Data(){

    };
};

class Ident {
    std::string name;
    type_of_lex type;
    Data        value;
public:
    Ident();
    bool operator==(const std::string& s) const;
    Ident(const std::string n);
    type_of_lex get_type() const;
    Data get_value(){return value;};
    void put_value(Data v);
};

int put(const std::string & buf);