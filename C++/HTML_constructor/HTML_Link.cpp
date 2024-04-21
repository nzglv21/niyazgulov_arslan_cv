#include "HTML_Link.hpp"

HTML_Link::HTML_Link(const char* text_, const char* url_){
    text = strCpy(text_);
    href = strCpy(url_);
}

HTML_Link::~HTML_Link(){
    delete[] text;
    delete[] href;
}

void HTML_Link::render(std::ostream& f=std::cout){
    f << "<a href=" <<href<<">";
    f << text<< "</a>";
}