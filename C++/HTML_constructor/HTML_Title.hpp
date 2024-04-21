#include "HTML_Element.hpp"


class HTML_Title: public HTML_Element{
    friend class HTML_Head;
    char *title;
    void render(std::ostream& f = std::cout) override;
public:    
    HTML_Title();
    HTML_Title(const char* HTML_Head);
    void setTitle(char* new_title);
    ~HTML_Title();
};




