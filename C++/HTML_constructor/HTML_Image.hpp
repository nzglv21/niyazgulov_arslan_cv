#include "HTML_Element.hpp"

class HTML_Image: public HTML_Element{
    friend class HTML;
    friend class HTML_Div;
    char* src;
    int width;
    int height;
    char* title;
    void render(std::ostream& f = std::cout) override;

public:
    HTML_Image(const char* src_, int width_=200, int high_=200, const char* title_=nullptr);
    ~HTML_Image();
};