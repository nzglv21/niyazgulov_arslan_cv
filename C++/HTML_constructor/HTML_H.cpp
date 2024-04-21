#include "HTML_H.hpp"

H::H(const char* t, int i){
    text = strCpy(t);
    level = i;
}

void H::render(std::ostream& f) {
    f  << "<h" << level << ">" << text << "</h" << level << ">\n";
}

H::~H() {
    delete[] text;
}

H1::H1(const char* t) : H(t, 1) {}

H2::H2(const char* t) : H(t, 2) {}

H3::H3(const char* t) : H(t, 3) {}

H4::H4(const char* t) : H(t, 4) {}

H5::H5(const char* t) : H(t, 5) {}

H6::H6(const char* t) : H(t, 6) {}

