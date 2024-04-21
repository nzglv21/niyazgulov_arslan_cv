#include "HTML_Element.hpp"

class HTML_Link: public HTML_Element{
    friend class HTML;
    friend class HTML_Div;
    char* text;
    char* href;
    void render(std::ostream&);
public:
    HTML_Link(const char* href_, const char* text_);
    ~HTML_Link();
};