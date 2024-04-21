#pragma once
#include "HTML_Element.hpp"
class HTML_Body: virtual public HTML_Element{
    HTML_Element** body_elements;
    int array_size;
    int elements_counts;
    void realloc();
public:
    HTML_Body();
    HTML_Body& addElement(HTML_Element &p);
    ~HTML_Body();
protected:
    void render(std::ostream& f = std::cout) override;
};