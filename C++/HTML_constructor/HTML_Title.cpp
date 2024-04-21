#include "HTML_Title.hpp"

HTML_Title::HTML_Title(){
    title = nullptr;
}

HTML_Title::HTML_Title(const char *t){
    title = strCpy(t);
}

void HTML_Title::setTitle(char* new_title){
    delete title;
    title = new_title;
}

void HTML_Title::render(std::ostream& f){

    f  <<"<title>";
    if (title == nullptr)
        f  << "empty title";
    else
        f  << title;
    
    f  <<"</title>\n";
}

HTML_Title::~HTML_Title(){
    if (title != nullptr)
        delete[] title;
}