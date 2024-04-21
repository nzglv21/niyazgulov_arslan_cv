#include "HTML.hpp"
char* strCpy(const char* from){
    if (from == nullptr)
        return nullptr;
    int len = strlen(from) + 1;
    char* to = new char[len];
    strcpy(to, from);
    return to;
}

void cast(HTML_Element& el){
}


HTML::HTML(): HTML_Head(), HTML_Body(){}

HTML::HTML(const char* title = nullptr): HTML_Head(title), HTML_Body(){}

HTML_Body& HTML::getBody(){
    return *(HTML_Body*)this;
}

HTML_Head& HTML::getHead(){
    return *(HTML_Head*)this;
}

void HTML::convert_to_file(const char* fileName){
    std::ofstream file(fileName);
    if (file.is_open()){
        file << "<!DOCTYPE html>\n";
        file << "<html>\n";
        HTML_Head::render(file);
        HTML_Body::render(file);
        file << "</html>\n";
    }else{
        std::cout << "<!DOCTYPE html>\n";
        file << "<html>\n";
        HTML_Head::render();
        HTML_Body::render();
        std::cout << "</html>\n";
    }
}

void HTML::render(std::ostream& p){}

