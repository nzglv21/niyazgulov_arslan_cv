#include "HTML_Element.hpp"

class HTML_Paragraph: public HTML_Element{
    friend class HTML;
    friend class HTML_Div;
    char* text;
    void render(std::ostream& f = std::cout) override;
public:
    HTML_Paragraph(const char *t);
    ~HTML_Paragraph();
};