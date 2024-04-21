#include "HTML_Element.hpp"

class H: public HTML_Element{
    friend class HTML;
    friend class HTML_Div;
protected:
    int level;
    char* text;
    H(const char* t, int i=1);
    void render(std::ostream& f = std::cout) override;
    ~H();
};
class H1 : public H {
public:
    H1(const char* t);
};

class H2 : public H {
public:
    H2(const char* t);
};

class H3 : public H {
public:
    H3(const char* t);
};

class H4 : public H {
public:
    H4(const char* t);
};

class H5 : public H {
public:
    H5(const char* t);
};

class H6 : public H {
public:
    H6(const char* t);
};