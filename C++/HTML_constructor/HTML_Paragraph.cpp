#include "HTML_Paragraph.hpp"

HTML_Paragraph::HTML_Paragraph(const char* t){
    text = strCpy(t);
}
HTML_Paragraph::~HTML_Paragraph(){
    delete[] text;
}

void HTML_Paragraph::render(std::ostream& f){
    f  << "<p>" << text << "</p>\n";
}