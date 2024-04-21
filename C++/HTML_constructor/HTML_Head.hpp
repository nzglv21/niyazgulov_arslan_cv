#pragma once
#include "HTML_Title.hpp"

class HTML_Head: virtual public HTML_Element{
    friend class HTML;
    friend class HTML_Div;
    HTML_Title * Title;
    void render(std::ostream& f = std::cout) override;
public:
    HTML_Head();
    HTML_Head(const char* title);
    
    ~HTML_Head();
    // void addElemetn(HTML_Title * p_title);
};