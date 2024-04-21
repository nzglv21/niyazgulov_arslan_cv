#include "HTML_Image.hpp"

HTML_Image::HTML_Image(const char* src_=nullptr, int width_, int height_, const char* title_){
    src = strCpy(src_);
    width = width_;
    height = height_;
    title = strCpy(title_);
}

HTML_Image::~HTML_Image(){
    delete[] src;
    delete[] title;
}

void HTML_Image::render(std::ostream& f){
    f  << "<img src=\"" << src << "\" width=\"" << width << "\" height=\"" << height << "\"";
    
    if (title != nullptr) {
        f  << " alt=\"" << title << "\"";
        f  << " title=\"" << title << "\"";
    }

    f  << "class = \"center\"";
    
    f  << ">" << std::endl;
}
