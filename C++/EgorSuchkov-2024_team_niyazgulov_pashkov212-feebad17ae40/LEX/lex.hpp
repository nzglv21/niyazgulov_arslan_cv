#pragma once
#include <iostream>
#include <fstream>
#include "../CONSTS/consts.hpp"
#include "../ID_TID/identificator.hpp"
using namespace std;


class Lex {
    type_of_lex t_lex;
    int v_lex;
public:
    Lex();
    Lex(type_of_lex t, int v);
    type_of_lex get_of_type() const;
    int get_value() const;
    friend ostream& operator<< (ostream &s, Lex &l);
};

class Scaner{
    FILE *f;
    char c;
    int look (const string buf, const char ** list );
    Lex prev_lex;
    bool is_EOF = false;
    
public:
    static const char *TW[], *TD[];
    static bool is_newline;
    Scaner (const char* program);
    void get_back(Lex l);
    void gc();
    void putc();
    Lex get_lex();
    bool is_prev;
};