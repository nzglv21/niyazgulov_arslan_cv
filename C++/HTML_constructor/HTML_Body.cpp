#include "HTML_Body.hpp"


HTML_Body::HTML_Body(){

    body_elements = new HTML_Element*[10];
    array_size = 10;
    elements_counts = 0;
    for (int i=0; i<array_size; ++i)
        body_elements[i] = nullptr;
    
}

void HTML_Body::realloc(){
    HTML_Element **new_elements;
    new_elements = new HTML_Element*[array_size+10];

    for (int i = 0;i < array_size; ++i)
        new_elements[i] = body_elements[i];

    delete[] body_elements;
    body_elements = new_elements;
    array_size +=10;
}

HTML_Body& HTML_Body::addElement(HTML_Element& p){
    
    cast(p);

    if (elements_counts<array_size){
        body_elements[elements_counts] = &p;
        ++elements_counts;
    }
    else{
        realloc();
        body_elements[elements_counts] = &p;
        ++elements_counts;
    }
    return *this;
}

void HTML_Body::render(std::ostream& f ){
    f  <<"<body>\n";
    for (int i = 0; i < elements_counts; ++i){
        body_elements[i]->render(f);
    }
    f  <<"</body>\n";
}

HTML_Body::~HTML_Body(){
    delete[] body_elements;
}