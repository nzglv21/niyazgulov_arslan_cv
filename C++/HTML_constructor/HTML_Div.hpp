#include "HTML_Element.hpp"

class HTML_Div: public HTML_Element{
    friend class HTML;
    char* id;
    HTML_Element **div_elements;
    int div_counts;
    int array_size;

    void render(std::ostream& f = std::cout) override;
public:

    HTML_Div();

    HTML_Div(const char*);

    ~HTML_Div();

    HTML_Div& addElement(HTML_Element& el);

};