#include "HTML_Text.hpp"

HTML_Text::HTML_Text(const char* text_, const char* tag_){
    text = strCpy(text_);
    tag = strCpy(tag_);    
}
HTML_Text::~HTML_Text(){
    delete[] text;
    delete[] tag;
}

void HTML_Text::render(std::ostream& f){
    f  << "<" <<tag << ">";
    f  << text;
    f  << "</" << tag << ">\n";
}

HTML_b::HTML_b(const char* text_): HTML_Text(text_, "b"){
};

HTML_i::HTML_i(const char *text_): HTML_Text(text_, "i"){};

HTML_u::HTML_u(const char* text_): HTML_Text(text_, "u"){};

HTML_strike::HTML_strike(const char* text_): HTML_Text(text_, "strike"){};

HTML_p::HTML_p(const char* text_): HTML_Text(text_, "p"){};

