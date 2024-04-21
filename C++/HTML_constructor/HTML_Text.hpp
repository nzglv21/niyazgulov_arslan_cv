#include "HTML_Element.hpp"

class HTML_Text: public HTML_Element{
    friend class HTML;
    char* text;
    char* tag;
protected:
    HTML_Text(const char*, const char*);
    ~HTML_Text();
    void render(std::ostream& f = std::cout) override;
};

class HTML_b: public HTML_Text{
public:
    HTML_b(const char*);
};

class HTML_i: public HTML_Text{
public:    
    HTML_i(const char*);
};

class HTML_u:public HTML_Text{
public:
    HTML_u(const char*);
};

class HTML_strike: public HTML_Text{
public:
    HTML_strike(const char*);
};

class HTML_p: public HTML_Text{
public:
    HTML_p(const char*);
};