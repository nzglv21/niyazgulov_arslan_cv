#include "identificator.hpp"

std::vector<Ident> TID;
std::vector<std::string> STRT;

Ident::Ident(){}

bool Ident::operator==(const std::string& s) const 
{ 
    return name == s; 
}

Ident::Ident(const std::string n) 
{
    name    = n;
}

type_of_lex Ident::get_type() const 
{ 
    return type; 
}

void Ident::put_value(Data v) 
{ 
    value = v; 
}

int put(const std::string & buf)
{
    std::vector<Ident>::iterator k;
 
    if ((k = std::find(TID.begin(), TID.end(), buf)) != TID.end())
        return k - TID.begin();
    TID.push_back(Ident(buf));
    return TID.size() - 1;
}
