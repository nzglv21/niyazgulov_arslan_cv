#pragma once
#include "iostream"
#include "cstring"
#include "fstream"
class HTML_Element{
public:
    virtual void render(std::ostream& f = std::cout) = 0;
};

char* strCpy(const char* from);   

void cast(HTML_Element& p);